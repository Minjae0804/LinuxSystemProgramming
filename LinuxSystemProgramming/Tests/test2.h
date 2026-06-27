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

void test2() {
	bank::BankAccount a(-1, "Sally", 1000);
	bank::BankAccount b(-1, "Minjae", 2000);
	bank::BankAccount c(100, "JeongHyeon", 3000);  // 복원, id=100으로 명시
	bank::BankAccount d(-1, "NewGuy", 500);  // 그 다음 자동발급은 101이어야 의도에 맞음

	std::cout << a.getId() << " " << b.getId() << " " << c.getId() << " " << d.getId() << std::endl;
	
}