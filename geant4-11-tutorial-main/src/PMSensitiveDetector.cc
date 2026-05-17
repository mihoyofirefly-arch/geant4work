#include "PMSensitiveDetector.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
}

PMSensitiveDetector::~PMSensitiveDetector()
{
}

void PMSensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;//每个事件开始时，初始化总能量沉积为0，只统计第 n 个 event 自己的能量沉积，而不会把前一个 event 的能量带过来。
}

G4bool PMSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)//每当粒子在敏感探测器体积中产生一个 step，Geant4 就调用一次这个函数。这个函数会记录该 step 的事件编号、位置、时间、动量/波长，并把这些信息写入 ntuple；同时累加该 step 的能量沉积。
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();//这行取得当前事件编号

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();//取得分析管理器

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();//取得 step 的起点信息

    G4double fGlobalTime = preStepPoint->GetGlobalTime();
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();//记录时间、位置、动量

    G4double fMomPhotonMag = momPhoton.mag();//计算动量大小

    G4double fWlen = (1.239841939 * eV / fMomPhotonMag) * 1E+03;//计算波长

    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleDColumn(0, 1, posPhoton[0]);
    analysisManager->FillNtupleDColumn(0, 2, posPhoton[1]);
    analysisManager->FillNtupleDColumn(0, 3, posPhoton[2]);
    analysisManager->FillNtupleDColumn(0, 4, fGlobalTime);
    analysisManager->FillNtupleDColumn(0, 5, fWlen);//把事件编号、位置、时间、波长填入 ntuple ,每调用一次 ProcessHits()，就往 ntuple 里增加一行：eventID, x, y, z, time, wavelength
    analysisManager->AddNtupleRow(0);//当前这些 column 填完了，把它们作为 ntuple 的一整行保存下来。//这些填数据的操作，都需要在PMRunAction先创建好对应的 histogram 和 ntuple。

    G4double energyDeposited = aStep->GetTotalEnergyDeposit();//这行取得当前 step 在敏感体积中的总能量沉积

    if (energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;//如果当前 step 的能量沉积大于0，就把它累加到 fTotalEnergyDeposited 里，这样在事件结束时，fTotalEnergyDeposited 就是该事件在敏感体积中的总能量沉积。
    }

    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();//取得分析管理器

    analysisManager->FillH1(0, fTotalEnergyDeposited);//把能量沉积填入一维直方图

    G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;//在终端输出当前 event 的总能量沉积
}
