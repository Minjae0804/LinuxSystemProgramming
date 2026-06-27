#include "../Bank/Bank.h"
#include "../Bank/BankImpl.h"
#include "../Bank/BankAccount.h"
#include "../Bank/BankAccountImpl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <memory>
#include <chrono>

void test1() {
	std::unique_ptr<bank::Bank> myBank = std::make_unique<bank::Bank>();

	int sallyId = myBank->addAccount(bank::CheckingAccount("Sally", 50000));
	int minjaeId = myBank->addAccount(bank::SavingsAccount("Minjae", 0));
	int jeonghyeonId = myBank->addAccount(bank::CheckingAccount("JeongHyeon", 6000));

	std::cout << "#accountList_prev:" << std::endl;
	for (auto accountInfo : myBank->printAllAccount())
		std::cout << accountInfo << std::endl;

	std::cout << "transfer "
		<< (bank::transfer(*myBank->findAccount(sallyId), *myBank->findAccount(minjaeId), 20000) ? "success" : "fail")
		<< std::endl;

	std::cout << "\n\n#accountList_next:" << std::endl;
	for (auto accountInfo : myBank->printAllAccount())
		std::cout << accountInfo << std::endl;


	myBank->save("accounts.txt");

	auto newBank = std::make_unique<bank::Bank>();
	newBank->load("accounts.txt");

	std::cout << "\n\n#loaded from file:" << std::endl;
	for (const auto& info : newBank->printAllAccount())
		std::cout << info << std::endl;
}