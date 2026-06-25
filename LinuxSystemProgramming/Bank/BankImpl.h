#pragma once
#include "Bank.h"
#include <algorithm>
#include <string>

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
}