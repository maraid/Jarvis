#include "HandsetHandler.h"
// #include "TelnetLogger.h"

HandsetHandler::HandsetHandler()
    : SerialDevice(SourceType::Handset),
    mAlwaysOnTimer(1000),
    mDisplayValueTimer(100),
    mMode(HandsetMode::ShowValue), 
    mLastReportedHeight(0)
{
}

size_t HandsetHandler::write(uint8_t byte)
{ 
    return Serial.write(byte); 
}

int HandsetHandler::read()
{ 
    return Serial.read(); 
}

int HandsetHandler::available()
{ 
    return Serial.available(); 
}

void HandsetHandler::setup()
{
    if (!Serial) {
        Serial.begin(BAUDRATE);
    }

    if (mMode != HandsetMode::Factory)
        mAlwaysOnTimer.start();
}

void HandsetHandler::loop()
{
    if (mAlwaysOnTimer.isFinished())
    {
        sendMessage(SerialMessage(CommandFromControlboxType::Height,
                                  static_cast<uint16_t>(0)));
        mAlwaysOnTimer.restart();
        
        if (mMode == HandsetMode::ShowValue)
            mDisplayValueTimer.start();
    }
    
    if (mDisplayValueTimer.isFinished())
    {
        sendMessage(SerialMessage(CommandFromControlboxType::Height, mDisplayValue));
        mDisplayValueTimer.reset();
    }
}

void HandsetHandler::setDisplayValue(uint16_t value)
{
    mDisplayValue = value;
}

void HandsetHandler::displayError(uint8_t code)
{
    sendMessage(SerialMessage(CommandFromControlboxType::Error, code));
}

void HandsetHandler::setHandsetMode(HandsetMode mode)
{
    mMode = mode;
}

uint16_t HandsetHandler::getLastReportedHeight()
{
    return mLastReportedHeight;
}

void HandsetHandler::handleCBMessage(SerialMessage& msg)
{
    switch(msg.getType())
    {
    case CommandFromControlboxType::Height:
    {
        uint16_t height = msg.getParam<uint16_t>();

        switch (mMode)
        {
        case HandsetMode::ScreenOff:
            break;
        case HandsetMode::Factory:
            sendMessage(msg); break;
        case HandsetMode::ShowHeight:
        case HandsetMode::ShowValue:
            if(height != mLastReportedHeight)
            {
                mAlwaysOnTimer.reset();
                sendMessage(msg);
            }
            else
            {
                mAlwaysOnTimer.start();
            }   
            break;
        default:
            sendMessage(msg); break;
        }

        mLastReportedHeight = height;
        break;
    }
    case CommandFromControlboxType::Preset:
    {
        switch(mMode)
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
    sendMessage(SerialMessage(CommandFromControlboxType::Height, mLastReportedHeight));

    // wait for the screen to turn off
    delay(11000);

    // send empty screen error
    displayError(0x10);

    //wait for the screen to turn off
    delay(11000);
}
