#include <Arduino.h>

#include "serial_device.h"
#include "utils.h"
#include "constants.h"
#include "logger.h"

// SERIAL DEVICE

SerialDevice::SerialDevice(uint8_t id) 
    : m_id(id)
{
}

void SerialDevice::setup()
{
}

void SerialDevice::loop()
{
    while (!m_buffer.isEmpty())
    {
        sendPacket(m_buffer.pop_front().construct());
    }
}

void SerialDevice::sendPacket(const Packet& packet)
{
    write(packet.data(), packet.size());
}

void SerialDevice::sendMessage(const SerialMessage& msg, uint8_t repetition)
{
    for (int _ = 0; _ < repetition; ++_)
    {
        m_buffer.push_back(msg);
    }
}

void SerialDevice::sendMessage(const SerialMessage&& msg, uint8_t repetition)
{
    sendMessage(msg, repetition);
}

bool SerialDevice::fetchMessage(SerialMessage& msg)
{
    Packet packet;
    bool success = fetchNextCommand(packet);
    if (success)
    {
        success = msg.setPacket(packet);
    }
    return success;
}

bool SerialDevice::fetchNextCommand(Packet& packet)
{
    bool isValid = false;
    while (available() > 0 && !isValid)
    {
        isValid = processData(read());
    }
    
    if (isValid)
    {
        packet = m_partialPacket;  // copy
    }
    return isValid;
}

bool SerialDevice::processData(uint8_t octet)
{
    if (StateMachineState::Start == m_stateMachineState)
    {
        m_partialPacket.clear();
    }

    m_partialPacket.append(octet);

    switch (m_stateMachineState)
    {
        case StateMachineState::Start:
        {
            if (octet == m_id)
            {
                m_stateMachineState = StateMachineState::Id;
            }
            else
            {
                m_stateMachineState = StateMachineState::Start;
            }
            break;
        }
        case StateMachineState::Id:
        {
            if (octet == m_id)
            {
                m_stateMachineState = StateMachineState::Command;
            }
            else
            {
                m_stateMachineState = StateMachineState::Start;
            }
            break;
        }
        case StateMachineState::Command:
        {
            m_stateMachineState = StateMachineState::ParamSize;
            break;
        }
        case StateMachineState::ParamSize:
        {
            switch (octet)
            {
                case 0x00: m_stateMachineState = StateMachineState::Checksum; break;
                case 0x01: m_stateMachineState = StateMachineState::Param0; break;
                case 0x02: m_stateMachineState = StateMachineState::Param1; break;
                case 0x03: m_stateMachineState = StateMachineState::Param2; break;
                case 0x04: m_stateMachineState = StateMachineState::Param3; break;
                default: 
                {
                    m_stateMachineState = StateMachineState::Start;
                }
            }
            break;
        }
        case StateMachineState::Param0:
        {
            m_stateMachineState = StateMachineState::Checksum; 
            break;
        }
        case StateMachineState::Param1:
        {
            m_stateMachineState = StateMachineState::Param0; 
            break;
        }
        case StateMachineState::Param2:
        {
            m_stateMachineState = StateMachineState::Param1;
            break;
        }
        case StateMachineState::Param3:
        {
            m_stateMachineState = StateMachineState::Param2;
            break;
        }
        case StateMachineState::Checksum:
        {
            uint8_t cmd = m_partialPacket.data()[2];
            uint8_t paramSize = m_partialPacket.data()[3];

            uint8_t params[MAX_PARAM_SIZE];
            memcpy(params, m_partialPacket.data() + 4, paramSize);
            
            uint8_t chk = SerialMessage::computeChecksum(cmd, paramSize, params);
            if (chk == octet)
            {
                m_stateMachineState = StateMachineState::End;
            }
            else
            {
                m_stateMachineState = StateMachineState::Start;
            }
            break;
        }
        case StateMachineState::End:
        {
            m_stateMachineState = StateMachineState::Start;
            return octet == 0x7E;
        }
    }
    return false;
}

HardwareSerialDevice::HardwareSerialDevice(SourceType sourceType)
    : SerialDevice(sourceType)
{
}

size_t HardwareSerialDevice::write(uint8_t byte)
{ 
    return Serial.write(byte); 
}

int HardwareSerialDevice::read()
{ 
    return Serial.read(); 
}

int HardwareSerialDevice::available()
{ 
    return Serial.available(); 
}

void HardwareSerialDevice::setup()
{
    SerialDevice::setup();

    if (!Serial) {
        Serial.begin(constants::BAUDRATE);
    }
}

void HardwareSerialDevice::loop()
{
    SerialDevice::loop();
}

SoftwareSerialDevice::SoftwareSerialDevice(SourceType sourceType)
    : SerialDevice(sourceType)
{
}

size_t  SoftwareSerialDevice::write(uint8_t byte) 
{
    return m_serial.write(byte); 
}

int SoftwareSerialDevice::read()
{ 
    return m_serial.read();
}

int SoftwareSerialDevice::available() 
{ 
    return m_serial.available(); 
}

void SoftwareSerialDevice::setup()
{
    SerialDevice::setup();
    m_serial.begin(constants::BAUDRATE,
        SWSERIAL_8N1,
        constants::SOFTWARE_SERIAL_RX,
        constants::SOFTWARE_SERIAL_TX,
        false
    );

    if (!m_serial)
    {
        jlog::error("EspSoftwareSerial initialization failed! Check configured pins.");
    }
}

void SoftwareSerialDevice::loop()
{
    SerialDevice::loop();
}

