#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#include "utils.h"
#include "logger.h"

#define MIN_PACKET_SIZE 6
#define MAX_PARAM_SIZE 4
#define MAX_PACKET_SIZE MIN_PACKET_SIZE + MAX_PARAM_SIZE


template<int SIZE>
class Packet_
{
public:
    Packet_() = default;
    Packet_(const uint8_t* data, uint8_t length);
    Packet_(const Packet_<SIZE>& Packet);

    void append(uint8_t byte);
    const uint8_t* data() const;
    uint8_t size() const;
    void clear();
    const char* c_str();

private:
    uint8_t m_packet[SIZE];
    uint8_t m_length{0};
    char m_string[SIZE * 3];
};

template<int SIZE>
Packet_<SIZE>::Packet_(const uint8_t* data, uint8_t length)
    : m_length(length)
{
    memcpy(m_packet, data, length);
}

template<int SIZE>
Packet_<SIZE>::Packet_(const Packet_<SIZE>& other)
{
    memcpy(this->m_packet, other.m_packet, other.m_size);
    this->m_size = other.m_size();
}

template<int SIZE>
void Packet_<SIZE>::append(uint8_t byte)
{
    if (m_length < SIZE)
    {
        m_packet[m_length++] = byte;
    }
}

template<int SIZE>
const uint8_t* Packet_<SIZE>::data() const
{
    return m_packet;
}

template<int SIZE>
uint8_t Packet_<SIZE>::size() const
{
    return m_length;
}

template<int SIZE>
void Packet_<SIZE>::clear()
{
    m_length = 0;
}

template<int SIZE>
const char* Packet_<SIZE>::c_str()
{   
    arrayToString(m_packet, m_length, m_string);
    return m_string;
}

using Packet = Packet_<MAX_PACKET_SIZE>;

#endif
