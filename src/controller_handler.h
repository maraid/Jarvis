#ifndef CONTROLLER_HANDLER_H
#define CONTROLLER_HANDLER_H

#include "serial_device.h"
#include "utils.h"
#include "timer.h"
#include "constants.h"

#include <SoftwareSerial.h>


class ControllerStates
{
public:
    void reset()
    {
        *this = ControllerStates();
    }

    uint16_t getPreset1() const { return m_preset1; }
    uint16_t getPreset2() const { return m_preset2; }
    uint16_t getPreset3() const { return m_preset3; }
    uint16_t getPreset4() const { return m_preset4; }
    UnitsValue getUnits() const { return m_units; }
    TouchModeValue getTouchMode() const { return m_touchMode; }
    AntiCollisionModeValue getAntiCollisionMode() const { return m_antiCollisionMode; }
    SensitivityValue getSensitivity() const { return m_sensitivity; }
    uint16_t getSysLimitMin() const { return m_sysLimitMin; }
    uint16_t getSysLimitMax() const { return m_sysLimitMax; }
    UserLimitSetValue getUserLimitSet() const { return m_userLimitSet; }
    uint16_t getUserLimitMin() const { return m_userLimitMin; }
    uint16_t getUserLimitMax() const { return m_userLimitMax; }

    virtual void setPreset1(uint16_t val) { m_preset1 = raw2Height(val); }
    virtual void setPreset2(uint16_t val) { m_preset2 = raw2Height(val); }
    virtual void setPreset3(uint16_t val) { m_preset3 = raw2Height(val); }
    virtual void setPreset4(uint16_t val) { m_preset4 = raw2Height(val); }
    virtual void setUnits(UnitsValue val) { m_units = val; }
    virtual void setTouchMode(TouchModeValue val) { m_touchMode = val; }
    virtual void setAntiCollisionMode(AntiCollisionModeValue val) { m_antiCollisionMode = val; }
    virtual void setSensitivity(SensitivityValue val) { m_sensitivity = val; }    
    virtual void setSysLimitMin(uint16_t val) { m_sysLimitMin = val; }
    virtual void setSysLimitMax(uint16_t val) { m_sysLimitMax = val; }
    virtual void setUserLimitSet(UserLimitSetValue val) { m_userLimitSet = val; }
    virtual void setUserLimitMin(uint16_t val) { m_userLimitMin = val; }
    virtual void setUserLimitMax(uint16_t val) { m_userLimitMax = val; }
    
    bool isSettingsComplete() const
    {
        return    (m_preset1 != UNKOWN_STATE)
               && (m_preset2 != UNKOWN_STATE)
               && (m_preset3 != UNKOWN_STATE)
               && (m_preset4 != UNKOWN_STATE)
               && (m_units != UnitsValue::Unkown)
               && (m_touchMode != TouchModeValue::Unkown)
               && (m_antiCollisionMode != AntiCollisionModeValue::Unkown)
               && (m_sensitivity != SensitivityValue::Unkown);
    }

    bool isSysLimitsComplete() const
    {
        return    (m_sysLimitMin != UNKOWN_STATE) 
               && (m_sysLimitMax != UNKOWN_STATE);
    }

    bool isUserLimitsComplete() const
    {
        return m_userLimitSet != UserLimitSetValue::Unkown;
    }

private:
    static constexpr uint16_t UNKOWN_STATE = 0xFFFF;

    uint16_t raw2Height(uint16_t raw)
    {   
        /* 
        /  Desk raw range:
        /  = sysLimitRawMax - sysLimitRawMin 
        /  = 0x3B80 - 0x13F5 = 10123
        /  
        /  Desk converted range:
        /  sysLimitMax - sysLimitMin = 649
        /  conversion = 649 / 10123 = 0.0641 mm
        /  height = sysLimitMin + (raw - sysLimitMin) * conversion
        */
        uint16_t height_mm = m_sysLimitMin + round((float)(raw - 0x13F5) * 0.0642f);
        return m_units == UnitsValue::mm ? height_mm : round((float)height_mm * 0.393f);
    }

    uint16_t m_preset1{UNKOWN_STATE};
    uint16_t m_preset2{UNKOWN_STATE};
    uint16_t m_preset3{UNKOWN_STATE};
    uint16_t m_preset4{UNKOWN_STATE};

    UnitsValue m_units{UnitsValue::Unkown};
    TouchModeValue m_touchMode{TouchModeValue::Unkown};
    AntiCollisionModeValue m_antiCollisionMode{AntiCollisionModeValue::Unkown};
    SensitivityValue m_sensitivity{SensitivityValue::Unkown};

    uint16_t m_sysLimitMin{UNKOWN_STATE};  // converted
    uint16_t m_sysLimitMax{UNKOWN_STATE};  // converted

    UserLimitSetValue m_userLimitSet{UserLimitSetValue::Unkown};
    uint16_t m_userLimitMin{UNKOWN_STATE};
    uint16_t m_userLimitMax{UNKOWN_STATE};
};


class ControllerHandler : public SoftwareSerialDevice
{
public:
    ControllerHandler();

    void setup() override;
    void loop() override;
    void handleMessage(const SerialMessage& msg) override;
    bool fetchMessage(SerialMessage& msg) override;

    void wakeUp();

    void setOffset(uint16_t offset);
    void setUnits(UnitsValue value);
    void setTouchMode(TouchModeValue value);
    void setAntiCollisionMode(AntiCollisionModeValue value);
    void setSensitivity(SensitivityValue value);
    void moveTo(uint16_t height);
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

    void registerHeightCallback(Callback<uint16_t>* callback);
    void registerPreset1Callback(Callback<uint16_t>* callback);
    void registerPreset2Callback(Callback<uint16_t>* callback);
    void registerPreset3Callback(Callback<uint16_t>* callback);
    void registerPreset4Callback(Callback<uint16_t>* callback);
    void registerUnitsCallback(Callback<UnitsValue>* callback);
    void registerTouchModeCallback(Callback<TouchModeValue>* callback);
    void registerAntiCollisionModeCallback(Callback<AntiCollisionModeValue>* callback);
    void registerSensitivityCallback(Callback<SensitivityValue>* callback);
    void registerSysLimitMinCallback(Callback<uint16_t>* callback);
    void registerSysLimitMaxCallback(Callback<uint16_t>* callback);
    void registerUserLimitSetCallback(Callback<UserLimitSetValue>* callback);
    void registerUserLimitMinCallback(Callback<uint16_t>* callback);
    void registerUserLimitMaxCallback(Callback<uint16_t>* callback);

private:
    void saveState(const SerialMessage& msg);
    void requestUserLimits();
    void requestSystemLimits();
    void requestSettings();

    template<typename PARAM_T>
    void call(Callback<PARAM_T>* callback, PARAM_T param) const
    {
        if (callback != nullptr)
        {
            (*callback)(param);
        }
    }

    ControllerStates m_states;
    Timer m_stateRequestTimer;

    Callback<uint16_t>* p_heightCallback{nullptr};
    Callback<uint16_t>* p_preset1Callback{nullptr};
    Callback<uint16_t>* p_preset2Callback{nullptr};
    Callback<uint16_t>* p_preset3Callback{nullptr};
    Callback<uint16_t>* p_preset4Callback{nullptr};
    Callback<uint16_t>* p_sysLimitMinCallback{nullptr};
    Callback<uint16_t>* p_sysLimitMaxCallback{nullptr};
    Callback<uint16_t>* p_userLimitMinCallback{nullptr};
    Callback<uint16_t>* p_userLimitMaxCallback{nullptr};
    Callback<UserLimitSetValue>* p_userLimitSetCallback{nullptr};
    Callback<UnitsValue>* p_unitsCallback{nullptr};
    Callback<TouchModeValue>* p_touchModeCallback{nullptr};
    Callback<AntiCollisionModeValue>* p_antiCollisionModeCallback{nullptr};
    Callback<SensitivityValue>* p_sensitivityCallback{nullptr};
};



#endif

