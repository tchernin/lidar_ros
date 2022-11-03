#ifndef LidarSAFETYPROPERTIES_HPP_
#define LidarSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class LidarSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    LidarSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSwitchingOn;
    eeros::safety::SafetyEvent doSystemOn;
    eeros::safety::SafetyEvent doMotorPowerOn;
    eeros::safety::SafetyEvent doMoving;
    eeros::safety::SafetyEvent doBraking;
    eeros::safety::SafetyEvent doMotorPowerOff;
    eeros::safety::SafetyEvent doEmergencyBraking;
    eeros::safety::SafetyEvent doEmergency;
    eeros::safety::SafetyEvent doReleaseEmergency;
    eeros::safety::SafetyEvent doWheelsAtRest;
    eeros::safety::SafetyEvent doSystemOff;
    eeros::safety::SafetyEvent doSwitchingOff;

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSwitchedOff;
    eeros::safety::SafetyLevel slWheelAtRest;
    eeros::safety::SafetyLevel slSwitchedOn;
    eeros::safety::SafetyLevel slEmergency;
    eeros::safety::SafetyLevel slEmergencyBraking;
    eeros::safety::SafetyLevel slSystemOn;
    eeros::safety::SafetyLevel slBraking;
    eeros::safety::SafetyLevel slMotorPowerOff;
    eeros::safety::SafetyLevel slMotorPowerOn;
    eeros::safety::SafetyLevel slMoving;

private:
    // Define all critical outputs
    eeros::hal::Output<bool>* greenLED;
    eeros::hal::Output<bool>* redLED;

    // Define all critical inputs
    eeros::hal::Input<bool>* buttonPause;
    eeros::hal::Input<bool>* buttonMode;

    ControlSystem &cs;
};

#endif // LidarSAFETYPROPERTIES_HPP_
