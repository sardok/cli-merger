#ifndef _MESSAGE_PROCESSOR_HPP_
#define _MESSAGE_PROCESSOR_HPP_
#include <string>
#include <vector>
#include <ostream>
#include <functional>
#include "packet.hpp"

template <typename T>
class MessageProcessor
{
public:
    void push(T);
    T pop();
    size_t size() const noexcept;
    void flush(std::ostream &);
    void flush(std::vector<T> &);

private:
    void siftDown(int, int);
    void siftUp(int);
    void iter(std::function<void(T &)>);
    std::vector<T> buffer;
};
#endif