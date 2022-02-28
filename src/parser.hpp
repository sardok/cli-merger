#ifndef _PARSER_HPP_
#define _PARSER_HPP_
#include <string>
#include <fstream>
#include <optional>
#include "packet.hpp"
#include "error.hpp"

class Parser
{
public:
    explicit Parser(const std::string &);
    ~Parser();
    std::optional<Packet> parse();

private:
    std::ifstream input_file;
};
#endif