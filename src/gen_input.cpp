#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include "packet.hpp"

char genRandomChar()
{
    return (char)('A' + random() % 10);
}

std::string genRandomTicker()
{
    std::stringstream ss;
    int len = 3 + random() % 3;
    while (len-- > 0)
        ss << genRandomChar();
    return ss.str();
}

unsigned long genRandomNumber(unsigned long max)
{
    return random() % max;
}

Packet genRandomPacket()
{
    Packet packet;
    packet.MsgType = genRandomChar();
    packet.SeqNumber = genRandomNumber(100000);
    packet.Side = genRandomChar();
    packet.Size = genRandomNumber(50000);
    packet.Ticker = genRandomTicker();
    packet.Price = (double)genRandomNumber(100000);
    return std::move(packet);
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 5; i++)
    {
        auto filename = "input_file" + std::to_string(i) + ".txt";
        std::ofstream os(filename, std::ios::binary);
        for (int j = 0; j < genRandomNumber(10000); j++)
            os << genRandomPacket();
        os.close();
        std::cout << "Generated " << filename << std::endl;
    }
    return 0;
}