#include "LMO2x2PhysicsList.hh"

LMO2x2PhysicsList::LMO2x2PhysicsList()
{
    
    RegisterPhysics(new G4EmStandardPhysics());

    RegisterPhysics(new G4RadioactiveDecayPhysics());

    RegisterPhysics(new G4DecayPhysics());
}

LMO2x2PhysicsList::~LMO2x2PhysicsList()
{
}