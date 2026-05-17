#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"//初级粒子产生动作的基类
#include "G4ParticleDefinition.hh"//定义粒子类型的类
#include "G4ParticleGun.hh"//粒子枪类，用于产生初级粒子
#include "G4ParticleTable.hh"//粒子表类，用于获取粒子定义
#include "G4SystemOfUnits.hh"//Geant4的单位系统头文件，允许我们使用诸如mm、cm、MeV等单位来定义物理量
#include "G4IonTable.hh"//离子表类，用于获取离子定义

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PMPrimaryGenerator();//构造函数，通常在这里初始化一些成员变量或者设置默认值。
    ~PMPrimaryGenerator();//析构函数，负责清理资源，释放内存等。

    virtual void GeneratePrimaries(G4Event *);//这个函数是纯虚函数，必须在子类中实现，它负责在每个事件开始时产生初级粒子，参数是一个指向G4Event对象的指针，代表当前事件。写了这个指针就不需要G4Event.hh了，因为G4Event.hh里定义了G4Event类，而这个函数的参数已经声明了G4Event*，编译器就知道G4Event是一个类了，不需要再包含G4Event.hh了。

private:
    G4ParticleGun *fParticleGun;//这是一个指向G4ParticleGun对象的指针，G4ParticleGun是Geant4提供的一个类，用于产生初级粒子，我们将在构造函数中创建这个对象，并在GeneratePrimaries函数中使用它来设置粒子的属性和产生粒子。
};

#endif