#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"//这是包含了G4VUserActionInitialization类的定义的头文件，G4VUserActionInitialization是Geant4提供的一个抽象基类，我们需要继承它来定义我们自己的动作初始化类，在这个类里我们会定义我们的用户动作，比如初级粒子产生、运行动作、事件动作等

#include "PMPrimaryGenerator.hh"
#include "PMRunAction.hh"

class PMActionInitialization : public G4VUserActionInitialization//这是定义了一个名为PMActionInitialization的类，它继承自G4VUserActionInitialization类，这意味着PMActionInitialization是一个用户动作初始化类，我们将在这个类中定义我们自己的用户动作，比如初级粒子产生、运行动作、事件动作等。
{
public:
    PMActionInitialization();
    ~PMActionInitialization();//这是PMActionInitialization类的构造函数和析构函数的声明，构造函数用于创建PMActionInitialization对象，析构函数用于销毁PMActionInitialization对象。

    virtual void BuildForMaster() const;//BuildForMaster() 主要用于 多线程模式
    virtual void Build() const;//Build() 是给工作线程 worker thread 或普通单线程运行使用的
};

#endif