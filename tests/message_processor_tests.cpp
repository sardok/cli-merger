#include <iostream>
#include <sstream>
#include "../src/message_processor.hpp"
#include "../src/packet.hpp"
#include "tests.hpp"

static void testDups()
{
    MessageProcessor<int> mp;
    mp.push(1);
    mp.push(2);
    mp.push(3);
    mp.push(1);
    mp.push(3);
    std::stringstream ss;
    mp.flush(ss);
    assertm(ss.str() == "123", "Dups test failed");
}

static void testOutOfOrder()
{
    MessageProcessor<int> mp;
    mp.push(4);
    mp.push(1);
    mp.push(3);
    mp.push(10);
    mp.push(0);
    mp.push(2);
    std::stringstream ss;
    mp.flush(ss);
    assertm(ss.str() == "0123410", ("Unexpected order: " + ss.str()));
}

static void testOrderWithPackets()
{
    MessageProcessor<Packet> mp;
    Packet packet1;
    Packet packet2;
    Packet packet3;
    packet1.SeqNumber = 10;
    packet2.SeqNumber = 5;
    packet3.SeqNumber = 0;
    mp.push(packet1);
    mp.push(packet2);
    mp.push(packet3);
    std::vector<Packet> vec;
    mp.flush(vec);
    assertm(vec[0] == packet3, "Unexpected packet seq for pos 0");
    assertm(vec[1] == packet2, "Unexpected packet seq for pos 1");
    assertm(vec[2] == packet1, "Unexpected packet seq for pos 2");
}

void runMessageProcessorTests()
{
    std::cout << "Executing message processor tests." << std::endl;
    runTest("duplicate", testDups);
    runTest("out-of-order", testOutOfOrder);
    runTest("test-order-with-packets", testOrderWithPackets);
    std::cout << "Done." << std::endl;
}