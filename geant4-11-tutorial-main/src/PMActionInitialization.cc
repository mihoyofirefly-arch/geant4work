#include "PMActionInitialization.hh"

PMActionInitialization::PMActionInitialization()
{}

PMActionInitialization::~PMActionInitialization()
{}

void PMActionInitialization::BuildForMaster() const
{
    PMRunAction *runAction = new PMRunAction();
    SetUserAction(runAction);
}

void PMActionInitialization::Build() const
{
    PMPrimaryGenerator *generator = new PMPrimaryGenerator();
    SetUserAction(generator);//把刚才创建的 PMPrimaryGenerator 注册给 Geant4,Geant4 会用你的 PMPrimaryGenerator 来产生初级粒子

    PMRunAction *runAction = new PMRunAction();
    SetUserAction(runAction);//这个对应在PMRunAction *runAction = new PMRunAction();里，这个是在创建 PMRunAction 对象时执行的
}