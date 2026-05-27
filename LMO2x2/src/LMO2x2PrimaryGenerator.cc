#include "LMO2x2PrimaryGenerator.hh"

LMO2x2PrimaryGenerator::LMO2x2PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    G4double x = 0. * cm;
    G4double y = -110. * cm;
    G4double z = 0. * cm;

    G4ThreeVector pos(x, y, z);

    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 0.;

    G4ThreeVector mom(px, py, pz);

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom); 
    fParticleGun->SetParticleEnergy(661.657 * keV);
    fParticleGun->SetParticleDefinition(particle);
}

LMO2x2PrimaryGenerator::~LMO2x2PrimaryGenerator()
{
    delete fParticleGun;
}

