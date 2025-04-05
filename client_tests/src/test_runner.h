/**
 * Simple test framework for C++.
 * This framework allows you to define test cases and check conditions using EXPECT macros.
 * It provides a way to register tests and run them, reporting any failures.
 * Two keyword: TEST and EXPECT
 * TEST: Used to define a test case. It registers the test case with category and name.
 * EXPECT_EQ: Used to check a condition within a test case. If the condition is false, it reports a failure.
 */

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Test case structure
struct TestCase {
    std::string category;
    std::string name;
    std::function<void()> test_func;
};

// Function to access test registry
std::vector<TestCase>& GetTests();

// Test registration function
void RegisterTest(const std::string& category, const std::string& name, std::function<void()> func);

// TEST macro
#define TEST(category, name)                                          \
    void test_##category##_##name();                                  \
    struct TestRegister_##category##_##name {                         \
        TestRegister_##category##_##name() {                          \
            RegisterTest(#category, #name, test_##category##_##name); \
        }                                                             \
    } register_##category##_##name;                                   \
    void test_##category##_##name()

// EXPECT_EQ macro
#define EXPECT_EQ(condition)                                                                              \
    do {                                                                                                  \
        if (!(condition)) {                                                                               \
            std::cout << "FAILED: " << __FILE__ << ":" << __LINE__ << " EXPECT(" #condition ") failed\n"; \
            throw std::runtime_error("Test failed");                                                      \
        }                                                                                                 \
    } while (0)
