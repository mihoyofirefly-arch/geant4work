#ifndef PMPHYSICSLISTS_HH
#define PMPHYSICSLISTS_HH

#include "G4VModularPhysicsList.hh"//这是包含了G4VModularPhysicsList类的定义的头文件，G4VModularPhysicsList是Geant4提供的一个抽象基类，我们需要继承它来定义我们自己的物理列表类，在这个类里我们会添加我们需要的物理过程，比如电磁相互作用、强相互作用等
#include "G4EmStandardPhysics.hh"//这是包含了G4EmStandardPhysics类的定义的头文件，G4EmStandardPhysics是Geant4提供的一个物理构造器类，它包含了电磁相互作用的物理过程，我们可以通过它来添加电磁相互作用到我们的物理列表里
#include "G4RadioactiveDecayPhysics.hh"//这是包含了G4RadioactiveDecayPhysics类的定义的头文件，G4RadioactiveDecayPhysics是Geant4提供的一个物理构造器类，它包含了放射性衰变的物理过程，我们可以通过它来添加放射性衰变到我们的物理列表里
#include "G4DecayPhysics.hh"//这是包含了G4DecayPhysics类的定义的头文件，G4DecayPhysics是Geant4提供的一个物理构造器类，它包含了粒子衰变的物理过程，我们可以通过它来添加粒子衰变到我们的物理列表里

class PMPhysicsList : public G4VModularPhysicsList//这是定义了一个名为PMPhysicsList的类，它继承自G4VModularPhysicsList，我们将在这个类里添加我们需要的物理过程，比如电磁相互作用、强相互作用等
{
public:
    PMPhysicsList();
    ~PMPhysicsList();//这是PMPhysicsList类的构造函数和析构函数的声明，我们将在PMPhysicsList.cc文件里定义它们，在构造函数里我们会添加我们需要的物理过程，比如电磁相互作用、强相互作用等
};

#endif