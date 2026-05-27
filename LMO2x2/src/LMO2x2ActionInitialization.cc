#include "LMO2x2ActionInitialization.hh"


LMO2x2ActionInitialization::LMO2x2ActionInitialization()
{}

LMO2x2ActionInitialization::~LMO2x2ActionInitialization()
{}

void LMO2x2ActionInitialization::BuildForMaster() const
{
    LMO2x2RunAction *runAction = new LMO2x2RunAction();
    SetUserAction(runAction);
}

void LMO2x2ActionInitialization::Build() const
{
    LMO2x2PrimaryGenerator *generator = new LMO2x2PrimaryGenerator();
    SetUserAction(generator);

    LMO2x2RunAction *runAction = new LMO2x2RunAction();
    SetUserAction(runAction);
}