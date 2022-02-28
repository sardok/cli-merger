#ifndef _RATE_LIMITER_CONFIG_HPP_
#define _RATE_LIMITER_CONFIG_HPP_
struct TimeFrame
{
    TimeFrame(int seconds) : Seconds{seconds} {}
    int Seconds;
};

struct RateLimiterConfig
{
    RateLimiterConfig(int requestLimit, TimeFrame timeFrame)
        : RequestLimit{requestLimit}, TimeFrame{timeFrame}
    {
    }
    int RequestLimit;
    TimeFrame TimeFrame;
};
#endif