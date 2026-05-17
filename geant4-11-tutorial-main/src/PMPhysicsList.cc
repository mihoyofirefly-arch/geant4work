#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()//这是PMPhysicsList类的构造函数的定义，在这个函数里我们会添加我们需要的物理过程，比如电磁相互作用、强相互作用等
{
    // EM Physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Radioactive decay physics
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // Decay physics
    RegisterPhysics(new G4DecayPhysics());
}

PMPhysicsList::~PMPhysicsList()//这是PMPhysicsList类的析构函数的定义，在这个函数里我们可以做一些清理工作，比如删除我们在构造函数里创建的物理过程对象等
{
}