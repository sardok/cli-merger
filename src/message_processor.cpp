#include "packet.hpp"
#include "message_processor.hpp"

template <typename T>
void MessageProcessor<T>::push(T item)
{
    buffer.push_back(std::move(item));
    siftDown(0, buffer.size()-1);
}

template <typename T>
T MessageProcessor<T>::pop()
{
    auto last = buffer.back();
    buffer.pop_back();
    if (buffer.size() > 0)
    {
        auto ret = buffer[0];
        buffer[0] = last;
        siftUp(0);
        return ret;
    }
    return last;
}

template <typename T>
size_t MessageProcessor<T>::size() const noexcept
{
    return buffer.size();
}

template <typename T>
void MessageProcessor<T>::siftDown(int startPos, int pos)
{
    auto newPacket = buffer[pos];
    while (pos > startPos)
    {
        auto parentPos = (pos - 1) / 2;
        auto parent = buffer[parentPos];
        if (newPacket < parent)
        {
            buffer[pos] = parent;
            pos = parentPos;
        }
        else break;
    }
    buffer[pos] = newPacket;
}

template <typename T>
void MessageProcessor<T>::siftUp(int pos)
{
    auto sinkingPacket = buffer[pos];
    int startpos = pos;
    int childpos = 2*pos + 1;
    while (childpos < buffer.size())
    {
        int rightpos = childpos + 1;
        if (rightpos < buffer.size() && buffer[childpos] > buffer[rightpos])
            childpos = rightpos;
        buffer[pos] = buffer[childpos]; 
        pos = childpos;
        childpos = 2 * pos + 1;
    }
    buffer[pos] = sinkingPacket;
    siftDown(startpos, pos);
}

template <typename T>
void MessageProcessor<T>::iter(std::function<void(T &)> cb)
{
    if (size() > 0)
    {
        auto last = pop();
        cb(last);
        while (size() > 0)
        {
            auto item = pop();
            if (item == last)
                continue;

            cb(item);
            last = item;
        }
    }
}

template <typename T>
void MessageProcessor<T>::flush(std::ostream &os)
{
    iter([&](T &item) {
        os << item;
    });
}

template <typename T>
void MessageProcessor<T>::flush(std::vector<T> &vec)
{
    iter([&](T &item) {
        vec.push_back(item);
    });
}

template void MessageProcessor<int>::push(int);
template void MessageProcessor<int>::flush(std::ostream &);
template void MessageProcessor<Packet>::push(Packet);
template void MessageProcessor<Packet>::flush(std::vector<Packet> &);
template void MessageProcessor<Packet>::flush(std::ostream &);