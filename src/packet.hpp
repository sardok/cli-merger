#ifndef _PACKET_HPP_
#define _PACKET_HPP_
#include <string>

struct Packet
{
    char MsgType;
    unsigned long SeqNumber;
    char Side;
    unsigned long Size;
    std::string Ticker;
    double Price;
};

std::ostream &operator<<(std::ostream &os, const Packet &packet);
std::istream &operator>>(std::istream &is, Packet &packet);
bool operator==(const Packet &, const Packet &);
bool operator<(const Packet &, const Packet &);
bool operator>(const Packet &, const Packet &);
#endif