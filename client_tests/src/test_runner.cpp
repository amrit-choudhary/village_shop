#include "test_runner.h"

#include <iostream>

// Define the test registry
std::vector<TestCase>& GetTests() {
    static std::vector<TestCase> tests;
    return tests;
}

// Implement test registration
void RegisterTest(const std::string& category, const std::string& name, std::function<void()> func) {
    GetTests().push_back({category, name, func});
}

// Main test runner
int main(int argc, char** argv) {
    int passed = 0;
    int total = GetTests().size();

    std::cout << "Running " << total << " tests...\n";

    for (const auto& test : GetTests()) {
        try {
            test.test_func();
            std::cout << "Result: PASSED\t\t" << "Category: " << test.category << "\t\tName: " << test.name << "\n";
            passed++;
        } catch (const std::exception& e) {
            std::cout << "Result: FAILED\t\t" << "Category: " << test.category << "\t\tName: " << test.name << "\n";
        }
    }

    std::cout << "\nTest Summary: " << passed << "/" << total << " passed\n";
    return passed == total ? 0 : 1;
}