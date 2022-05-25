#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>

enum class UnitsValue : uint8_t
{
    mm         = 0x00,
    inch       = 0x01,
    Unkown     = 0xFF
};

enum class UserLimitSetValue : uint8_t
{
    None       = 0x00,
    Max        = 0x01,
    Min        = 0x10,
    Both       = 0x11,
    Unkown     = 0xFF
};

enum class userLimitReachedValue : uint8_t
{
    MaxReached = 0x01,
    MinReached = 0x02,
    Unkown     = 0xFF
};

enum class TouchModeValue : uint8_t
{
    Single     = 0x00,
    Continuous = 0x01,
    Unkown     = 0xFF
};

enum class KillModeValue : uint8_t
{
    Kill       = 0x00,
    LetLive    = 0x01,
    Unkown     = 0xFF
};

enum class SensitivityValue : uint8_t
{
    High       = 0x01,
    Medium     = 0x02,
    Low        = 0x03,
    Unkown     = 0xFF
};

String char2hex(uint8_t c);

String array2String(uint8_t *packet, size_t packetSize);

std::string valToString(UnitsValue units);
std::string valToString(UserLimitSetValue limits);
std::string valToString(userLimitReachedValue limits);
std::string valToString(TouchModeValue mode);
std::string valToString(KillModeValue mode);
std::string valToString(SensitivityValue sens);

template<class T>
T getByteType(uint8_t value);

template <class T, size_t N>
bool verifyByteAgainstArray(uint8_t b, T (&array)[N]);

#endif   // UTILS_H
