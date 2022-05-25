#include "ControlboxHandler.h"

ControlboxHandler::ControlboxHandler()
    : SerialDevice(SourceType::Controlbox), mSerial(RECEIVE_PIN, TRANSMIT_PIN)
{
}

size_t  ControlboxHandler::write(uint8_t byte) 
{
    return mSerial.write(byte); 
}

int ControlboxHandler::read()
{ 
    return mSerial.read();
}

int ControlboxHandler::available() 
{ 
    return mSerial.available(); 
}

void ControlboxHandler::setup()
{ 
    mSerial.begin(BAUDRATE); 
}

void ControlboxHandler::loop()
{
}
