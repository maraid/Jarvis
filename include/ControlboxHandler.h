#ifndef CONTROLBOXHANDLER_H
#define CONTROLBOXHANDLER_H

#include "SerialDevice.h"
#include "utils.h"

#include <SoftwareSerial.h>

#define RECEIVE_PIN 14
#define TRANSMIT_PIN 4

class ControlboxHandler : public SerialDevice
{
public:
    ControlboxHandler();

    void setup() override;
    void loop() override;

    void requestSettings();

protected:
    size_t  write(uint8_t byte) override;
    int read() override;
    int available() override;
    using Print::write;

private:
    SoftwareSerial mSerial;
};

#endif  // CONTROLBOXHANDLER_H

