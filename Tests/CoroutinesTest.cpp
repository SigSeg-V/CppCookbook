//
// Created by charlie on 8/18/2024.
//
#include <array>
#include <gtest/gtest.h>
import Coroutines;

TEST(CoroutinesTest, IntGenerator)
{
    // build string of ints generated to compare
    struct TestCase
    {
        std::string expected;
        int start;
        int end;
    };
    const std::array Tests
    {
            TestCase{"12345", 1, 6},
    };

    for (const auto& Test : Tests)
    {
        std::string str = "";
        for (auto num : Coroutines::Counter(Test.start, Test.end))
        {
            str += std::to_string(num);
        }

        EXPECT_EQ(str, Test.expected)
        << "Generator did not supply correct values, got "
        << str
        << ", expected "
        << Test.expected;
    }

}