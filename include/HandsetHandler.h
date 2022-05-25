#ifndef HANDSETHANDLER_H
#define HANDSETHANDLER_H

#include "SerialDevice.h"
#include "Timer.h"
#include "utils.h"

enum HandsetMode
{
    Factory,
    ScreenOff,
    ShowHeight,
    ShowValue
};

class HandsetHandler : public SerialDevice
{
public:
    HandsetHandler();

    void setup() override;
    void loop() override;

    void setDisplayValue(uint16_t value);
    void displayError(uint8_t code);

    void setHandsetMode(HandsetMode mode);
    void handleCBMessage(SerialMessage& msg);
    void executeLedsOffExploit();
    uint16_t getLastReportedHeight();
    
protected:
    size_t write(uint8_t byte) override;
    int read() override;
    int available() override;
    using Print::write;

private:
    Timer mAlwaysOnTimer;
    Timer mDisplayValueTimer;
    HandsetMode mMode;

    uint16_t mLastReportedHeight;
    uint16_t mDisplayValue;
};

#endif  // HANDSETHANDLER_H
