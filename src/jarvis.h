#ifndef JARVIS_H
#define JARVIS_H

#include "handset_handler.h"
#include "controller_handler.h"
#include "serial_message.h"
#include "timer.h"
#include "utils.h"


class Jarvis
{
public:
    virtual void setup();
    virtual void loop();

    // Controller interface
    void setOffset(uint16_t value);
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
    
    // Handset interface
    void setDisplayNumber(uint16_t val);

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
    void handleHandset();
    void handleController();

    HandsetHandler m_handset;
    ControllerHandler m_controller;
};

#endif
