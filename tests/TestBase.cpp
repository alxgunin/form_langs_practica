#include "TestBase.hpp"
#include "slib.hpp"

TEST(TestBase, OperationsTest1) {
    TaskInput input;
    input.k = 1;
    input.x = 'a';
    input.regexp = "ab+";
    EXPECT_EQ(GetAnswer(input).answer, "1");
}

TEST(TestBase, OperationsTest2) {
    TaskInput input;
    input.k = 1;
    input.x = 'b';
    input.regexp = "ab.";
    EXPECT_EQ(GetAnswer(input).answer, "2");
}

TEST(TestBase, OperationsTest3) {
    TaskInput input;
    input.k = 3;
    input.x = 'a';
    input.regexp = "a*";
    EXPECT_EQ(GetAnswer(input).answer, "3");
}
