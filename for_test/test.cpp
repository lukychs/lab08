#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include <stdexcept>

class MAccount : public Account {
 private:
  int id;
  int balance;
 public:
  MAccount(int id, int balance) : Account(id, balance) {}
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};
class MTransaction : public Transaction {
 public:
  MTransaction() : Transaction() {}
  MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};



TEST(account1, te1) {
  Account uno(1, 100);
  EXPECT_EQ(100, uno.GetBalance());
  uno.Lock();
  uno.ChangeBalance(2000);
  uno.Unlock();
  EXPECT_EQ(2100, uno.GetBalance());
  try {
    uno.ChangeBalance(1000000);
  }
  catch (std::runtime_error& el) {}
  EXPECT_EQ(2100, uno.GetBalance());
}

TEST(transaction1, te1) {
  Account uno(1, 10000);
  Account dos(2, 10000);
  Transaction tres;
  Transaction quatro; quatro.set_fee(500);
  try {tres.Make(uno, dos, 100);}
  catch (std::logic_error& el2) {}
  try {tres.Make(uno, dos, -100);}
  catch (std::invalid_argument& q) {}
  try {tres.Make(uno, dos, 0);}
  catch (std::logic_error& q) {}
  EXPECT_EQ(false, quatro.Make(uno, dos, 200));
  tres.Make(uno, dos, 1999);
  EXPECT_EQ(uno.GetBalance(), 10000); EXPECT_EQ(dos.GetBalance(), 9998);
}
using ::testing::AtLeast;
TEST(transaction2, te2) {
  Account uno(1, 10000);
  Account dos(2, 10000);
  MTransaction tres;
  EXPECT_CALL(tres, SaveToDataBase(uno, dos, 1999)).Times(AtLeast(1));
  tres.Make(uno, dos, 1999);
}

TEST(account1, te2) {
  MAccount uno(1, 1000);
  EXPECT_CALL(uno, GetBalance()).Times(AtLeast(1));
  uno.GetBalance();
  EXPECT_CALL(uno, Lock()).Times(AtLeast(1));
  uno.Lock();
  EXPECT_CALL(uno, ChangeBalance(1)).Times(AtLeast(1));
  uno.ChangeBalance(1);
  EXPECT_CALL(uno, Unlock()).Times(AtLeast(1));
  uno.Unlock();
}

