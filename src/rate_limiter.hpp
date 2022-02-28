#ifndef _RATE_LIMITER_HPP_
#define _RATE_LIMITER_HPP_
#include <queue>
#include <ctime>
#include <memory>
#include <mutex>
#include "rate_limiter_config.hpp"
#include "message_processor.hpp"

enum class ProcessResult
{
    Ok,
    TooMany,
};

template <typename T>
class RateLimiter
{
public:
    using mp_ptr_t = std::shared_ptr<MessageProcessor<T>>;
    explicit RateLimiter(RateLimiterConfig);
    explicit RateLimiter(RateLimiterConfig, mp_ptr_t);
    ProcessResult process(T);

private:
    void elicitExpired(std::time_t &);
    RateLimiterConfig config;
    std::queue<std::time_t> logs;
    mp_ptr_t mp;
    std::mutex mutex;
};
#endif