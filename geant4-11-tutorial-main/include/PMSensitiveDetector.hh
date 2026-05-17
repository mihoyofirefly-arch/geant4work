#ifndef PMSENSITIVEDETECTOR_HH
#define PMSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class PMSensitiveDetector : public G4VSensitiveDetector
{
public:
    PMSensitiveDetector(G4String);//构造函数，接受一个字符串参数，通常是敏感探测器的名称，这个名称会被用来在 Geant4 的敏感探测器管理器中注册这个探测器，以便在模拟过程中识别和使用它。
    ~PMSensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);//每当粒子在敏感体积中走了一步，Geant4 就会调用一次 ProcessHits(),如果你把某个逻辑体设置成 sensitive detector，Geant4 就会在每个事件中跟踪粒子在这个体积中的每一步，并调用 ProcessHits() 方法来处理这些步骤。你可以在这个方法里记录粒子与敏感体积的交互信息，比如能量沉积、位置、时间等，然后把这些信息存储到一个 hits collection 中，以便后续分析。

    virtual void Initialize(G4HCofThisEvent*) override;//
    virtual void EndOfEvent(G4HCofThisEvent *) override;

    G4double fTotalEnergyDeposited;
};

#endif