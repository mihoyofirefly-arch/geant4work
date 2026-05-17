#include "PMPrimaryGenerator.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);//创建一个粒子枪对象，参数1表示每次调用GeneratePrimaryVertex()方法时产生一个粒子。

    // Particle position
    G4double x = 0. * cm;
    G4double y = 0. * cm;
    G4double z = -1. * cm;

    G4ThreeVector pos(x, y, z);//创建一个三维向量对象pos，表示粒子的位置，单位是厘米。

    // Particle direction//momentum
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 0.;

    G4ThreeVector mom(px, py, pz);//创建一个三维向量对象mom，表示粒子的动量方向，这里设置为0，表示粒子没有动量。

    // G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();//获取粒子表对象，粒子表包含了所有已知粒子的定义和属性。
    // G4ParticleDefinition *particle = particleTable->FindParticle("gamma");//通过粒子表的FindParticle方法查找一个粒子定义对象，这里查找的是质子。这个对象包含了质子的物理属性，如质量、寿命、衰变模式等。

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom); 
    // fParticleGun->SetParticleEnergy(1. * keV);
    // fParticleGun->SetParticleDefinition(fParticle);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)//每产生一个 event，Geant4 就调用一次这个函数。在这个函数里面，你告诉 Geant4：本次事件的初级粒子是什么、能量是多少、从哪里发射、方向是什么，然后把它加入当前事件 anEvent
{
    // Particle type
    G4int Z = 9;
    G4int A = 18;

    G4double charge = 0. * eplus;
    G4double energy = 0. * keV;

    G4ParticleDefinition *ion  = G4IonTable::GetIonTable()->GetIon(Z, A, energy);//通过G4IonTable类的GetIon方法获取一个离子粒子定义对象，这个对象包含了该离子的物理属性，如质量、寿命、衰变模式等。参数Z和A分别是离子的原子序数和质量数，energy是离子的激发能量，这里设置为0，表示基态离子。
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(charge);
    fParticleGun->SetParticleEnergy(energy);

    // Create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);//把初级粒子加进当前 event，anEvent 是一个指向 G4Event 对象的指针，代表当前事件。GeneratePrimaryVertex() 方法会根据之前设置的粒子属性，在事件中创建一个初级顶点，并把粒子加入到这个顶点中。
}