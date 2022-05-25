#include "JarvisDesk.h"
#include "Timer.h"
#include <math.h>


Timer T1(10000);

JarvisDesk::JarvisDesk()
{
}

void JarvisDesk::setup()
{
    mControlbox.setup();
    mHandset.setup();

    wakeUp();
    delay((long unsigned int)70);
    
    T1.start();
}

void JarvisDesk::loop()
{
    mControlbox.loop();
    mHandset.loop();

    handleHandset();
    handleControlbox();
    
    if (!mSettings and T1.isFinished())
    {
        ESP_LOGI("TAG", "Requesting settings from controlbox...");
        requestAllSettings();
        T1.restart();
    }
}

void JarvisDesk::handleHandset()
{
    SerialMessage inMsg;
    if (not mHandset.fetchMessage(inMsg))
    {
        return;
    }
    
    // ESP_LOGI("TAG", "%s", inMsg.toString().c_str());
    
    // On powerup make the handset stop sending wake commands.
    switch(inMsg.getType())
    {
    case CommandFromHandsetType::SetPreset1:
    case CommandFromHandsetType::SetPreset2:
    case CommandFromHandsetType::SetPreset3:
    case CommandFromHandsetType::SetPreset4:
    case CommandFromHandsetType::SetUnits:
    case CommandFromHandsetType::SetOffset:
    case CommandFromHandsetType::SetTouchMode:
    case CommandFromHandsetType::SetSensitivity:
    case CommandFromHandsetType::SetMaxHeight:
    case CommandFromHandsetType::SetMinHeight:
    case CommandFromHandsetType::ClearMinMax:
        sendMessage(inMsg);
        requestAllSettings();
        break;
    default:
        sendMessage(inMsg);  // handset->controlbox uninterrupted
    }
}

void JarvisDesk::handleControlbox()
{
    SerialMessage inMsg;
    if (not mControlbox.fetchMessage(inMsg))
    {
        return;
    }
    
    // ESP_LOGI("TAG", "%s", inMsg.toString().c_str());

    extractSetting(inMsg);

    mHandset.handleCBMessage(inMsg);
}

void JarvisDesk::extractSetting(const SerialMessage& msg)
{
    switch(msg.getType())
    {
    case CommandFromControlboxType::LocPreset1:
        mSettings.presetRaw1 = msg.getParam<uint16_t>(); break;
    case CommandFromControlboxType::LocPreset2:
        mSettings.presetRaw2 = msg.getParam<uint16_t>(); break;
    case CommandFromControlboxType::LocPreset3:
        mSettings.presetRaw3 = msg.getParam<uint16_t>(); break;
    case CommandFromControlboxType::LocPreset4:
        mSettings.presetRaw4 = msg.getParam<uint16_t>(); break;
    case CommandFromControlboxType::Units:
        mSettings.units = msg.getParam<UnitsValue>(); break;
    case CommandFromControlboxType::TouchMode:
        mSettings.touchMode = msg.getParam<TouchModeValue>(); break;
    case CommandFromControlboxType::KillMode:
        mSettings.killMode = msg.getParam<KillModeValue>(); break;
    case CommandFromControlboxType::Sensitivity:
    {
        mSettings.sensitivity = msg.getParam<SensitivityValue>();
        
        ESP_LOGI("TAG", "Sensitivity: ", mSettings.sensitivity);
        
        sPreset1->publish_state(mSettings.getPreset1()); 
        sPreset2->publish_state(mSettings.getPreset2()); 
        sPreset3->publish_state(mSettings.getPreset3()); 
        sPreset4->publish_state(mSettings.getPreset4()); 
        sUnits->publish_state(valToString(mSettings.units));
        sTouchMode->publish_state(valToString(mSettings.touchMode));
        sKillMode->publish_state(valToString(mSettings.killMode));
        sSensitivity->publish_state(valToString(mSettings.sensitivity));
        break;
    }
    case CommandFromControlboxType::MinMaxSet:
        mSettings.userLimitSet = msg.getParam<UserLimitSetValue>();
        sUserLimitSet->publish_state(valToString(mSettings.userLimitSet));
        break;
    case CommandFromControlboxType::MinHeight:
        mSettings.userLimitMin = msg.getParam<uint16_t>();
        sUserLimitMin->publish_state(mSettings.userLimitMin);
        break;
    case CommandFromControlboxType::MaxHeight:
        mSettings.userLimitMax = msg.getParam<uint16_t>();
        sUserLimitMax->publish_state(mSettings.userLimitMax);
        break;
    case CommandFromControlboxType::AbsLimits:
    {
        uint8_t params[4];
        uint8_t paramSize;
        msg.getParamArray(params, paramSize);
        
        // -1 = probably rounding error inside
        mSettings.sysLimitMax = COMBINE_BYTES(params[0], params[1]) - 1;
        mSettings.sysLimitMin = COMBINE_BYTES(params[2], params[3]) - 1;
        
        sSysLimitMin->publish_state(mSettings.sysLimitMin);
        sSysLimitMax->publish_state(mSettings.sysLimitMax);
        
        break;
    }
    case CommandFromControlboxType::Height:
        sHeight->publish_state(msg.getParam<uint16_t>()); break;
    default:
        break;
    }
}

void JarvisDesk::processResponse(uint32_t duration)
{
    Timer ct(duration);
    ct.start();
    while (!ct.isFinished())
    {
        handleControlbox();
    }
}

void JarvisDesk::sendMessage(const SerialMessage& msg, uint8_t reps)
{
    switch (msg.getSourceId())
    {
    case SourceType::Handset:
        mControlbox.sendMessage(msg, reps);
        break;
    case SourceType::Controlbox:
        mHandset.sendMessage(msg, reps);
        break;
    default:
        break;
    }
}

void JarvisDesk::sendMessage(const SerialMessage&& msg, uint8_t reps)
{
    sendMessage(msg, reps);
}

void JarvisDesk::wakeUp()
{
    sendMessage(SerialMessage(CommandFromHandsetType::Wake));
}

void JarvisDesk::requestAllSettings()
{
    sendMessage(SerialMessage(CommandFromHandsetType::GetUserLimits));
    processResponse(70);
    sendMessage(SerialMessage(CommandFromHandsetType::GetAbsLimits));
    processResponse(70);
    mControlbox.sendMessage(SerialMessage(CommandFromHandsetType::GetSettings));
    processResponse(70);
}

void JarvisDesk::setOffset(uint16_t offset)
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetOffset,
                              static_cast<uint16_t>(offset - 6)));
}

void JarvisDesk::setUnits(const char* value)
{
    UnitsValue v = UnitsValue::Unkown;
    if (std::strcmp(value, "inch") == 0)
    {
        v = UnitsValue::inch;
    }
    else if (std::strcmp(value, "cm") == 0)
    {
        v = UnitsValue::mm;
    }
    else
    {
        ESP_LOGW("TAG", "Unknown units value received: [%s]", value);
        return;
    }
    
    SerialMessage newcmd(CommandFromHandsetType::SetUnits);
    newcmd.setParam<UnitsValue>(v);
    sendMessage(newcmd);
    requestAllSettings();
}

void JarvisDesk::setTouchMode(const char* value)
{
    TouchModeValue v = TouchModeValue::Unkown;
    if (std::strcmp(value, "Single") == 0)
    {
        v = TouchModeValue::Single;
    }
    else if (std::strcmp(value, "Continuous") == 0)
    {
        v = TouchModeValue::Continuous;
    }
    else
    {
        ESP_LOGW("TAG", "Unknown touch mode value received: [%s]", value);
        return;
    }
    
    SerialMessage newcmd(CommandFromHandsetType::SetTouchMode);
    newcmd.setParam<TouchModeValue>(v);
    sendMessage(newcmd);
    requestAllSettings();
}

void JarvisDesk::setKillMode(const char* value)
{
    KillModeValue v = KillModeValue::Unkown;
    if (std::strcmp(value, "Kill") == 0)
    {
        v = KillModeValue::Kill;
    }
    else if (std::strcmp(value, "LetLive") == 0)
    {
        v = KillModeValue::LetLive;
    }
    else
    {
        ESP_LOGW("TAG", "Unknown kill mode value received: [%s]", value);
        return;
    }
    SerialMessage newcmd(CommandFromHandsetType::SetKillMode);
    newcmd.setParam<KillModeValue>(v);
    sendMessage(newcmd);
    requestAllSettings();
}

void JarvisDesk::setSensitivity(const char* value)
{
    SensitivityValue v = SensitivityValue::Unkown;
    if (std::strcmp(value, "High") == 0)
    {
        v = SensitivityValue::High;
    }
    else if (std::strcmp(value, "Medium") == 0)
    {
        v = SensitivityValue::Medium;
    }
    else if (std::strcmp(value, "Low") == 0)
    {
        v = SensitivityValue::Low;
    }
    else
    {
        ESP_LOGW("TAG", "Unknown sensitivity value received: [%s]", value);
        return;
    }
    SerialMessage newcmd(CommandFromHandsetType::SetSensitivity);
    newcmd.setParam<SensitivityValue>(v);
    sendMessage(newcmd);
    requestAllSettings();
}

void JarvisDesk::move(uint16_t height)
{
    // Hysteresis correction
    if (height > mHandset.getLastReportedHeight())
        height += 2;
    sendMessage(SerialMessage(CommandFromHandsetType::MoveTo, 
                              static_cast<uint16_t>(height)));
}

void JarvisDesk::goPreset1()
{
    sendMessage(SerialMessage(CommandFromHandsetType::MoveToPreset1));
}

void JarvisDesk::goPreset2()
{
    sendMessage(SerialMessage(CommandFromHandsetType::MoveToPreset2));
}

void JarvisDesk::goPreset3()
{
    sendMessage(SerialMessage(CommandFromHandsetType::MoveToPreset3));
}

void JarvisDesk::goPreset4()
{
    sendMessage(SerialMessage(CommandFromHandsetType::MoveToPreset4));
}

void JarvisDesk::setPreset1()
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetPreset1));
    requestAllSettings();
}

void JarvisDesk::setPreset2()
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetPreset2));
    requestAllSettings();
}

void JarvisDesk::setPreset3()
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetPreset3));
    requestAllSettings();
}

void JarvisDesk::setPreset4()
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetPreset4));
    requestAllSettings();
}

void JarvisDesk::setMaxHeight()
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetMaxHeight));
}

void JarvisDesk::setMinHeight()
{
    sendMessage(SerialMessage(CommandFromHandsetType::SetMinHeight));
}

void JarvisDesk::clearMaxHeight()
{
    sendMessage(SerialMessage(CommandFromHandsetType::ClearMinMax,
                              static_cast<uint8_t>(0x01)));
}

void JarvisDesk::clearMinHeight()
{
    sendMessage(SerialMessage(CommandFromHandsetType::ClearMinMax,
                              static_cast<uint8_t>(0x02)));
}
 
void JarvisDesk::setDisplayNumber(uint16_t val)
{
    mHandset.setDisplayValue(val);
}
 