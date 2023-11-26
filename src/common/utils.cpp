#include "utils.h"


void charToHex(const uint8_t* pin, char* outp /*exactly len 3*/)
{
    const char * hex = "0123456789ABCDEF";
    const uint8_t* pin_ = pin;
    *outp++ = hex[(*pin_>>4)&0xF];
    *outp++ = hex[(*pin_)&0xF];
    *outp = 0;
}

void arrayToString(const uint8_t* data, int size, char* outp)
{
    const uint8_t* pin = data;
    char* pout = outp;
    char hexChar[3];
    for(int i = 0; i < size-1; ++i){
        charToHex(pin++, hexChar);
        *pout++ = hexChar[0];
        *pout++ = hexChar[1];
        *pout++ = ' ';
    }
    if (size > 0)
    {
        charToHex(pin, hexChar);
        *pout++ = hexChar[0];
        *pout++ = hexChar[1];
    }
    *pout = 0;
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
