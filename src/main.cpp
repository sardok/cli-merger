#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <chrono>
#include <exception>
#include "message_processor.hpp"
#include "packet.hpp"
#include "rate_limiter.hpp"
#include "rate_limiter_config.hpp"
#include "parser.hpp"
#include "error.hpp"

void printHelp()
{
    std::cout << "Arguments as follows:" << std::endl;
    std::cout << "-i <filename>\t: specify input file (required, can be provided multiple times)" << std::endl;
    std::cout << "-o <filename>\t: specify out file (default: out_file.txt)" << std::endl; 
    std::cout << "-r <number>\t: specify request rate (default: 100 per file)" << std::endl;
    std::cout << "-t <number>: time in seconds for request rate (-r 10 -t 5 means, max 10 request in 5 secs. default: 1 sec.)" << std::endl;
    std::cout << "-h\t\t: prints this message." << std::endl;
}

using rate_limiter_t = RateLimiter<Packet>;
using mp_t = MessageProcessor<Packet>;

void processFile(const std::string &filename, std::shared_ptr<rate_limiter_t> rl)
{
    Parser parser{filename};
    int maxRetryForPacketProcess = 100;
    while (true)
    {
        auto packet = parser.parse();
        if (packet.has_value())
        {
            int retried = 0;
            while (rl->process(packet.value()) != ProcessResult::Ok)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                if (++retried == maxRetryForPacketProcess)
                    throw std::runtime_error("Max retry count for the packet process is reached");
            }
        }
        else break;
    }
    std::cout << "Processing " << filename << " is completed." << std::endl;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv+1, argv+argc);
    std::vector<std::string> inputs;
    std::string output{"out_file.txt"};
    RateLimiterConfig cfg{100, TimeFrame{1}};
    for (auto i = args.begin(); i != args.end(); i++)
    {
        if (*i == "-i")
        {
            inputs.push_back(*++i);
        }
        else if (*i == "-o")
        {
            output = *++i;
        }
        else if (*i == "-h")
        {
            printHelp();
            return 0;
        }
        else if (*i == "-r")
        {
            cfg.RequestLimit = std::stoi(*++i);
        }
        else if (*i == "-t")
        {
            cfg.TimeFrame.Seconds = std::stoi(*++i);
        }
        else
        {
            std::cout << "Unrecognized argument: " << *i << std::endl;
            return -1;
        }
    }
    if (inputs.size() == 0)
    {
        std::cout << "At least one input file is required" << std::endl;
        return -1;
    }

    std::vector<std::future<void>> futures;
    auto mp = std::make_shared<mp_t>();
    auto rl = std::make_shared<rate_limiter_t>(cfg, mp);
    for (const auto &filename: inputs)
    {
        auto future = std::async(std::launch::async, processFile, filename, rl);
        futures.push_back(std::move(future));
    }

    for (const auto &future : futures)
    {
        future.wait();
    }
    
    std::ofstream outfile(output);
    mp->flush(outfile);
    std::cout << "Output written to " << output << std::endl;
    return 0;
}