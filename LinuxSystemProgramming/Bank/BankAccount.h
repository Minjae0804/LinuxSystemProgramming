#pragma once
#include <string>
#include <iostream>

namespace bank {

	class BankAccount {
		static int nextId;
		int id;
		std::string ownerName;
		int money;

	public:
		BankAccount(const std::string& ownerName, int money = 0) : id(nextId++), ownerName(ownerName), money(money) {};
		virtual ~BankAccount() {}; 

		int getId() const { return id; }
		int getBalance() const { return money; }
        const std::string& getOwnerName() const { return ownerName; }

		void deposit(int money) { this->money += money; }
		virtual bool withdrawal(int money);

        std::string printAccountInfo() const;

        static void swap(BankAccount& lhs, BankAccount& rhs) noexcept;

		virtual BankAccount* clone() const { return new BankAccount(*this); }

	protected:
		virtual int getFee() const { return 0; }
	};

	class SavingsAccount : public BankAccount {
	public:
		SavingsAccount(const std::string& ownerName, int money = 0) : BankAccount(ownerName, money) {}; 
		BankAccount* clone() const override { return new SavingsAccount(*this); }
	};

	class CheckingAccount : public BankAccount {
	public:
		CheckingAccount(const std::string& ownerName, int money = 0) : BankAccount(ownerName, money) {};
		BankAccount* clone() const override { return new CheckingAccount(*this); }
	protected:
		int getFee() const override { return 100; }
	};

}