#include "TestBase.hpp"
#include "slib.hpp"

TEST_F(TestBase, OperationsTest1) {
    TaskInput input;
    input.k = 1;
    input.x = 'a';
    input.regexp = "ab+";
    ASSERT_EQ(GetAnswer(input).answer, "1");
}

TEST_F(TestBase, OperationsTest2) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "ab.";
    ASSERT_EQ(GetAnswer(input).answer, "2");
}

TEST_F(TestBase, OperationsTest3) {
    TaskInput input;
    input.k = 3;
    input.x = 'a';
    input.regexp = "a*";
    ASSERT_EQ(GetAnswer(input).answer, "3");
}

TEST_F(TestBase, NoSuchString) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "a*";
    ASSERT_EQ(GetAnswer(input).answer, "INF");
}

TEST_F(TestBase, IncorrectInput1) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "a+";
    ASSERT_ANY_THROW(GetAnswer(input));
}

TEST_F(TestBase, IncorrectInput2) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "a.";
    ASSERT_ANY_THROW(GetAnswer(input));
}

TEST_F(TestBase, IncorrectInput3) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "*";
    ASSERT_ANY_THROW(GetAnswer(input));
}

TEST_F(TestBase, IncorrectInput4) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "abcd+";
    ASSERT_ANY_THROW(GetAnswer(input));
}
