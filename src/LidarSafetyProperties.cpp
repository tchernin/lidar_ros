#include "LidarSafetyProperties.hpp"

LidarSafetyProperties::LidarSafetyProperties(ControlSystem &cs, double dt)
        : cs(cs),
    
      slSystemOff("System is offline"),
      slSwitchedOff("System switching on"),
      slWheelAtRest("Check if the wheels are at rest"),
      slSwitchedOn("System switching off"),
      slEmergency("Emergency "),
      slEmergencyBraking("Emergency Braking"),
      slSystemOn("System is online"),
      slMotorPowerOff("Motor powered off"),
      slMotorPowerOn("Motor powered on"),
      slBraking("Braking"),
      slMoving("Moving"),
      

      doSwitchingOn("Do Switching On"),
      doSystemOn("Do system on"),
      doMotorPowerOn("Do Motor powered on"),
      doMoving("Do Moving"),
      doBraking("Do Braking"),
      doMotorPowerOff("Do Motor powered off"),
      doEmergencyBraking("Do Emergency Braking"),
      doEmergency("Do Emergency"),
      doReleaseEmergency("Do Release Emergency"),
      doWheelsAtRest("Do check if the wheels are at rest"),
      doSystemOff("Do Shutdown"),
      doSwitchingOff("Do Switching off")
    
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    greenLED = hal.getLogicOutput("onBoardLEDgreen");
    redLED = hal.getLogicOutput("onBoardLEDred");

    criticalOutputs = { greenLED, redLED };

    // Declare and add critical inputs
    buttonPause = eeros::hal::HAL::instance().getLogicInput("onBoardButtonPause");
    buttonMode = eeros::hal::HAL::instance().getLogicInput("onBoardButtonMode");

    criticalInputs = { buttonPause, buttonMode };

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slSwitchedOff);
    addLevel(slWheelAtRest);
    addLevel(slSwitchedOn);
    addLevel(slEmergency);
    addLevel(slEmergencyBraking);
    addLevel(slSystemOn);
    addLevel(slMotorPowerOff);
    addLevel(slBraking);
    addLevel(slMotorPowerOn);
    addLevel(slMoving);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSwitchingOn, slSwitchedOn, kPublicEvent);
    slSwitchedOn.addEvent(doSystemOn, slSystemOn, kPrivateEvent);
    slSystemOn.addEvent(doMotorPowerOn, slMotorPowerOn, kPublicEvent);
    slMotorPowerOn.addEvent(doMoving, slMoving, kPublicEvent);
    slMoving.addEvent(doBraking, slBraking, kPublicEvent);
    slBraking.addEvent(doMotorPowerOff, slMotorPowerOff, kPublicEvent);
    slEmergencyBraking.addEvent(doEmergency, slEmergency, kPublicEvent);
    slEmergencyBraking.addEvent(doReleaseEmergency, slSystemOn, kPublicEvent);
    slEmergency.addEvent(doReleaseEmergency, slSystemOn, kPublicEvent);
    slWheelAtRest.addEvent(doSwitchingOff, slSwitchedOff, kPublicEvent);
    slSwitchedOff.addEvent(doSwitchingOff, slSystemOff, kPrivateEvent);

     

    // Add events to multiple safety levels
    addEventToAllLevelsBetween(slEmergency, slMoving, doWheelsAtRest, slWheelAtRest, kPublicEvent);
    addEventToAllLevelsBetween(slSystemOn, slMoving, doEmergencyBraking, slEmergencyBraking, kPublicEvent);

    // Define input actions for all levels
    slSystemOff.setInputActions({           ignore(buttonPause),                    ignore(buttonMode) });
    slSwitchedOff.setInputActions({        ignore(buttonPause),                    ignore(buttonMode) });
    slWheelAtRest.setInputActions({        ignore(buttonPause),                    ignore(buttonMode) });
    slSwitchedOn.setInputActions({             ignore(buttonPause),                    ignore(buttonMode) });
    slEmergency.setInputActions({          check(buttonPause, false, doReleaseEmergency),  ignore(buttonMode) });
    slEmergencyBraking.setInputActions({         check(buttonPause, false, doReleaseEmergency),                    ignore(buttonMode) });
    slSystemOn.setInputActions({           ignore(buttonPause),                  check(buttonMode, false, doEmergencyBraking)  });
    slBraking.setInputActions({           ignore(buttonPause),                  check(buttonMode, false, doEmergencyBraking)  });
    slMotorPowerOff.setInputActions({    ignore(buttonPause),                    check(buttonMode, false, doEmergencyBraking)  });
    slMotorPowerOn.setInputActions({           ignore(buttonPause),                  check(buttonMode, false, doEmergencyBraking)  });
    slMoving.setInputActions({           ignore(buttonPause),                  check(buttonMode, false, doEmergencyBraking)  });

    // Define output actions for all levels
    slSystemOff.setOutputActions({           set(greenLED, false),   set(redLED, false) });
    slSwitchedOff.setOutputActions({        set(greenLED, false),   set(redLED, true) });
    slWheelAtRest.setOutputActions({        set(greenLED, false),   set(redLED, true) });
    slSwitchedOn.setOutputActions({             set(greenLED, true),   set(redLED, false) });
    slEmergency.setOutputActions({          set(greenLED, true),    set(redLED, true) });
    slEmergencyBraking.setOutputActions({          set(greenLED, true),    set(redLED, true) });
    slSystemOn.setOutputActions({           set(greenLED, true),    set(redLED, false) });
    slBraking.setOutputActions({          set(greenLED, false),    set(redLED, true) });
    slMotorPowerOff.setOutputActions({    set(greenLED, false),    set(redLED, true) });
    slMotorPowerOn.setOutputActions({            set(greenLED, true),    set(redLED, false) });
    slMoving.setOutputActions({        set(greenLED, true),    set(redLED, false) });

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        eeros::Executor::stop();
    });



    slSwitchedOff.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        privateContext->triggerEvent(doSwitchingOn);
    });


    slSwitchedOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
        if (slSystemOn.getNofActivations()*dt >= 1)   // wait 1 sec
        {
            privateContext->triggerEvent(doSystemOn);
        }
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doMotorPowerOn); //??? How do you do multiple actions?
        //privateContext->triggerEvent(doWheelsAtRest);
        //privateContext->triggerEvent(doEmergencyBraking);??How to make the system wait after braking?
         
    });


    slMotorPowerOn.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slSystemOn.getNofActivations()*dt >= 1)   // wait 1 sec
        {
            privateContext->triggerEvent(doMoving);
        }
        //privateContext->triggerEvent(doWheelsAtRest);
        //privateContext->triggerEvent(doEmergencyBraking);    
    });


    slMoving.setLevelAction([&, dt](SafetyContext *privateContext) {
        //if (cs.method){

        //}
        privateContext->triggerEvent(doBraking);
        //privateContext->triggerEvent(doWheelsAtRest);
        //privateContext->triggerEvent(doEmergencyBraking); 
    });

    slBraking.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slMoving.getNofActivations()*dt >= 5)   // wait 5 sec
        {
            privateContext->triggerEvent(doMotorPowerOff);
        }
        //privateContext->triggerEvent(doWheelsAtRest);
        //privateContext->triggerEvent(doEmergencyBraking); 
    });

    slMotorPowerOff.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slBraking.getNofActivations()*dt >= 5)   // wait 5 sec
        {
            privateContext->triggerEvent(doMotorPowerOn);
        }
        //privateContext->triggerEvent(doWheelsAtRest);
        //privateContext->triggerEvent(doEmergencyBraking); 
    });

    slEmergencyBraking.setLevelAction([&, dt](SafetyContext *privateContext) {
        privateContext->triggerEvent(doEmergency);
        //privateContext->triggerEvent(doWheelsAtRest);
    });     


    slEmergency.setLevelAction([&, dt](SafetyContext *privateContext) {
        privateContext->triggerEvent(doReleaseEmergency);
        privateContext->triggerEvent(doWheelsAtRest);  
    });


    slWheelAtRest.setLevelAction([&, dt](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSwitchingOff);
    });


    slSwitchedOff.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slSystemOn.getNofActivations()*dt >= 2)   // wait 2 sec
        {
            privateContext->triggerEvent(doSystemOff);
        }    
    });


    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSwitchingOff);
    });
}