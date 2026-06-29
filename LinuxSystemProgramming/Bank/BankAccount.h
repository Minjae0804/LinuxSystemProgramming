#pragma once
#include <string>
#include <iostream>

namespace bank {

	// 계좌 클래스
	class BankAccount {
		static int nextId;
		int id;
		std::string ownerName;
		int money;

	public:
		BankAccount(const std::string& ownerName, int money = 0) 
			: id(nextId++), ownerName(ownerName), money(money) {}; 
		BankAccount(int id, const std::string& ownerName, int money) 
			: id(id == -1 ? nextId++ : id), ownerName(ownerName), money(money) { if (this->id >= nextId) nextId = id + 1; }
		virtual ~BankAccount() {}

		int getId() const { return id; }
		int getBalance() const { return money; }
        const std::string& getOwnerName() const { return ownerName; }
		virtual std::string getType() const { return "BankAccount"; }

		void setOwnerName(const std::string& newname) { this->ownerName = newname; }

		void deposit(int money) { this->money += money; }	// 입금
		virtual bool withdrawal(int money);					// 출금

        std::string printAccountInfo() const;				// 해당 계좌의 주인 및 잔액을 문자열로 반환

        static void swap(BankAccount& lhs, BankAccount& rhs) noexcept;

		virtual BankAccount* clone() const { return new BankAccount(*this); }

	protected:
		virtual int getFee() const { return 0; }			// 출금 수수료
	};

	// 저축계좌 클래스
	class SavingsAccount : public BankAccount {
	public:
		SavingsAccount(const std::string& ownerName, int money = 0) : BankAccount(ownerName, money) {};
		SavingsAccount(int id, const std::string& ownerName, int money) : BankAccount(id, ownerName, money) {}
		BankAccount* clone() const override { return new SavingsAccount(*this); }
		std::string getType() const override { return "SavingsAccount"; }
	};

	// 직불계좌 클래스
	class CheckingAccount : public BankAccount {
	public:
		CheckingAccount(const std::string& ownerName, int money = 0) : BankAccount(ownerName, money) {};
		CheckingAccount(int id, const std::string& ownerName, int money) : BankAccount(id, ownerName, money) {}
		BankAccount* clone() const override { return new CheckingAccount(*this); }
		std::string getType() const override { return "CheckingAccount"; }
	protected:
		int getFee() const override { return 100; }			// 출금 수수료
	};

}