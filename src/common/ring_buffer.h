#ifndef RING_BUFFER_H
#define RING_BUFFER_H

template<typename T, int SIZE>
class RingBuffer
{
public:
    bool push_back(const T& item);
    T pop_front();
    bool isFull();
    bool isEmpty();

private:
    T buffer[SIZE];
    T* head{buffer};
    T* tail{buffer};
    size_t count{0};
};

template<typename T, int SIZE>
bool RingBuffer<T, SIZE>::push_back(const T& item)
{
    if (++tail == buffer + SIZE) {
        tail = buffer;
    }
    *tail = item;
    if (count == SIZE) {
        if (++head == buffer + SIZE) {
            head = buffer;
        }
        return false;
    } else {
        if (count++ == 0) {
            head = tail;
        }
        return true;
    }
}

template<typename T, int SIZE>
T RingBuffer<T, SIZE>::pop_front()
{
    if (count == 0) 
    {
        return *tail;
    }

    T result = *tail--;
    if (tail < buffer) 
    {
        tail = buffer + SIZE - 1;
    }
    count--;
    return result;
}

template<typename T, int SIZE>
bool RingBuffer<T, SIZE>::isFull()
{
    return count == SIZE;
}

template<typename T, int SIZE>
bool RingBuffer<T, SIZE>::isEmpty()
{
    return count == 0;
}

#endif // RING_BUFFER_H