#pragma once
#include "BankAccount.h"
#include <vector>
#include <memory>
#include <string>

namespace bank {

    class Bank {
        std::vector<std::unique_ptr<BankAccount>> accountList;

    public:
        int addAccount(const BankAccount& account);
        BankAccount* findAccount(int id);
        std::vector<std::string> printAllAccount() const;
        void save(const std::string& filename);
        void load(const std::string& filename);
    };
}