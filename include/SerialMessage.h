#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#define MIN_PACKET_SIZE 6
#define MAX_PARAM_SIZE 4
#define MAX_PACKET_SIZE MIN_PACKET_SIZE + MAX_PARAM_SIZE

#define FIRST_BYTE(X) ((uint8_t)(X >> 8))
#define SECOND_BYTE(X) ((uint8_t)X)
#define COMBINE_BYTES(X, Y) ((((uint16_t)X) << 8) | Y)

enum SourceType : uint8_t
{
    Handset    = 0xF1,
    Controlbox = 0xF2
};

enum CommandFromControlboxType : uint8_t
{
    Height           = 0x01,
    Error            = 0x02,
    Reset            = 0x04,
    AbsLimits        = 0x07,
    Calibration      = 0x1B,
    MovingTo         = 0x1B,
    Version          = 0x1C,
    MinMaxSet        = 0x20,
    MaxHeight        = 0x21,
    MinHeight        = 0x22,
    MinMaxReached    = 0x23,
    Preset           = 0x92,

    // Settings
    LocPreset1       = 0x25,
    LocPreset2       = 0x26,
    LocPreset3       = 0x27,
    LocPreset4       = 0x28,
    Units            = 0x0E,
    TouchMode        = 0x19,
    KillMode         = 0x17,
    Sensitivity      = 0x1D,
};

enum CommandFromHandsetType : uint8_t
{
    Up               = 0x01,
    Down             = 0x02,
    SetPreset1       = 0x03,
    SetPreset2       = 0x04,
    SetPreset3       = 0x25,
    SetPreset4       = 0x26,
    MoveToPreset1    = 0x05,
    MoveToPreset2    = 0x06,
    MoveToPreset3    = 0x27,
    MoveToPreset4    = 0x28,
    GetSettings      = 0x07,
    GetAbsLimits     = 0x0C,
    SetUnits         = 0x0E,
    SetOffset        = 0x10,
    SetKillMode      = 0x17,
    SetTouchMode     = 0x19,
    MoveTo           = 0x1B,
    GetVersion       = 0x1C,  // unkown
    SetSensitivity   = 0x1D,
    GetUserLimits    = 0x20,
    SetMaxHeight     = 0x21,
    SetMinHeight     = 0x22,
    ClearMinMax      = 0x23,
    Wake             = 0x29,
    SetMoveToLoc     = 0x80, 
    EnterCalibration = 0x91
};

template <class T, size_t N>
bool verifyByteAgainstArray(uint8_t b, T (&array)[N])
{
    for (int i = 0; i < N; ++i)
        if (array[i] == b)
            return true;

    return false;
}

class SerialMessage
{
public:
    SerialMessage();

    SerialMessage(CommandFromHandsetType cmd);
    SerialMessage(CommandFromControlboxType cmd);

    SerialMessage(CommandFromHandsetType cmd, uint8_t p0);
    SerialMessage(CommandFromControlboxType cmd, uint8_t p0);

    SerialMessage(CommandFromHandsetType cmd, uint8_t p0, uint8_t p1);
    SerialMessage(CommandFromControlboxType cmd, uint8_t p0, uint8_t p1);

    SerialMessage(CommandFromHandsetType cmd, uint16_t p01);
    SerialMessage(CommandFromControlboxType cmd, uint16_t p01);
    
    SerialMessage(uint8_t sourceType, uint8_t commandType, size_t paramSize = 0, uint8_t p0 = 0, uint8_t p1 = 0, uint8_t p2 = 0, uint8_t p3 = 0);

    virtual ~SerialMessage() {};

    String toString() const;

    uint8_t getType() const;
    void setType(uint8_t val);

    size_t getPacketLength() const;

    SourceType getSourceId() const;
    void setSourceId(SourceType tId);

    size_t getParamSize() const;
    void setParamSize(size_t size);

    template<typename T>
    T getParam() const
    {
        return static_cast<T>(mParams[0]);
    }

    template<typename T>
    bool setParam(T value)
    {
        return setParam(static_cast<uint8_t>(value));
    }

    uint8_t getParamN(size_t n) const;
    bool setParamN(uint8_t value, size_t n);

    bool setParam(uint8_t p0);
    bool setParam(uint8_t p0, uint8_t p1);
    bool setParam(uint8_t p0, uint8_t p1, uint8_t p2);
    bool setParam(uint16_t p01);
   
    void getParamArray(uint8_t* params, uint8_t& paramSize) const;
    void setParamArray(uint8_t* params, uint8_t paramSize);

    void construct(uint8_t* data) const;
    bool setPacket(uint8_t* data, size_t dataSize);
    static bool verifyPacket(uint8_t* data, size_t dataSize);
    
    static uint8_t computeChecksum(uint8_t command, uint8_t paramSize, const uint8_t* params);

protected:
    uint8_t mSourceId;
    uint8_t mType;
    uint8_t mParamSize;
    uint8_t mParams[MAX_PARAM_SIZE];
};

template<>
inline uint16_t SerialMessage::getParam<uint16_t>() const
{
    return COMBINE_BYTES(mParams[0], mParams[1]);
}

#endif  // SERIALMESSAGE_H