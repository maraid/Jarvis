#include "jarvis.h"
#include "logger.h"


void Jarvis::setup()
{
    m_controller.setup();
    m_handset.setup();
}

void Jarvis::loop()
{
    handleHandset();
    handleController();

    m_controller.loop();
    m_handset.loop();
}

void Jarvis::handleHandset()
{
    SerialMessage inMsg;
    if (m_handset.fetchMessage(inMsg))
    {
        jlog::debug("From Handset: %s", inMsg.toString().c_str());
        m_controller.handleMessage(inMsg);
    }
}

void Jarvis::handleController()
{
    SerialMessage inMsg;
    if (m_controller.fetchMessage(inMsg))
    {
        jlog::debug("From Controller: %s", inMsg.toString().c_str());
        m_handset.handleMessage(inMsg);
    }
}

void Jarvis::setOffset(uint16_t value)
{
    m_controller.setOffset(value);
}

void Jarvis::setUnits(UnitsValue value)
{
    m_controller.setUnits(value);
}

void Jarvis::setTouchMode(TouchModeValue value)
{
    m_controller.setTouchMode(value);
}

void Jarvis::setAntiCollisionMode(AntiCollisionModeValue value)
{
    m_controller.setAntiCollisionMode(value);
}

void Jarvis::setSensitivity(SensitivityValue value)
{
    m_controller.setSensitivity(value);
}

void Jarvis::moveTo(uint16_t height)
{
    // Hysteresis correction
    if (height > m_handset.getLastReportedHeight())
        height += 2;
    m_controller.moveTo(height);
}

void Jarvis::moveToPreset1()
{
    m_controller.moveToPreset1();
}

void Jarvis::moveToPreset2()
{
    m_controller.moveToPreset2();
}

void Jarvis::moveToPreset3()
{
    m_controller.moveToPreset3();
}

void Jarvis::moveToPreset4()
{
    m_controller.moveToPreset4();
}

void Jarvis::setPreset1()
{
    m_controller.setPreset1();
}

void Jarvis::setPreset2()
{
    m_controller.setPreset2();
}

void Jarvis::setPreset3()
{
    m_controller.setPreset3();
}

void Jarvis::setPreset4()
{
    m_controller.setPreset4();
}

void Jarvis::setMaxHeight()
{
    m_controller.setMaxHeight();
}

void Jarvis::setMinHeight()
{
    m_controller.setMinHeight();
}

void Jarvis::clearMaxHeight()
{
    m_controller.clearMaxHeight();
}

void Jarvis::clearMinHeight()
{
    m_controller.clearMinHeight();
}
 
void Jarvis::setDisplayNumber(uint16_t val)
{
    m_handset.setDisplayValue(val);
}

void Jarvis::registerHeightCallback(Callback<uint16_t>* callback)
{
    m_controller.registerHeightCallback(callback);
}

void Jarvis::registerPreset1Callback(Callback<uint16_t>* callback)
{
    m_controller.registerPreset1Callback(callback);
}

void Jarvis::registerPreset2Callback(Callback<uint16_t>* callback)
{
    m_controller.registerPreset2Callback(callback);
}

void Jarvis::registerPreset3Callback(Callback<uint16_t>* callback)
{
    m_controller.registerPreset3Callback(callback);
}

void Jarvis::registerPreset4Callback(Callback<uint16_t>* callback)
{
    m_controller.registerPreset4Callback(callback);
}

void Jarvis::registerUnitsCallback(Callback<UnitsValue>* callback)
{
    m_controller.registerUnitsCallback(callback);
}

void Jarvis::registerTouchModeCallback(Callback<TouchModeValue>* callback)
{
    m_controller.registerTouchModeCallback(callback);
}

void Jarvis::registerAntiCollisionModeCallback(Callback<AntiCollisionModeValue>* callback)
{
    m_controller.registerAntiCollisionModeCallback(callback);
}

void Jarvis::registerSensitivityCallback(Callback<SensitivityValue>* callback)
{
    m_controller.registerSensitivityCallback(callback);
}

void Jarvis::registerSysLimitMinCallback(Callback<uint16_t>* callback)
{
    m_controller.registerSysLimitMinCallback(callback);
}

void Jarvis::registerSysLimitMaxCallback(Callback<uint16_t>* callback)
{
    m_controller.registerSysLimitMaxCallback(callback);
}


void Jarvis::registerUserLimitSetCallback(Callback<UserLimitSetValue>* callback)
{
    m_controller.registerUserLimitSetCallback(callback);
}

void Jarvis::registerUserLimitMinCallback(Callback<uint16_t>* callback)
{
    m_controller.registerUserLimitMinCallback(callback);
}

void Jarvis::registerUserLimitMaxCallback(Callback<uint16_t>* callback)
{
    m_controller.registerUserLimitMaxCallback(callback);
}
