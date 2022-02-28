#include <iostream>
#include <memory>
#include "rate_limiter.hpp"
#include "packet.hpp"
#include "message_processor.hpp"

template <typename T>
RateLimiter<T>::RateLimiter(RateLimiterConfig config) 
    : config{std::move(config)}
{
    mp = std::make_shared<MessageProcessor<T>>();
}

template <typename T>
RateLimiter<T>::RateLimiter(RateLimiterConfig config, mp_ptr_t mp)
    : config{std::move(config)}, mp{std::move(mp)}
{

}

template <typename T>
ProcessResult RateLimiter<T>::process(T item)
{
    std::lock_guard<std::mutex> guard(mutex);
    auto now = std::time(nullptr);
    elicitExpired(now);

    if (logs.size() >= config.RequestLimit)
        return ProcessResult::TooMany;

    logs.push(now);
    mp->push(std::move(item));
    return ProcessResult::Ok;
}

template <typename T>
void RateLimiter<T>::elicitExpired(std::time_t &now)
{
    std::time_t lowerBound = now - config.TimeFrame.Seconds;
    while (logs.size() > 0 && logs.front() <= lowerBound)
    {
        logs.pop();
    }
}

template RateLimiter<int>::RateLimiter(RateLimiterConfig);
template RateLimiter<int>::RateLimiter(RateLimiterConfig, std::shared_ptr<MessageProcessor<int>>);
template ProcessResult RateLimiter<int>::process(int);
template RateLimiter<Packet>::RateLimiter(RateLimiterConfig);
template RateLimiter<Packet>::RateLimiter(RateLimiterConfig, std::shared_ptr<MessageProcessor<Packet>>);
template ProcessResult RateLimiter<Packet>::process(Packet);