#ifndef LMO2x2RunAction_hh
#define LMO2x2RunAction_hh

#include "G4UserRunAction.hh"

class LMO2x2RunAction : public G4UserRunAction
{
public:
    LMO2x2RunAction();
    virtual ~LMO2x2RunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
};

#endif 