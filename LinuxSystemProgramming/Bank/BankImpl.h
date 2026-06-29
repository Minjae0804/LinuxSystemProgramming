#pragma once
#include "Bank.h"
#include "../SystemUtils/FDGuard.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <vector>

namespace bank {
    std::vector<std::unique_ptr<BankAccount>>::iterator Bank::findIterator(int id) {
        return std::find_if(accountList.begin(), accountList.end(), 
            [&](const std::unique_ptr<BankAccount>& account) { return account->getId() == id; }
        );
    }

    int Bank::addAccount(const BankAccount& account) {
        accountList.push_back(std::unique_ptr<BankAccount>(account.clone()));

        return accountList.back()->getId();
    }
    
    BankAccount* Bank::findAccount(int id) {
        auto it = findIterator(id);
        if (it == accountList.end()) throw std::runtime_error("account not found: " + std::to_string(id));

        return it->get();
    }

    bool Bank::deleteAccount(int id) {
        auto it = findIterator(id);
        if (it == accountList.end()) return false;
        accountList.erase(it);

        return true;
    }

    bool Bank::updateAccountOwnername(int id, const std::string& newname) {
        auto it = findIterator(id);
        if (it == accountList.end()) return false;
        (*it)->setOwnerName(newname);

        return true;
    }

    std::vector<std::string> Bank::printAllAccount() const {
        std::vector<std::string> res;
        for (const auto& account : accountList)
            res.push_back(account->printAccountInfo());

        return res;
    }
    
    void Bank::save(const std::string& filename) {
        FDGuard fd(creat(filename.c_str(), 0644));
        if (fd.get() == -1) throw std::runtime_error(std::string("creat failed: ") + strerror(errno));

        for (const auto& account : accountList) {
            std::string line =
                account->getType() + "," + 
                std::to_string(account->getId()) + "," + 
                account->getOwnerName() + "," + 
                std::to_string(account->getBalance()) + "\n";
            ssize_t written = write(fd.get(), line.c_str(), line.size());
            if (written == -1) throw std::runtime_error(std::string("creat failed: ") + strerror(errno));
        }
    }

    // 계좌 데이터를 임시 저장하는 구조체
    struct AccountData {
        std::string type;
        int id;
        std::string ownerName;
        int money;
    };

    // 파일 내 라인을 받고 계좌 데이터로 반환
    AccountData parsAccountLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) tokens.push_back(token);

        return AccountData(
            tokens[0], 
            std::stoi(tokens[1]), 
            tokens[2], 
            std::stoi(tokens[3])
        );
    }

    // 계좌 로드용 팩토리 함수
    std::unique_ptr<BankAccount> createAccount(const AccountData& data) {
        if      (data.type == "SavingsAccount")     return std::make_unique<SavingsAccount> (data.id, data.ownerName, data.money);
        else if (data.type == "CheckingAccount")    return std::make_unique<CheckingAccount>(data.id, data.ownerName, data.money);
        else if (data.type == "BankAccount")        return std::make_unique<BankAccount>    (data.id, data.ownerName, data.money);

        throw std::runtime_error("unknown account error: " + data.type);
    }
    
    void Bank::load(const std::string& filename) {
        FDGuard fd(open(filename.c_str(), O_RDONLY));
        if (fd.get() == -1) throw std::runtime_error(std::string("open failed: ") + strerror(errno));

        struct stat st;
        fstat(fd.get(), &st);
        std::vector<char> buffer(st.st_size);
        ssize_t bytesRead = read(fd.get(), buffer.data(), buffer.size());
        if(bytesRead == -1) throw std::runtime_error(std::string("read failed: ") + strerror(errno));

        std::vector<AccountData> datas;
        std::string content(buffer.data(), buffer.size());
        std::stringstream ss(content);
        std::string dataLine;
        while (std::getline(ss, dataLine))
            datas.push_back(parsAccountLine(dataLine));
        
        for (const AccountData& accountData : datas)
            accountList.push_back(createAccount(accountData));
    }
}