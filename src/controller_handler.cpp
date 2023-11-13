#include "controller_handler.h"


ControllerHandler::ControllerHandler()
    : SoftwareSerialDevice(SourceType::Controller)
    , m_stateRequestTimer(constants::INITIAL_STATE_REQUEST_DELAY)
{
}

void ControllerHandler::setup()
{ 
    SoftwareSerialDevice::setup();
    wakeUp();
    m_stateRequestTimer.start();
}

void ControllerHandler::loop()
{
    SoftwareSerialDevice::loop();

    if (m_stateRequestTimer.isFinished())
    {
        m_stateRequestTimer.restart();

        if (!m_states.isSysLimitsComplete())
        {
            requestSystemLimits();
        }
        else if (!m_states.isUserLimitsComplete())
        {
            requestUserLimits();
        }
        else if (!m_states.isSettingsComplete())
        {
            requestSettings();
        }
        else
        {
            // all states have been requested. stop
            m_stateRequestTimer.reset();
        }
    }
}

void ControllerHandler::handleMessage(const SerialMessage& msg)
{
    // forward everything handset->controller
    sendMessage(msg);
}

bool ControllerHandler::fetchMessage(SerialMessage& msg)
{
    bool success = SoftwareSerialDevice::fetchMessage(msg);

    saveState(msg);

    return success;
}

void ControllerHandler::wakeUp()
{
    sendMessage(SerialMessage{CommandFromHandsetType::Wake});
}

void ControllerHandler::requestUserLimits()
{
    sendMessage(SerialMessage{CommandFromHandsetType::GetUserLimits});
}

void ControllerHandler::requestSystemLimits()
{
    sendMessage(SerialMessage{CommandFromHandsetType::GetAbsLimits});
}

void ControllerHandler::requestSettings()
{
    sendMessage(SerialMessage{CommandFromHandsetType::GetSettings});
}

void ControllerHandler::setOffset(uint16_t offset)
{
    sendMessage({CommandFromHandsetType::SetOffset, static_cast<uint16_t>(offset - 6)});
}

void ControllerHandler::setUnits(UnitsValue value)
{
    sendMessage({CommandFromHandsetType::SetUnits, value});
    requestSettings();
}

void ControllerHandler::setTouchMode(TouchModeValue value)
{
    sendMessage({CommandFromHandsetType::SetTouchMode, value});
    requestSettings();
}

void ControllerHandler::setAntiCollisionMode(AntiCollisionModeValue value)
{
    sendMessage({CommandFromHandsetType::SetAntiCollisionMode, value});
    requestSettings();
}

void ControllerHandler::setSensitivity(SensitivityValue value)
{
    sendMessage({CommandFromHandsetType::SetSensitivity, value});
    requestSettings();
}

void ControllerHandler::moveTo(uint16_t height)
{
    sendMessage({CommandFromHandsetType::MoveTo, uint16_t{height}});
}

void ControllerHandler::moveToPreset1()
{
    sendMessage(SerialMessage{CommandFromHandsetType::MoveToPreset1});
}

void ControllerHandler::moveToPreset2()
{
    sendMessage(SerialMessage{CommandFromHandsetType::MoveToPreset2});
}

void ControllerHandler::moveToPreset3()
{
    sendMessage(SerialMessage{CommandFromHandsetType::MoveToPreset3});
}

void ControllerHandler::moveToPreset4()
{
    sendMessage(SerialMessage{CommandFromHandsetType::MoveToPreset4});
}

void ControllerHandler::setPreset1()
{
    sendMessage(SerialMessage{CommandFromHandsetType::SetPreset1});
    requestSettings();
}

void ControllerHandler::setPreset2()
{
    sendMessage(SerialMessage{CommandFromHandsetType::SetPreset2});
    requestSettings();
}

void ControllerHandler::setPreset3()
{
    sendMessage(SerialMessage{CommandFromHandsetType::SetPreset3});
    requestSettings();
}

void ControllerHandler::setPreset4()
{
    sendMessage(SerialMessage{CommandFromHandsetType::SetPreset4});
    requestSettings();
}

void ControllerHandler::setMaxHeight()
{
    sendMessage(SerialMessage{CommandFromHandsetType::SetMaxHeight});
    requestUserLimits();
}

void ControllerHandler::setMinHeight()
{
    sendMessage(SerialMessage{CommandFromHandsetType::SetMinHeight});
    requestUserLimits();
}

void ControllerHandler::clearMaxHeight()
{
    sendMessage({CommandFromHandsetType::ClearMinMax, uint8_t{0x01}});
    requestUserLimits();
}

void ControllerHandler::clearMinHeight()
{
    sendMessage({CommandFromHandsetType::ClearMinMax, uint8_t{0x02}});
    requestUserLimits();
}

void ControllerHandler::saveState(const SerialMessage& msg)
{
    switch(msg.getType())
    {
        case CommandFromControllerType::LocPreset1:
            m_states.setPreset1(msg.getParam<uint16_t>());
            call(p_preset1Callback, m_states.getPreset1());
            break;
        case CommandFromControllerType::LocPreset2:
            m_states.setPreset2(msg.getParam<uint16_t>()); 
            call(p_preset2Callback, m_states.getPreset2());
            break;
        case CommandFromControllerType::LocPreset3:
            m_states.setPreset3(msg.getParam<uint16_t>()); 
            call(p_preset3Callback, m_states.getPreset3());
            break;
        case CommandFromControllerType::LocPreset4:
            m_states.setPreset4(msg.getParam<uint16_t>());
            call(p_preset4Callback, m_states.getPreset4());
            break;
        case CommandFromControllerType::Units:
            m_states.setUnits(msg.getParam<UnitsValue>()); 
            call(p_unitsCallback, m_states.getUnits());
            break;
        case CommandFromControllerType::TouchMode:
            m_states.setTouchMode(msg.getParam<TouchModeValue>()); 
            call(p_touchModeCallback, m_states.getTouchMode());
            break;
        case CommandFromControllerType::AntiCollisionMode:
            m_states.setAntiCollisionMode(msg.getParam<AntiCollisionModeValue>()); 
            call(p_antiCollisionModeCallback, m_states.getAntiCollisionMode());
            break;
        case CommandFromControllerType::Sensitivity:
            m_states.setSensitivity(msg.getParam<SensitivityValue>());
            call(p_sensitivityCallback, m_states.getSensitivity());
            break;
        case CommandFromControllerType::MinMaxSet:
            m_states.setUserLimitSet(msg.getParam<UserLimitSetValue>());
            call(p_userLimitSetCallback, m_states.getUserLimitSet());
            break;
        case CommandFromControllerType::MinHeight:
            m_states.setUserLimitMin(msg.getParam<uint16_t>());
            call(p_userLimitMinCallback, m_states.getUserLimitMin());
            break;
        case CommandFromControllerType::MaxHeight:
            m_states.setUserLimitMax(msg.getParam<uint16_t>());
            call(p_userLimitMaxCallback, m_states.getUserLimitMax());
            break;
        case CommandFromControllerType::AbsLimits:
            uint8_t params[4];
            uint8_t paramSize;
            msg.getParamArray(params, paramSize);
            
            // -1 = probably rounding error inside
            m_states.setSysLimitMax(COMBINE_BYTES(params[0], params[1]) - 1);
            m_states.setSysLimitMin(COMBINE_BYTES(params[2], params[3]) - 1);

            call(p_sysLimitMaxCallback, m_states.getSysLimitMax());
            call(p_sysLimitMinCallback, m_states.getSysLimitMin());
            break;
        case CommandFromControllerType::Height:
            call(p_heightCallback, msg.getParam<uint16_t>());
            break;
        default:
            break;
    }
}



void ControllerHandler::registerHeightCallback(Callback<uint16_t>* callback)
{
    p_heightCallback = callback;
}

void ControllerHandler::registerPreset1Callback(Callback<uint16_t>* callback)
{
    p_preset1Callback = callback;
}


void ControllerHandler::registerPreset2Callback(Callback<uint16_t>* callback)
{
    p_preset2Callback = callback;
}


void ControllerHandler::registerPreset3Callback(Callback<uint16_t>* callback)
{
    p_preset3Callback = callback;
}


void ControllerHandler::registerPreset4Callback(Callback<uint16_t>* callback)
{
    p_preset4Callback = callback;
}


void ControllerHandler::registerUnitsCallback(Callback<UnitsValue>* callback)
{
    p_unitsCallback = callback;
}


void ControllerHandler::registerTouchModeCallback(Callback<TouchModeValue>* callback)
{
    p_touchModeCallback = callback;
}


void ControllerHandler::registerAntiCollisionModeCallback(Callback<AntiCollisionModeValue>* callback)
{
    p_antiCollisionModeCallback = callback;
}


void ControllerHandler::registerSensitivityCallback(Callback<SensitivityValue>* callback)
{
    p_sensitivityCallback = callback;
}


void ControllerHandler::registerSysLimitMinCallback(Callback<uint16_t>* callback)
{
    p_sysLimitMinCallback = callback;
}


void ControllerHandler::registerSysLimitMaxCallback(Callback<uint16_t>* callback)
{
    p_sysLimitMaxCallback = callback;
}


void ControllerHandler::registerUserLimitSetCallback(Callback<UserLimitSetValue>* callback)
{
    p_userLimitSetCallback = callback;
}


void ControllerHandler::registerUserLimitMinCallback(Callback<uint16_t>* callback)
{
    p_userLimitMinCallback = callback;
}


void ControllerHandler::registerUserLimitMaxCallback(Callback<uint16_t>* callback)
{
    p_userLimitMaxCallback = callback;
}
