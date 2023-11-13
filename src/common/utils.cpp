#include "utils.h"

String char2hex(uint8_t c)
{
  String ret = String((c < 16 ? "0" : "") + String(c, HEX));
  ret.toUpperCase();
  return ret;
}

String array2String(uint8_t *packet, size_t packetSize)
{
    String result;
    for (size_t i = 0; i < packetSize; ++i)
    {
        result += char2hex(packet[i]) + " ";
    }
    return result;
}

std::string valToString(UnitsValue units)
{
    switch(units)
    {
        case UnitsValue::mm: return "cm";
        case UnitsValue::inch: return "inch";
        default: return "unset";
    }
}

std::string valToString(UserLimitSetValue limits)
{

    switch(limits)
    {
        case UserLimitSetValue::None: return "None";
        case UserLimitSetValue::Max:  return "Max";
        case UserLimitSetValue::Min:  return "Min";
        case UserLimitSetValue::Both: return "Both";
        default: return "unset";
    }
}

std::string valToString(userLimitReachedValue limits)
{
    switch(limits)
    {
        case userLimitReachedValue::MaxReached: return "MaxReached";
        case userLimitReachedValue::MinReached: return "MinReached";
        default: return "unset";
    }
}

std::string valToString(TouchModeValue mode)
{
    switch(mode)
    {
        case TouchModeValue::Single: return "Single";
        case TouchModeValue::Continuous: return "Continuous";
        default: return "unset";
    }
}

std::string valToString(AntiCollisionModeValue mode)
{
    switch(mode)
    {
        case AntiCollisionModeValue::Disabled: return "Disabled";
        case AntiCollisionModeValue::Enabled: return "Enabled";
        default: return "unset";
    }
}

std::string valToString(SensitivityValue sens)
{
    switch(sens)
    {
        case SensitivityValue::High: return "High";
        case SensitivityValue::Medium: return "Medium";
        case SensitivityValue::Low: return "Low";
        default: return "unset";
    }
}
