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
		BankAccount(const std::string& ownerName, int money = 0) 
			: id(nextId++), ownerName(ownerName), money(money) {}; 
		BankAccount(int id, const std::string& ownerName, int money) 
			: id(id), ownerName(ownerName), money(money) { if (id >= nextId) nextId = id + 1; }
		virtual ~BankAccount() {}; 

		int getId() const { return id; }
		int getBalance() const { return money; }
        const std::string& getOwnerName() const { return ownerName; }
		virtual std::string getType() const { return "BankAccount"; }

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
		SavingsAccount(int id, const std::string& ownerName, int money) : BankAccount(id, ownerName, money) {}
		BankAccount* clone() const override { return new SavingsAccount(*this); }
		std::string getType() const override { return "SavingsAccount"; }
	};

	class CheckingAccount : public BankAccount {
	public:
		CheckingAccount(const std::string& ownerName, int money = 0) : BankAccount(ownerName, money) {};
		CheckingAccount(int id, const std::string& ownerName, int money) : BankAccount(id, ownerName, money) {}
		BankAccount* clone() const override { return new CheckingAccount(*this); }
		std::string getType() const override { return "CheckingAccount"; }
	protected:
		int getFee() const override { return 100; }
	};

}