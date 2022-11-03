#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/Mux.hpp>
#include <eeros/control/D.hpp>
#include "customBlocks/FwKinOdom.hpp"
//#include "customBlocks/Controller.hpp"
//#include "customBlocks/InvMotMod.hpp"
#include <eeros/control/DeMux.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/SocketData.hpp>

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    PeripheralInput<> E1, E2;
    Mux<2> E;
    D<eeros::math::Vector2> Ed;
    FwKinOdom fwKinOdom;
    TimeDomain timedomain;
    //added
    Mux<3> odo;
    DeMux<2> pos;
    Mux<2> velo_vector_Rframe;
    SocketData<eeros::math::Vector3, double> socket;
};

#endif // CONTROLSYSTEM_HPP