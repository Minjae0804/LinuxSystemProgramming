#pragma once
#include <memory>

namespace bank {

	int BankAccount::nextId = 0;

	bool BankAccount::withdrawal(int money) {
		int total = money + getFee();
		if (total > this->money) return false;
		this->money -= total;

		return true;
	}

    std::string BankAccount::printAccountInfo() const {
        std::string info;
        info += "owner name: " + ownerName + "\nmoney: " + std::to_string(this->money);

        return info;
    }

    void BankAccount::swap(BankAccount& lhs, BankAccount& rhs) noexcept {
        using std::swap;
        swap(lhs.money, rhs.money);
        swap(lhs.ownerName, rhs.ownerName);
    }

	bool transfer(BankAccount& src, BankAccount& dst, int money) {
		if (money > src.getBalance()) return false;

		std::unique_ptr<BankAccount> srcTemp(src.clone());
		std::unique_ptr<BankAccount> dstTemp(dst.clone());

		srcTemp->withdrawal(money);
		dstTemp->deposit(money);

		BankAccount::swap(src, *srcTemp);
		BankAccount::swap(dst, *dstTemp);

		return true;
	}
}