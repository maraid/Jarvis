#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <SoftwareSerial.h>
#include <Arduino.h>

#include "serial_message.h"
#include "utils.h"
#include "constants.h"
#include "timer.h"
#include "ring_buffer.h"
#include "packet.h"

class SerialDevice : public Print
{
public:
    explicit SerialDevice(uint8_t id);
    
    virtual void setup();
    virtual void loop();
    virtual void handleMessage(const SerialMessage& msg) = 0;

    void sendMessage(const SerialMessage& msg, uint8_t repetition = 1);
    void sendMessage(const SerialMessage&& msg, uint8_t repetition = 1);
    
    virtual bool fetchMessage(SerialMessage& msg);
    
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

    void sendPacket(const Packet& packet);
    bool fetchNextCommand(Packet& packet);
    bool processData(uint8_t oktet);

    uint8_t m_id;
    Packet m_partialPacket;
    StateMachineState m_stateMachineState{StateMachineState::Start};
    RingBuffer<SerialMessage, 16> m_buffer;
};

class HardwareSerialDevice : public SerialDevice
{
public:
    explicit HardwareSerialDevice(SourceType sourceType);

    void setup() override;
    void loop() override;
    
protected:
    size_t write(uint8_t byte) override;
    int read() override;
    int available() override;
};


class SoftwareSerialDevice : public SerialDevice
{
public:
    explicit SoftwareSerialDevice(SourceType sourceType);

    void setup() override;
    void loop() override;

protected:
    size_t  write(uint8_t byte) override;
    int read() override;
    int available() override;

private:
    SoftwareSerial m_serial;
};

#endif
