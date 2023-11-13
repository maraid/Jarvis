#include "serial_message.h"

SerialMessage::SerialMessage()
{
}

SerialMessage::SerialMessage(CommandFromControllerType cmd)
    : SerialMessage(SourceType::Controller, cmd, 0)
{
}

SerialMessage::SerialMessage(CommandFromControllerType cmd, uint8_t p0)
    : SerialMessage(SourceType::Controller, cmd, 1, p0)
{
}

SerialMessage::SerialMessage(CommandFromControllerType cmd, uint8_t p0, uint8_t p1)
    : SerialMessage(SourceType::Controller, cmd, 2, p0, p1)
{
}

SerialMessage::SerialMessage(CommandFromControllerType cmd, uint16_t p01)
    : SerialMessage(SourceType::Controller, cmd, 2, FIRST_BYTE(p01), SECOND_BYTE(p01))
{
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd)
    : SerialMessage(SourceType::Handset, cmd, 0)
{
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, uint8_t p0)
    : SerialMessage(SourceType::Handset, cmd, 1, p0)
{
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, uint8_t p0, uint8_t p1)
    : SerialMessage(SourceType::Handset, cmd, 2, p0, p1)
{
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, uint16_t p01)
    : SerialMessage(SourceType::Handset, cmd, 2, FIRST_BYTE(p01), SECOND_BYTE(p01))
{
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, UnitsValue p)
    : SerialMessage(cmd)
{
    setParam<UnitsValue>(p);
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, TouchModeValue p)
    : SerialMessage(cmd)
{
    setParam<TouchModeValue>(p);
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, AntiCollisionModeValue p)
    : SerialMessage(cmd)
{
    setParam<AntiCollisionModeValue>(p);
}

SerialMessage::SerialMessage(CommandFromHandsetType cmd, SensitivityValue p)
    : SerialMessage(cmd)
{
    setParam<SensitivityValue>(p);
}

SerialMessage::SerialMessage(
    uint8_t sourceType,
    uint8_t commandType, 
    size_t paramSize, 
    uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3)
    : mSourceId(sourceType),
    mType(commandType),
    mParamSize(paramSize)
{
    mParams[0] = p0;
    mParams[1] = p1;
    mParams[2] = p2;
    mParams[3] = p3;
}

String SerialMessage::toString() const
{
    uint8_t packet[MAX_PACKET_SIZE];
    uint8_t size;
    construct(packet, size);
    return array2String(packet, size);
}

uint8_t SerialMessage::getType() const
{
    return mType;
}

void SerialMessage::setType(uint8_t val)
{
    mType = val;
}

size_t SerialMessage::getPacketLength() const
{
    return 6 + mParamSize;
}

SourceType SerialMessage::getSourceId() const
{
    return static_cast<SourceType>(mSourceId);
}

void SerialMessage::setSourceId(SourceType sourceId)
{
    mSourceId = sourceId;
}

size_t SerialMessage::getParamSize() const
{
    return mParamSize;
}

void SerialMessage::setParamSize(size_t paramSize)
{
    mParamSize = paramSize;
}

uint8_t SerialMessage::getParamN(size_t n) const
{
    return mParams[n];
}

bool SerialMessage::setParamN(uint8_t value, size_t n)
{
    bool isIndexValid = n < MAX_PARAM_SIZE;
    if (isIndexValid)
    {
        mParams[n] = value;
    }
    return isIndexValid;
}

bool SerialMessage::setParam(uint8_t p0)
{
    mParamSize = 1;
    return setParamN(p0, 0);
}

bool SerialMessage::setParam(uint8_t p0, uint8_t p1)
{
    mParamSize = 2;
    return setParamN(p0, 0) && setParamN(p1, 1);
}

bool SerialMessage::setParam(uint8_t p0, uint8_t p1, uint8_t p2)
{
    mParamSize = 3;
    return setParamN(p0, 0) && setParamN(p1, 1) && setParamN(p2, 2);
}

bool SerialMessage::setParam(uint16_t p01)
{
    mParamSize = 2;
    return setParam(FIRST_BYTE(p01), SECOND_BYTE(p01));
}

    
void SerialMessage::getParamArray(uint8_t* params, uint8_t& paramSize) const
{
    memcpy(params, mParams, mParamSize);
    paramSize = mParamSize;
}

void SerialMessage::setParamArray(uint8_t* params, uint8_t paramSize)
{
    memcpy(mParams, params, paramSize);
    mParamSize = paramSize;
}

uint8_t SerialMessage::computeChecksum(uint8_t command, uint8_t paramSize, const uint8_t* params)
{
    uint8_t checksum = (command + paramSize) % 256;
    for(int i = 0; i < paramSize; ++i)
        checksum = (checksum + params[i]) % 256;
    return checksum;
}

void SerialMessage::construct(uint8_t* data, uint8_t& size) const
{
    data[0] = data[1] = mSourceId;
    data[2] = mType;
    data[3] = mParamSize;
    data[4 + mParamSize] = computeChecksum(mType, mParamSize, mParams);
    data[5 + mParamSize] = 0x7E;

    memcpy(data + 4, mParams, mParamSize);
    size = getPacketLength();
}

bool SerialMessage::setPacket(uint8_t* data, size_t dataSize)
{
    if (!verifyPacket(data, dataSize))
    {
        return false;
    }

    mSourceId = data[0];
    mType = data[2];
    mParamSize = data[3];
    
    memcpy(mParams, data + 4, mParamSize);

    return true;
}
   
bool SerialMessage::verifyPacket(uint8_t* data, size_t dataSize)
{   
    uint8_t paramSize = data[3];
    if (paramSize > MAX_PARAM_SIZE)
    {
        return false;
    }
    
    uint8_t params[MAX_PARAM_SIZE];
    memcpy(params, data + 4, paramSize);

    return data[0] == data[1]
           && (   data[0] == SourceType::Handset
               || data[0] == SourceType::Controller)
           && data[0] == data[1]
           && dataSize == static_cast<size_t>(6 + paramSize)
           && data[dataSize - 2] == computeChecksum(data[2], data[3], params)
           && data[dataSize - 1] == 0x7E;
}
