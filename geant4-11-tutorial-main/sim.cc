#include <iostream>//we will use this for debugging purposes, to print out some messages to the console

#include "G4RunManager.hh"//这些都是头文件，包含了我们需要用到的类和函数的定义，可能根据不同的需要会添加不同的include
#include "G4MTRunManager.hh"//如果你的这些头文件有红线报错，按Ctrl + Shift + P，选择C++: Edit Configurations (UI)，在包含路径里添加自己的路径，比如我这里是/home/chao/geant4/install/include/Geant4。一定要在${workspaceFolder}/**下方添加
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "PMPhysicsList.hh"
#include "PMDetectorConstruction.hh"
#include "PMActionInitialization.hh"

int main(int argc, char** argv)//这是主函数，是程序的入口点，argc和argv是命令行参数的数量和内容，比如./simulation run.mac，argc == 2，argv[0] == "./simulation"，argv[1] == "run.mac"
{
    G4UIExecutive *ui = nullptr;//这是一个指向G4UIExecutive类的指针，G4UIExecutive是Geant4提供的一个类，用来管理用户界面，如果用户没有提供命令行参数，我们就创建一个UI界面，让用户可以在界面里输入命令来控制模拟的运行，比如执行宏文件、设置参数等

    #ifdef G4MULTITHREADED//这是一个预处理指令，检查是否定义了G4MULTITHREADED，如果定义了，就使用多线程的运行管理器，否则就使用单线程的运行管理器
        G4MTRunManager *runManager = new G4MTRunManager;
    #else
        G4RunManager *runManager = new G4RunManager;
    #endif

    // Physics list
    runManager->SetUserInitialization(new PMPhysicsList());//这是设置物理列表的代码，PMPhysicsList是我们自己定义的一个类，继承自G4VModularPhysicsList，在这个类里我们会添加我们需要的物理过程，比如电磁相互作用、强相互作用等

    // Detector construction
    runManager->SetUserInitialization(new PMDetectorConstruction());//这是设置探测器构造的代码，PMDetectorConstruction是我们自己定义的一个类，继承自G4VUserDetectorConstruction，在这个类里我们会定义我们的探测器的几何形状、材料、位置等

    // Action initialization
    runManager->SetUserInitialization(new PMActionInitialization());//这是设置动作初始化的代码，PMActionInitialization是我们自己定义的一个类，继承自G4VUserActionInitialization，在这个类里我们会定义我们的用户动作，比如初级粒子产生、运行动作、事件动作等

    if (argc == 1)//这一步是创建ui
    {
        ui = new G4UIExecutive(argc, argv);//如果用户没有提供命令行参数，就创建一个UI界面，让用户可以在界面里输入命令来控制模拟的运行，比如执行宏文件、设置参数等
    }

    G4VisManager *visManager = new G4VisExecutive();//这是创建一个可视化管理器的代码，G4VisExecutive是Geant4提供的一个默认的可视化管理器，它支持多种可视化驱动，比如OpenGL、RayTracer等，我们可以根据需要选择不同的驱动来显示我们的模拟结果
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();//这是获取UI管理器的代码，UI管理器是用来执行用户命令的，我们可以通过它来执行一些预定义的命令，比如执行宏文件、设置参数等

    if(ui)//这一步是创建好ui后的命令
    {
        UImanager->ApplyCommand("/control/execute vis.mac");//如果用户没有提供命令行参数，就执行一个默认的可视化宏文件，这个文件里可能包含一些设置可视化参数的命令，比如选择可视化驱动、设置颜色等,这里是加载vis.mac文件
        ui->SessionStart();//然后启动UI界面，让用户可以在界面里输入命令来控制模拟的运行，比如执行宏文件、设置参数等
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);//如果用户提供了命令行参数，就执行用户提供的宏文件，这个文件里可能包含一些设置模拟参数的命令，比如设置粒子类型、能量、运行次数等,这里是加载用户提供的宏文件，比如run.mac，如果运行时输入了./simulation run.mac，那么argv[1]就是"run.mac"，我们就执行/run.mac文件里的命令
    }

    return 0;
}