#ifndef LMO2x2ActionInitialization_hh
#define LMO2x2ActionInitialization_hh

#include "G4VUserActionInitialization.hh"
#include "LMO2x2PrimaryGenerator.hh"
#include "LMO2x2RunAction.hh"

class LMO2x2ActionInitialization : public G4VUserActionInitialization
{
public:
    LMO2x2ActionInitialization();
    virtual ~LMO2x2ActionInitialization();  

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif