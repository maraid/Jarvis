#ifndef HANDSETHANDLER_H
#define HANDSETHANDLER_H

#include "serial_device.h"
#include "timer.h"
#include "utils.h"

enum HandsetMode
{
    Factory,
    ScreenOff,
    ShowHeight,
    ShowValue
};

class HandsetHandler : public HardwareSerialDevice
{
public:
    HandsetHandler();

    void setup() override;
    void loop() override;

    void setDisplayValue(uint16_t value);
    void displayError(uint8_t code);

    void setHandsetMode(HandsetMode mode);
    void handleMessage(const SerialMessage& msg);
    void executeLedsOffExploit();
    uint16_t getLastReportedHeight();

private:
    Timer m_alwaysOnTimer{constants::ALWAYS_ON_TIMER_DELAY};
    Timer m_displayValueTimer{constants::DISPLAY_VALUE_TIMER_DELAY};
    HandsetMode m_mode{HandsetMode::ShowValue};

    uint16_t m_lastReportedHeight{0};
    uint16_t m_displayValue;
};

#endif
