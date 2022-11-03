#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : E1("enc1"),
      E2("enc2"),
      fwKinOdom(0.15),
      timedomain("Main time domain", dt, true),
      //added
      socket("", 9876, dt)
      
      {
    // Name all blocks
    E1.setName("E1");
    E2.setName("E2");
    E.setName("E");
    Ed.setName("Ed");
    fwKinOdom.setName("fwKinOdom");
    

    // Name all signals
    E1.getOut().getSignal().setName("q1 [m]");
    E2.getOut().getSignal().setName("q2 [m]");
    E.getOut().getSignal().setName("q [m]");
    E.getOut().getSignal().setName("qd [m/s]");

    // Connect signals
    E.getIn(0).connect(E1.getOut());
    E.getIn(1).connect(E2.getOut());
    Ed.getIn().connect(E.getOut());
    fwKinOdom.getIn().connect(Ed.getOut());
    //added multiplex
    pos.getIn().connect(fwKinOdom.getOutGrR());
    odo.getIn(0).connect(pos.getOut(0));
    odo.getIn(1).connect(pos.getOut(1));
    odo.getIn(2).connect(fwKinOdom.getOutPhi());
    socket.getIn().connect(odo.getOut());
    

    //socket.getIn(0).connect(fwKinOdom.getOutGvR());//Vector2>& global robot velocity
    //socket.getIn().connect(fwKinOdom.getOutGrR());//Vector2>& global robot position
    //socket.getIn(1).connect(fwKinOdom.getOutPhi());//global robot orientation
    //socket.getIn(3).connect(fwKinOdom.getOutOmegaR());//global robot angular velocity

    // Add blocks to timedomain
    timedomain.addBlock(E1);
    timedomain.addBlock(E2);
    timedomain.addBlock(E);
    timedomain.addBlock(Ed);
    timedomain.addBlock(fwKinOdom);
    //added
    timedomain.addBlock(pos);
    timedomain.addBlock(odo);
    timedomain.addBlock(socket);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}