#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "AutMobRoSSafetyProperties.hpp"
//#include "LidarSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <eeros/sequencer/Wait.hpp>

class MainSequence : public eeros::sequencer::Sequence
{
public:

    MainSequence(std::string name, eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 AutMobRoSSafetyProperties &sp, ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),

          sleep("Sleep", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        while (eeros::sequencer::Sequencer::running)
        {
            sleep(1.0);
            //log.info() << "ici: " << cs.fwKinOdom.getOutGrR().getSignal();
            //log.info() << cs.fwKinOdom.getOutPhi().getSignal();
            log.info() << "current omegaR " << cs.fwKinOdom.RJW.getOut(2).getSignal();
            log.info() << "current RvRx " << cs.fwKinOdom.RJW.getOut(0).getSignal();
            //log.info() << cs.myGain.getOut().getSignal();

        }
        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    AutMobRoSSafetyProperties &sp;

    eeros::sequencer::Wait sleep;
};

#endif // MAINSEQUENCE_HPP_