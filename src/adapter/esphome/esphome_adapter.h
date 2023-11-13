#ifndef ESPHOME_ADAPTER_H
#define ESPHOME_ADAPTER_H

#include <esphome.h>
#include <cstdarg>

#include "logger.h"
#include "jarvis.h"
#include "callbacks.h"

using namespace esphome;

class EspHomeLoggerAdapter : public jlog::GenericLoggerIF
{
public:
    EspHomeLoggerAdapter();

    void logV(const char* format, va_list args) const override;
    void logD(const char* format, va_list args) const override;
    void logI(const char* format, va_list args) const override;
    void logW(const char* format, va_list args) const override;
    void logE(const char* format, va_list args) const override;
    void log(jlog::Level level, const char* format, va_list args) const override;

private:
    static constexpr char TAG[] = "Jarvis";
};

struct Sensors
{
    number::Number* height;
    sensor::Sensor* preset1;
    sensor::Sensor* preset2;
    sensor::Sensor* preset3;
    sensor::Sensor* preset4;
    sensor::Sensor* sysLimitMin;
    sensor::Sensor* sysLimitMax;
    sensor::Sensor* userLimitMin;
    sensor::Sensor* userLimitMax;
    text_sensor::TextSensor* userLimitSet;
    select::Select* units;
    select::Select* touchMode;
    select::Select* antiCollisionMode;
    select::Select* sensitivity;
};

struct Callbacks
{
    Callbacks(Sensors& sensors) 
        : heightCallback(sensors.height)
        , preset1Callback(sensors.preset1)
        , preset2Callback(sensors.preset2)
        , preset3Callback(sensors.preset3)
        , preset4Callback(sensors.preset4)
        , sysLimitMinCallback(sensors.sysLimitMin)
        , sysLimitMaxCallback(sensors.sysLimitMax)
        , userLimitMinCallback(sensors.userLimitMin)
        , userLimitMaxCallback(sensors.userLimitMax)
        , userLimitSetCallback(sensors.userLimitSet)
        , unitsCallback(sensors.units)
        , touchModeCallback(sensors.touchMode)
        , antiCollisionModeCallback(sensors.antiCollisionMode)
        , sensitivityCallback(sensors.sensitivity)
    {}

    HeightCallback heightCallback;
    Preset1Callback preset1Callback;
    Preset2Callback preset2Callback;
    Preset3Callback preset3Callback;
    Preset4Callback preset4Callback;
    SysLimitMinCallback sysLimitMinCallback;
    SysLimitMaxCallback sysLimitMaxCallback;
    UserLimitMinCallback userLimitMinCallback;
    UserLimitMaxCallback userLimitMaxCallback;
    UserLimitSetCallback userLimitSetCallback;
    UnitsCallback unitsCallback;
    TouchModeCallback touchModeCallback;
    AntiCollisionModeCallback antiCollisionModeCallback;
    SensitivityCallback sensitivityCallback;
};

struct EspHomeJarvisAdapter : public Component
{
   EspHomeJarvisAdapter(
        number::Number* height,
        sensor::Sensor* preset1,
        sensor::Sensor* preset2,
        sensor::Sensor* preset3,
        sensor::Sensor* preset4,
        sensor::Sensor* sysLimitMin,
        sensor::Sensor* sysLimitMax,
        sensor::Sensor* userLimitMin,
        sensor::Sensor* userLimitMax,
        text_sensor::TextSensor* userLimitSet,
        select::Select* units,
        select::Select* touchMode,
        select::Select* antiCollisionMode,
        select::Select* sensitivity
   );
    
    void setup() override;
    void loop() override;

    void setOffset(uint16_t offset);
    void moveTo(uint16_t height);
    void turnLedsOff();
    
    void setUnits(const char* value);
    void setTouchMode(const char* value);
    void setAntiCollisionMode(const char* value);
    void setSensitivity(const char* value);
    
    void moveToPreset1();
    void moveToPreset2();
    void moveToPreset3();
    void moveToPreset4();
    
    void setPreset1();
    void setPreset2();
    void setPreset3();
    void setPreset4();
    
    void setMaxHeight();    
    void setMinHeight();
    
    void clearMaxHeight();
    void clearMinHeight();
    
    void setDisplayNumber(uint16_t val);
    
private:
    EspHomeLoggerAdapter m_logger;
    Sensors m_sensors;
    Callbacks m_callbacks;
    Jarvis m_jarvis;
};




#endif // ESPHOME_ADAPTER_H
