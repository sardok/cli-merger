#include <iostream>
#include <sstream>
#include <exception>
#include "packet.hpp"
#include "error.hpp"

std::ostream &operator<<(std::ostream &os, const Packet &packet)
{
    os << packet.MsgType;
    os << packet.SeqNumber;
    os << packet.Side;
    os << packet.Ticker.size();
    os << packet.Ticker;
    os << packet.Price;
    return os;
}

double readDouble(std::istream &is)
{
    std::stringstream ss;
    while (std::isdigit(is.peek()))
    {
        ss << (char)is.get();
    }
    if (is.peek() == '.')
    {
        ss << (char)is.get();
        while (std::isdigit(is.peek()))
        {
            auto chr = (char)is.get();
            ss << chr;
        }
    }
    auto s = ss.str();
    if (s.back() == '.')
        throw Error::MalformedInput;

    try
    {
        return std::stod(s);
    }
    catch (std::invalid_argument &)
    {
        throw Error::MalformedInput;
    }
}

std::string readTicker(std::istream &is)
{
    std::stringstream ss;
    while (std::isalpha(is.peek()))
        ss << (char)is.get();
    return ss.str();
}

std::istream &operator>>(std::istream &is, Packet &packet)
{
    unsigned long size;
    std::string ticker;

    is >> packet.MsgType;
    is >> packet.SeqNumber;
    is >> packet.Side;
    is >> packet.Size;
    packet.Ticker = readTicker(is); 
    packet.Price = readDouble(is);
    return is;
}

bool operator==(const Packet &lhs, const Packet &rhs)
{
    return lhs.SeqNumber == rhs.SeqNumber;
}

bool operator<(const Packet &lhs, const Packet &rhs)
{
    return lhs.SeqNumber < rhs.SeqNumber;
}

bool operator>(const Packet &lhs, const Packet &rhs)
{
    return lhs.SeqNumber > rhs.SeqNumber;
}