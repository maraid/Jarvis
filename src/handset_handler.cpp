#include "handset_handler.h"
#include "constants.h"


HandsetHandler::HandsetHandler()
    : HardwareSerialDevice(SourceType::Handset)
{
}

void HandsetHandler::setup()
{
    HardwareSerialDevice::setup();
    
    if (m_mode != HandsetMode::Factory)
        m_alwaysOnTimer.start();
}

void HandsetHandler::loop()
{
    HardwareSerialDevice::loop();
    if (m_alwaysOnTimer.isFinished())
    {
        sendMessage({CommandFromControllerType::Height, constants::KEEP_ALIVE_VALUE});
        m_alwaysOnTimer.restart();
    }
    
    if (m_displayValueTimer.isFinished())
    {
        sendMessage({CommandFromControllerType::Height, m_displayValue});
        m_displayValueTimer.restart();
    }
}

void HandsetHandler::setDisplayValue(uint16_t value)
{
    m_displayValue = value;
    sendMessage({CommandFromControllerType::Height, value});
}

void HandsetHandler::displayError(uint8_t code)
{
    sendMessage({CommandFromControllerType::Error, code});
}

void HandsetHandler::setHandsetMode(HandsetMode mode)
{
    m_mode = mode;
}

uint16_t HandsetHandler::getLastReportedHeight()
{
    return m_lastReportedHeight;
}

void HandsetHandler::handleMessage(const SerialMessage& msg)
{
    switch(msg.getType())
    {
    case CommandFromControllerType::Height:
    {
        uint16_t height = msg.getParam<uint16_t>();

        switch (m_mode)
        {
        case HandsetMode::ScreenOff:
            break;
        case HandsetMode::ShowValue:
            if(height != m_lastReportedHeight)
            {
                m_displayValueTimer.restart();
                m_alwaysOnTimer.restart();
                sendMessage(msg);
            }   
            break;
        default:
            sendMessage(msg);
            break;
        }

        m_lastReportedHeight = height;
        break;
    }
    case CommandFromControllerType::Preset:
    {
        switch(m_mode)
        {
        case HandsetMode::ScreenOff:
            break;
        default:
            sendMessage(msg); break;
        }
        break;
    }
    default:
        sendMessage(msg);
        break;
    }
}

void HandsetHandler::executeLedsOffExploit()
{
    // quit screen off mode
    sendMessage(SerialMessage(CommandFromControllerType::Height, m_lastReportedHeight));

    // wait for the screen to turn off
    delay(11000);

    // send empty screen error
    displayError(0x10);

    //wait for the screen to turn off
    delay(11000);
}
