#pragma once
#include "BankAccount.h"
#include <vector>
#include <memory>
#include <string>

namespace bank {

    // 은행 클래스
    class Bank {
        std::vector<std::unique_ptr<BankAccount>> accountList;
        std::vector<std::unique_ptr<BankAccount>>::iterator findIterator(int id);
    public:
        int addAccount(const BankAccount& account);                 // 계좌를 생성한 후 accountList에 저장
        BankAccount* findAccount(int id);                           // accountList에서 계좌정보 조회
        bool deleteAccount(int id);                                 // accountList에서 계좌를 삭제
        bool updateAccountOwnername(int id, const std::string& newname);    // 계좌의 소유자명 변경
            
        std::vector<std::string> printAllAccount() const;           // accountList의 모든 계좌정보를 문자열로 출력

        void save(const std::string& filename);                     // accountList에 저장된 계좌정보를 파일로 저장
        void load(const std::string& filename);                     // 파일을 읽고 계좌로 변환한 후 accountList에 저장
    };
}