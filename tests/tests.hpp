#ifndef _TEST_HPP_
#define _TEST_HPP_
#include <string>
#include <functional>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

void runTest(const std::string &testName, std::function<void()> testRunner);

#endif