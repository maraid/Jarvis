#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <esphome.h>

#include "utils.h"

template<typename SENSOR_T, typename PARAM_T>
class EspCallback : public Callback<PARAM_T>
{
public:
    EspCallback(SENSOR_T* sensor)
        : m_sensor(sensor)
    {
    }
    
    void operator()(PARAM_T param) override
    {
        if (m_sensor != nullptr)
        {
            m_sensor->publish_state(valToString(param));
        }
    }

private:
    SENSOR_T* m_sensor;
};

template<typename SENSOR_T>
class EspCallback<SENSOR_T, uint16_t> : public Callback<uint16_t>
{
public:
    EspCallback(SENSOR_T* sensor)
        : m_sensor(sensor)
    {
    }
    
    void operator()(uint16_t param) override
    {
        if (m_sensor != nullptr)
        {
            m_sensor->publish_state(param);
        }
    }

private:
    SENSOR_T* m_sensor;
};


using HeightCallback            = EspCallback<esphome::number::Number, uint16_t>;
using Preset1Callback           = EspCallback<esphome::sensor::Sensor, uint16_t>;
using Preset2Callback           = EspCallback<esphome::sensor::Sensor, uint16_t>;
using Preset3Callback           = EspCallback<esphome::sensor::Sensor, uint16_t>;
using Preset4Callback           = EspCallback<esphome::sensor::Sensor, uint16_t>;
using UnitsCallback             = EspCallback<esphome::select::Select, UnitsValue>;
using TouchModeCallback         = EspCallback<esphome::select::Select, TouchModeValue>;
using AntiCollisionModeCallback = EspCallback<esphome::select::Select, AntiCollisionModeValue>;
using SensitivityCallback       = EspCallback<esphome::select::Select, SensitivityValue>;
using SysLimitMinCallback       = EspCallback<esphome::sensor::Sensor, uint16_t>;
using SysLimitMaxCallback       = EspCallback<esphome::sensor::Sensor, uint16_t>;
using UserLimitSetCallback      = EspCallback<esphome::text_sensor::TextSensor, UserLimitSetValue>;
using UserLimitMinCallback      = EspCallback<esphome::sensor::Sensor, uint16_t>;
using UserLimitMaxCallback      = EspCallback<esphome::sensor::Sensor, uint16_t>;

#endif
