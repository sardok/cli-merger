#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <memory>
#include "../src/packet.hpp"
#include "../src/rate_limiter.hpp"
#include "../src/message_processor.hpp"
#include "tests.hpp"

static void testTimeFrame1()
{
    RateLimiterConfig config{1, TimeFrame{1}};
    RateLimiter<int> rl{config};
    assertm(rl.process(1) == ProcessResult::Ok, "Packet should be accepted");
    assertm(rl.process(2) == ProcessResult::TooMany, "Packet should be denied");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    assertm(rl.process(3) == ProcessResult::Ok, "Packet should be accepted");
}

static void testLimit2()
{
    RateLimiterConfig config{2, TimeFrame{60}};
    RateLimiter<int> rl{config};
    assertm(rl.process(1) == ProcessResult::Ok, "Packet should be accepted");
    assertm(rl.process(2) == ProcessResult::Ok, "Packet should be accepted");
    assertm(rl.process(3) == ProcessResult::TooMany, "Packet should be denied");
}

static void testLimit1()
{
    RateLimiterConfig config{1, TimeFrame{60}};
    RateLimiter<int> rl{config};
    assertm(rl.process(1) == ProcessResult::Ok, "Packet should be accepted");
    assertm(rl.process(2) == ProcessResult::TooMany, "Packet should be denied");
}

static void testMPCall()
{
    auto mp = std::make_shared<MessageProcessor<int>>();
    RateLimiterConfig config{1, TimeFrame{10}};
    RateLimiter rl{config, mp};
    assertm(rl.process(1) == ProcessResult::Ok, "Packet should be accepted");
    assertm(mp->size() == 1, "Unexpected message processor size");
    assertm(rl.process(2) == ProcessResult::TooMany, "Packet should be denied");
    assertm(mp->size() == 1, "Unexpected message processor size");
}

void runRateLimiterTests()
{
    std::cout << "Executing rate limiter tests." << std::endl;
    runTest("limit-1", testLimit1);
    runTest("limit-2", testLimit2);
    runTest("time-frame-1", testTimeFrame1);
    runTest("test-message-processor-call", testMPCall);
    std::cout << "Done." << std::endl;
}