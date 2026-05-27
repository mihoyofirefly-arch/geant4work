#ifndef LMO2x2PhysicsList_hh
#define LMO2x2PhysicsList_hh

#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4VModularPhysicsList.hh"

class LMO2x2PhysicsList : public G4VModularPhysicsList
{
public:
    LMO2x2PhysicsList();
    virtual ~LMO2x2PhysicsList();
};

#endif