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
#include <functional>
#include <algorithm>

#include "Tests/test1.h"

struct Command {
	std::string command;
	int requiredArgs;
	std::string usage;
	std::function<void(const std::vector<std::string>&)> action;
};

int main() {
	std::unique_ptr<bank::Bank> myBank = std::make_unique<bank::Bank>();

	std::vector<Command> commands = {
		{
			"create", 4, "create <type> <name> <money>", [&](const std::vector<std::string>& tokens) {
				int id = myBank->addAccount(*bank::createAccount(bank::AccountData{ tokens[1], -1, tokens[2], stoi(tokens[3]) }));
				std::cout << "created new account. id: " << id << std::endl;
			}
		}, {
			"deposit", 3, "deposit <id> <amount>", [&](const std::vector<std::string>& tokens) {
				bank::BankAccount* acc = myBank->findAccount(std::stoi(tokens[1]));
				acc->deposit(std::stoi(tokens[2]));
				std::cout << "deposit complete. balance: " << acc->getBalance() << std::endl;
			}
		}, {
			"withdraw", 3, "withdraw <id> <amount>", [&](const std::vector<std::string>& tokens) {
				bank::BankAccount* acc = myBank->findAccount(stoi(tokens[1]));
				bool isComplete = acc->withdrawal(stoi(tokens[2]));
				if (isComplete) std::cout << "withdraw complete. balance: " << acc->getBalance() << std::endl;
				else std::cout << "withdraw fail. Insufficient balance." << std::endl;
			}
		}, {
			"transfer", 4, "transfer <fromId> <toId> <amount>", [&](const std::vector<std::string>& tokens) {
				bank::BankAccount* srcAcc = myBank->findAccount(stoi(tokens[1]));
				bank::BankAccount* dstAcc = myBank->findAccount(stoi(tokens[2]));
				bool isComplete = bank::transfer(*srcAcc, *dstAcc, stoi(tokens[3]));
				if (isComplete) std::cout << "transfer complete.\nsrc's Balance: " << srcAcc->getBalance() 
					<< "\ndst's Balance: " << dstAcc->getBalance() << std::endl;
				else std::cout << "transfer fail. src's balance is insufficient." << std::endl;
			}
		}, {
			"balance", 2, "balance <id>", [&](const std::vector<std::string>& tokens) {
				bank::BankAccount* acc = myBank->findAccount(std::stoi(tokens[1]));
				std::cout << acc->printAccountInfo() << std::endl;
			}
		}, {
			"list", 1, "", [&](const std::vector<std::string>& tokens) {
				auto list = myBank->printAllAccount();
				if (list.empty()) std::cout << "account does not exist." << std::endl;
				else for (const auto& acc : list) std::cout << acc << std::endl;
			}
		}, {
			"save", 2, "save <filename>", [&](const std::vector<std::string>& tokens) {
				myBank->save(tokens[1]);
				std::cout << "save complete." << std::endl;
			}
		}, {
			"load", 2, "load <filename>", [&](const std::vector<std::string>& tokens) {
				myBank->load(tokens[1]);
				std::cout << "load complete." << std::endl;
			}
		}, {
			"exit", 1, "", [&](const std::vector<std::string>& tokens) {}
		}, {
			"help", 1, "", [&](const std::vector<std::string>& tokens) {}
		}
	};

	while (true) {
		std::cout << "> ";
		std::string line;
		std::getline(std::cin, line);
		if (line.empty()) continue;
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> tokens;
		while (ss >> token) tokens.push_back(token);
		

		if (tokens.empty()) continue;
		const std::string& command = tokens[0];

		auto it = std::find_if(commands.begin(), commands.end(), [&](const Command& com) { return command == com.command; });

		if (it == commands.end())	{ std::cout << "please enter the correct command." << std::endl; continue; }
		if (it->command == "exit")	{ std::cout << "exit." << std::endl; break; }
		if (it->command == "help")	{
			for (const Command& com : commands) {
				if(com.usage.empty())	std::cout << com.command	<< std::endl; 
				else					std::cout << com.usage		<< std::endl;

				continue;
			}
		}

		if (static_cast<int>(tokens.size()) < it->requiredArgs) {
			std::cout << "How to use: " << it->usage << std::endl;
			continue;
		}
		
		try {
			it->action(tokens);
		} catch (const std::exception& e) {
			std::cout << "오류: " << e.what() << std::endl;
			continue;
		}
	}

}
