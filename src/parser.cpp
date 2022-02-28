#include <string>
#include <fstream>
#include "parser.hpp"

Parser::Parser(const std::string &filename)
    : input_file(filename, std::ios::binary)
{
}

Parser::~Parser()
{
    input_file.close();
}

std::optional<Packet> Parser::parse()
{
    if (input_file.eof())
        return {};

    Packet packet;
    input_file >> packet;
    return packet;
}