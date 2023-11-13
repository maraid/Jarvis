#include "esphome_adapter.h"
#include "timer.h"

using namespace esphome;


EspHomeJarvisAdapter::EspHomeJarvisAdapter(
    number::Number*          height,
    sensor::Sensor*          preset1,
    sensor::Sensor*          preset2,
    sensor::Sensor*          preset3,
    sensor::Sensor*          preset4,
    sensor::Sensor*          sysLimitMin,
    sensor::Sensor*          sysLimitMax,
    sensor::Sensor*          userLimitMin,
    sensor::Sensor*          userLimitMax,
    text_sensor::TextSensor* userLimitSet,
    select::Select*          units,
    select::Select*          touchMode,
    select::Select*          antiCollisionMode,
    select::Select*          sensitivity
)
    : Component()
    , m_logger()
    , m_sensors({
        height,
        preset1,
        preset2,
        preset3,
        preset4,
        sysLimitMin,
        sysLimitMax,
        userLimitMin,
        userLimitMax,
        userLimitSet,
        units,
        touchMode,
        antiCollisionMode,
        sensitivity})
    , m_callbacks(m_sensors)
{
    m_jarvis.registerHeightCallback(&(m_callbacks.heightCallback));
    m_jarvis.registerPreset1Callback(&(m_callbacks.preset1Callback));
    m_jarvis.registerPreset2Callback(&(m_callbacks.preset2Callback));
    m_jarvis.registerPreset3Callback(&(m_callbacks.preset3Callback));
    m_jarvis.registerPreset4Callback(&(m_callbacks.preset4Callback));
    m_jarvis.registerUnitsCallback(&(m_callbacks.unitsCallback));
    m_jarvis.registerTouchModeCallback(&(m_callbacks.touchModeCallback));
    m_jarvis.registerAntiCollisionModeCallback(&(m_callbacks.antiCollisionModeCallback));
    m_jarvis.registerSensitivityCallback(&(m_callbacks.sensitivityCallback));
    m_jarvis.registerSysLimitMinCallback(&(m_callbacks.sysLimitMinCallback));
    m_jarvis.registerSysLimitMaxCallback(&(m_callbacks.sysLimitMaxCallback));
    m_jarvis.registerUserLimitSetCallback(&(m_callbacks.userLimitSetCallback));
    m_jarvis.registerUserLimitMinCallback(&(m_callbacks.userLimitMinCallback));
    m_jarvis.registerUserLimitMaxCallback(&(m_callbacks.userLimitMaxCallback));
}

void EspHomeJarvisAdapter::setup()
{
    m_jarvis.setup();
}

void EspHomeJarvisAdapter::loop()
{
    m_jarvis.loop();
}

void EspHomeJarvisAdapter::setOffset(uint16_t offset)
{
    m_jarvis.setOffset(offset);
}

void EspHomeJarvisAdapter::setUnits(const char* value)
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
        jlog::warning("Unknown units value received: [%s]", value);
        return;
    }
    
    m_jarvis.setUnits(v);
}

void EspHomeJarvisAdapter::setTouchMode(const char* value)
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
        jlog::warning("Unknown touch mode value received: [%s]", value);
        return;
    }
    
    m_jarvis.setTouchMode(v);
}

void EspHomeJarvisAdapter::setAntiCollisionMode(const char* value)
{
    AntiCollisionModeValue v = AntiCollisionModeValue::Unkown;
    if (std::strcmp(value, "Disabled") == 0)
    {
        v = AntiCollisionModeValue::Disabled;
    }
    else if (std::strcmp(value, "Enabled") == 0)
    {
        v = AntiCollisionModeValue::Enabled;
    }
    else
    {
        jlog::warning("Unknown anti-collision mode value received: [%s]", value);
        return;
    }
    m_jarvis.setAntiCollisionMode(v);
}

void EspHomeJarvisAdapter::setSensitivity(const char* value)
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
        jlog::warning("Unknown sensitivity value received: [%s]", value);
        return;
    }

    m_jarvis.setSensitivity(v);
}

void EspHomeJarvisAdapter::moveTo(uint16_t height)
{
    m_jarvis.moveTo(height);
}

void EspHomeJarvisAdapter::moveToPreset1()
{
    m_jarvis.moveToPreset1();
}

void EspHomeJarvisAdapter::moveToPreset2()
{
    m_jarvis.moveToPreset2();
}

void EspHomeJarvisAdapter::moveToPreset3()
{
    m_jarvis.moveToPreset3();
}

void EspHomeJarvisAdapter::moveToPreset4()
{
    m_jarvis.moveToPreset4();
}

void EspHomeJarvisAdapter::setPreset1()
{
    m_jarvis.setPreset1();
}

void EspHomeJarvisAdapter::setPreset2()
{
    m_jarvis.setPreset2();
}

void EspHomeJarvisAdapter::setPreset3()
{
    m_jarvis.setPreset3();
}

void EspHomeJarvisAdapter::setPreset4()
{
    m_jarvis.setPreset4();
}

void EspHomeJarvisAdapter::setMaxHeight()
{
    m_jarvis.setMaxHeight();
}

void EspHomeJarvisAdapter::setMinHeight()
{
    m_jarvis.setMinHeight();
}

void EspHomeJarvisAdapter::clearMaxHeight()
{
    m_jarvis.clearMaxHeight();
}

void EspHomeJarvisAdapter::clearMinHeight()
{
    m_jarvis.clearMinHeight();
}
 
void EspHomeJarvisAdapter::setDisplayNumber(uint16_t val)
{
    m_jarvis.setDisplayNumber(val);
}

EspHomeLoggerAdapter::EspHomeLoggerAdapter()
{
    jlog::logger = this;
}

void EspHomeLoggerAdapter::logV(const char* format, va_list args) const 
{ 
    char outbuf[128];
    vsprintf(outbuf, format, args);
    ESP_LOGV(EspHomeLoggerAdapter::TAG, "%s", outbuf);
}

void EspHomeLoggerAdapter::logD(const char* format, va_list args) const
{ 
    char outbuf[128];
    vsprintf(outbuf, format, args);
    ESP_LOGD(EspHomeLoggerAdapter::TAG, "%s", outbuf);
}

void EspHomeLoggerAdapter::logI(const char* format, va_list args) const
{ 
    char outbuf[128];
    vsprintf(outbuf, format, args);
    ESP_LOGI(EspHomeLoggerAdapter::TAG, "%s", outbuf);
}

void EspHomeLoggerAdapter::logW(const char* format, va_list args) const
{ 
    char outbuf[128];
    vsprintf(outbuf, format, args);
    ESP_LOGW(EspHomeLoggerAdapter::TAG, "%s", outbuf);
}

void EspHomeLoggerAdapter::logE(const char* format, va_list args) const
{ 
    char outbuf[128];
    vsprintf(outbuf, format, args);
    ESP_LOGE(EspHomeLoggerAdapter::TAG, "%s", outbuf);
}

void EspHomeLoggerAdapter::log(jlog::Level level, const char* format, va_list args) const
{
}
 