#ifndef JARVISDESK_H
#define JARVISDESK_H

#include "HandsetHandler.h"
#include "ControlboxHandler.h"
#include "Timer.h"
#include "esphome.h"

using namespace esphome;

enum LedState
{
    On,
    Off
};

// const HandsetMode defaultHandsetMode = HandsetMode::Factory;
// const LedState defaultLedState = LedState::On;

struct Settings
{
    Settings() 
        : presetRaw1(0),
        presetRaw2(0),
        presetRaw3(0),
        presetRaw4(0),
        units(UnitsValue::Unkown),
        touchMode(TouchModeValue::Unkown),
        killMode(KillModeValue::Unkown),
        sensitivity(SensitivityValue::Unkown),
        sysLimitMin(0),
        sysLimitMax(0),
        userLimitSet(UserLimitSetValue::Unkown),
        userLimitMin(0),
        userLimitMax(0)
    {}

    void reset()
    {
        *this = Settings();
    }

    uint16_t raw2Height(uint16_t raw)
    {   
        /* 
        /  Desk raw range:
        /  = sysLimitRawMax - sysLimitRawMin 
        /  = 0x3B80 - 0x13F5 = 10123
        /  
        /  Desk converted range:
        /  = sysLimitMax - sysLimitMin = 649
        /  = conversion = 649 / 10123 = 0.0641 mm
        /  height = sysLimitMin + (raw - sysLimitMin) * conversion
        */
        uint16_t height_mm = sysLimitMin + round((float)(raw - 0x13F5) * 0.0642f);
        return units == UnitsValue::mm ? height_mm : round((float)height_mm * 0.393f);
    }

    uint16_t getPreset1() { return raw2Height(presetRaw1); }
    uint16_t getPreset2() { return raw2Height(presetRaw2); }
    uint16_t getPreset3() { return raw2Height(presetRaw3); }
    uint16_t getPreset4() { return raw2Height(presetRaw4); }

    // Conventional
    uint16_t presetRaw1;
    uint16_t presetRaw2;
    uint16_t presetRaw3;
    uint16_t presetRaw4;

    UnitsValue units;
    TouchModeValue touchMode;
    KillModeValue killMode;
    SensitivityValue sensitivity;

    // Extended
    uint16_t sysLimitMin;  // converted
    uint16_t sysLimitMax;  // converted

    UserLimitSetValue userLimitSet;
    uint16_t userLimitMin;
    uint16_t userLimitMax;

    operator bool() const
    {
        return presetRaw1
               && presetRaw2
               && presetRaw3
               && presetRaw4
               && units != UnitsValue::Unkown
               && touchMode != TouchModeValue::Unkown
               && killMode != KillModeValue::Unkown
               && sensitivity != SensitivityValue::Unkown
               && sysLimitMin
               && sysLimitMax
               && userLimitSet != UserLimitSetValue::Unkown
               && userLimitMin
               && userLimitMax;
    }
    
    /*
    String toString()
    {
        return "preset1: " + String(getPreset1()) + "\n"
               + "preset2: " + String(getPreset2()) + "\n"
               + "preset3: " + String(getPreset3()) + "\n"
               + "preset4: " + String(getPreset4()) + "\n"
               + "units: " + valToString(units) + "\n"
               + "touchMode: " + valToString(touchMode) + "\n"
               + "killMode: " + valToString(killMode) + "\n"
               + "sensitivity: " + valToString(sensitivity) + "\n"
               + "sysLimitMin: " + String(sysLimitMin) + "\n"
               + "sysLimitMax: " + String(sysLimitMax) + "\n"
               + "userLimitSet: " + valToString(userLimitSet) + "\n"
               + "userLimitMin: " + String(userLimitMin) + "\n"
               + "userLimitMax: " + String(userLimitMax);
    } */
};


class JarvisDesk : public Component
{
public:
    JarvisDesk();

    void setup() override;
    void loop() override;

    void handleHandset();
    void handleControlbox();

    void processResponse(uint32_t duration=100);

    void sendMessage(const SerialMessage& msg, uint8_t reps = 1);
    void sendMessage(const SerialMessage&& msg, uint8_t reps = 1);

    void wakeUp();
    void requestAllSettings();
    void requestSystemLimits();
    void extractSetting(const SerialMessage& msg);
    void requestUserLimits();
    void setOffset(uint16_t offset);
    void move(uint16_t height);
    void turnLedsOff();
    
    void setUnits(const char* value);
    void setTouchMode(const char* value);
    void setKillMode(const char* value);
    void setSensitivity(const char* value);
    
    void goPreset1();
    void goPreset2();
    void goPreset3();
    void goPreset4();
    
    void setPreset1();
    void setPreset2();
    void setPreset3();
    void setPreset4();
    
    void setMaxHeight();    
    void setMinHeight();
    
    void clearMaxHeight();
    void clearMinHeight();
    
    void setDisplayNumber(uint16_t val);
    
    sensor::Sensor* sPreset1;
    sensor::Sensor* sPreset2;
    sensor::Sensor* sPreset3;
    sensor::Sensor* sPreset4;
    sensor::Sensor* sSysLimitMin;
    sensor::Sensor* sSysLimitMax;
    sensor::Sensor* sUserLimitMin;
    sensor::Sensor* sUserLimitMax;
    
    text_sensor::TextSensor* sUserLimitSet;
    
    select::Select* sUnits;
    select::Select* sTouchMode;
    select::Select* sKillMode;
    select::Select* sSensitivity;
    
    number::Number* sHeight;
    
private:
    Settings mSettings;

    HandsetHandler mHandset;
    ControlboxHandler mControlbox;
};

#endif   // JARVISDESK_H