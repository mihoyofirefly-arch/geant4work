#include "PMRunAction.hh"

PMRunAction::PMRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();//依旧获取分析管理器。G4AnalysisManager 负责管理直方图，Ntuple 表格，ROOT / CSV / HDF5 / XML 输出文件。

    analysisManager->CreateH1("Edep", "Energy deposit", 100, 0., 1.1 * MeV);//创建一个直方图，CreateH1(名字, 标题, bin数量, 下限, 上限);创建一个名字叫 Edep 的一维直方图；标题是 Energy deposit；分成 100 个 bin；横轴范围从 0 到 1.1 MeV。

    analysisManager->CreateNtuple("Photons", "Photons");//创建一个 ntuple，名字叫Photons，标题也叫Photons
    analysisManager->CreateNtupleIColumn("iEvent");//在 ntuple 里创建一个整数列，名字叫 iEvent，用来存储事件编号
    analysisManager->CreateNtupleDColumn("fX");//记录x光子位置//对应analysisManager->FillNtupleDColumn(0, 1, posPhoton[0]);（在ProcessHits()里）
    analysisManager->CreateNtupleDColumn("fY");//记录y光子位置//对应analysisManager->FillNtupleDColumn(0, 2, posPhoton[1]);
    analysisManager->CreateNtupleDColumn("fZ");//记录z光子位置//对应analysisManager->FillNtupleDColumn(0, 3, posPhoton[2]);
    analysisManager->CreateNtupleDColumn("fGlobalTime");//记录光子全局时间//对应analysisManager->FillNtupleDColumn(0, 4, fGlobalTime);
    analysisManager->CreateNtupleDColumn("fWlen");//记录光子波长//对应analysisManager->FillNtupleDColumn(0, 5, fWlen);
    analysisManager->FinishNtuple(0);//ntuple 的列已经定义完了，可以后续往里面填数据了，FinishNtuple(0) 里的 0 是 ntuple 的 ID，表示我们刚才创建的这个 ntuple 的 ID 是 0，以后如果我们创建了更多的 ntuple，它们的 ID 就会依次增加，1, 2, 3...我们在 ProcessHits() 里往 ntuple 填数据的时候，就需要指定填哪个 ntuple，比如 analysisManager->FillNtupleDColumn(0, 1, posPhoton[0]); 就是往 ID 为 0 的 ntuple 的第 1 列填入 posPhoton[0] 的值。
}

PMRunAction::~PMRunAction()
{
}

void PMRunAction::BeginOfRunAction(const G4Run *run)//每当一个 run 开始时，Geant4 就会调用一次这个函数。在这个函数里，我们通常会打开一个输出文件，准备好写入数据。
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();//依旧获取分析管理器

    G4int runID = run->GetRunID();//这里从 G4Run 对象里取得当前 run 的 ID。这个 ID 是由 Geant4 自动分配的，通常是从 0 开始递增的整数，表示这是第几个 run。我们可以用这个 ID 来区分不同 run 的输出文件，比如 output0.root, output1.root 等等，这样就不会把不同 run 的数据写到同一个文件里了。

    std::stringstream strRunID;
    strRunID << runID;//这两行是在把整数 runID 转成字符串。

    analysisManager->OpenFile("output" + strRunID.str() + ".root");//这行是打开一个输出文件，文件名是 output + runID + .root，比如 output0.root, output1.root 等等。这个文件就是用来存储我们在 PMRunAction 里创建的直方图和 ntuple 的数据的。当我们往直方图和 ntuple 填数据的时候，这些数据就会被写入到这个文件里，最后我们可以用 ROOT 来打开这个文件，查看和分析这些数据。
}

void PMRunAction::EndOfRunAction(const G4Run *run)//每当一个 run 结束时，Geant4 就会调用一次这个函数。在这个函数里，我们通常会把数据写入文件，然后关闭文件。
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();//依旧获取分析管理器

    analysisManager->Write();//把数据写入文件。这个操作会把我们在 PMRunAction 里创建的直方图和 ntuple 的数据写入到之前打开的那个 output + runID + .root 文件里。

    analysisManager->CloseFile();//关闭当前输出文件

    G4int runID = run->GetRunID();//这行从 G4Run 对象里取当前 run 的编号。第一次执行/run/beamOn 1000，runID = 0。第二次执行/run/beamOn 1000，runID = 1。每次执行/run/beamOn，runID 就会增加 1。

    G4cout << "Finishing run " << runID << G4endl;//在终端输出当前 run 的编号，表示这个 run 已经结束了。
}