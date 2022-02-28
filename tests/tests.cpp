#include <iostream>
#include "tests.hpp"

void runRateLimiterTests();
void runMessageProcessorTests();

void runTest(const std::string &testName, std::function<void()> testRunner)
{
    std::cout << testName << " tests...";
    testRunner();
    std::cout << "DONE." << std::endl;
}

int main(int argc, char *argv[])
{
    runRateLimiterTests();
    runMessageProcessorTests();
    return 0;
}