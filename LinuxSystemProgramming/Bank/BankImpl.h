#pragma once
#include "Bank.h"
#include "../SystemUtils/FDGuard.h"
#include <algorithm>
#include <string>
#include <fcntl.h>
#include <unistd.h>

namespace bank {

    int Bank::addAccount(const BankAccount& account) {
        accountList.push_back(std::unique_ptr<BankAccount>(account.clone()));

        return accountList.back()->getId();
    }

    BankAccount* Bank::findAccount(int id) {
        auto it = std::find_if(accountList.begin(), accountList.end(), 
            [&](const std::unique_ptr<BankAccount>& account) { return account->getId() == id; }
        );
        if (it == accountList.end()) throw std::runtime_error("account not found: " + std::to_string(id));

        return it->get();
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
            write(fd.get(), line.c_str(), line.size());
        }
    }

    void Bank::load(std::string& filename) {
        FDGuard fd(open(filename.c_str(), O_RDONLY));
        if (fd.get() == -1) throw std::runtime_error(std::string("open failed: ") + strerror(errno));

    }
}