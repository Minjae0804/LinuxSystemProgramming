#include "Bank/Bank.h"
#include "Bank/BankImpl.h"
#include "Bank/BankAccount.h"
#include "Bank/BankAccountImpl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <memory>
#include <chrono>
#include <sstream>

#include "Tests/test1.h"

int main() {
	std::unique_ptr<bank::Bank> myBank = std::make_unique<bank::Bank>();

	while (true) {
		std::cout << "> ";
		std::string line;
		std::getline(std::cin, line);
		if (line.empty()) continue;
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> tokens;
		while (ss >> token) tokens.push_back(token);
		

		const std::string& command = tokens[0];

		if (command == "create") {
			if (tokens.size() < 4) {
				std::cout << "How to use: create <type> <name> <money>" << std::endl;
				std::cout << "type: checking, savings" << std::endl;
				continue;
			}

			int id = -1;
			try {
				id = myBank->addAccount(*bank::createAccount(bank::AccountData{ tokens[1], -1, tokens[2], stoi(tokens[3]) }));
			} catch(const std::exception& e){
				std::cout << "오류: " << e.what() << std::endl;
				continue;
			}

			std::cout << "created new account. id: " << id << std::endl;
			continue;

		} else if (command == "deposit") {
			if (tokens.size() < 3) {
				std::cout << "How to use: deposit <id> <ammount>" << std::endl;
				continue;
			}

			int balance = -1;
			try {
				bank::BankAccount* acc(myBank->findAccount(stoi(tokens[1])));
				acc->deposit(stoi(tokens[2]));
				balance = acc->getBalance();
			} catch (const std::exception& e) {
				std::cout << "오류: " << e.what() << std::endl;
				continue;
			}

			std::cout << "deposit complete. balance: " << balance << std::endl;
			continue;
		} else if (command == "withdraw") {
			if (tokens.size() < 3) {
				std::cout << "How to use: withdraw <id> <amount>" << std::endl;
				continue;
			}

			int balance = -1;
			try {
				bank::BankAccount* acc(myBank->findAccount(stoi(tokens[1])));
				acc->withdrawal(stoi(tokens[2]));
				balance = acc->getBalance();
			} catch (const std::exception& e) {
				std::cout << "오류: " << e.what() << std::endl;
				continue;
			}

			std::cout << "withdraw complete. balance: " << balance << std::endl;
			continue;
		}
		else if (command == "transfer") { // transfer <srcId> <dstId> <amount>
			if (tokens.size() < 4) {
				std::cout << "How to use: transfer <fromId> <toId> <amount>" << std::endl;
				continue;
			}

			int srcBal = -1, dstBal = -1;
			try {
				bank::BankAccount* srcAcc(myBank->findAccount(stoi(tokens[1])));
				bank::BankAccount* dstAcc(myBank->findAccount(stoi(tokens[2])));
				bank::transfer(*srcAcc, *dstAcc, stoi(tokens[3]));
				srcBal = srcAcc->getBalance();
				dstBal = dstAcc->getBalance();
			} catch (const std::exception e) {
				std::cout << "오류: " << e.what() << std::endl;
				continue;
			}

			std::cout << "transfer complete.\nsrc's Balance: " << srcBal << "\ndst's Balance: " << dstBal << std::endl;
			continue;
		} 
		else {
			std::cout << "please enter the correct command." << std::endl;
			continue;
		}

		if (command == "list") {
			auto list = myBank->printAllAccount();
			if (list.empty()) {
				std::cout << "account does not exist." << std::endl;
			} else {
				for (const auto& acc : list)
					std::cout << acc << std::endl;
			}

			continue;
		}

		if (command == "exit") break;
	}

}
