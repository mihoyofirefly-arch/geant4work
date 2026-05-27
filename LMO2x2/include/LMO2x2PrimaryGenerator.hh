#ifndef LMO2x2PrimaryGenerator_hh
#define LMO2x2PrimaryGenerator_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"

class LMO2x2PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    LMO2x2PrimaryGenerator();
    ~LMO2x2PrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);

private:
    G4ParticleGun *fParticleGun;
};

#endif