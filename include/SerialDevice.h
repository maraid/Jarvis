#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include "SerialMessage.h"

#define BAUDRATE 9600

class SerialDevice : public Print
{
public:
    SerialDevice(uint8_t id);
    virtual ~SerialDevice();

    virtual void setup() = 0;
    virtual void loop() = 0;

    void sendPacket(uint8_t* packet, size_t packetSize);
    void sendMessage(const SerialMessage& msg, uint8_t repetition = 1);
    void sendMessage(const SerialMessage&& msg, uint8_t repetition = 1);
    
    bool fetchMessage(SerialMessage& msg);

protected:
    virtual size_t write(uint8_t byte) = 0;
    virtual int read() = 0;
    virtual int available() = 0;
    using Print::write;

private:
    enum class StateMachineState
    {
        Start,
        Id,
        Command,
        ParamSize,
        Param0,
        Param1,
        Param2,
        Param3,
        Checksum,
        End
    };

    bool fetchNextCommand(uint8_t* array, size_t& arraySize);
    bool processData(uint8_t oktet);

    uint8_t mId;
    uint8_t mPartialMessage[MAX_PACKET_SIZE];
    size_t mPMSize;
    StateMachineState mSMState;
};

#endif  // SERIALDEVICE_H
