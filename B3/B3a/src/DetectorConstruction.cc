//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B3/B3a/src/DetectorConstruction.cc
/// \brief Implementation of the B3::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4NistManager.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

namespace B3
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  DefineMaterials();
}//构造函数

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()//定义材料
{
  G4NistManager* man = G4NistManager::Instance();//获取材料管理

  G4bool isotopes = false;//是否使用同位素

  G4Element* O = man->FindOrBuildElement("O", isotopes);//获取元素
  G4Element* Si = man->FindOrBuildElement("Si", isotopes);//获取元素
  G4Element* Lu = man->FindOrBuildElement("Lu", isotopes);//获取元素

  auto LSO = new G4Material("Lu2SiO5", 7.4 * g / cm3, 3);//创建材料
  LSO->AddElement(Lu, 2);
  LSO->AddElement(Si, 1);
  LSO->AddElement(O, 5);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()//构建几何结构
{
  // Gamma detector Parameters
  //
  G4double cryst_dX = 6 * cm, cryst_dY = 6 * cm, cryst_dZ = 3 * cm;//晶体尺寸
  G4int nb_cryst = 32;//32块晶体
  G4int nb_rings = 9;//9个环
  //
  G4double dPhi = twopi / nb_cryst, half_dPhi = 0.5 * dPhi;//每块晶体占的角度的角宽以及定义每块晶体占的角度的一半
  G4double cosdPhi = std::cos(half_dPhi);//每个晶体占的角度的一半的余弦值
  G4double tandPhi = std::tan(half_dPhi);//每个晶体占的角度的一半的正切值
  //
  G4double ring_R1 = 0.5 * cryst_dY / tandPhi;//环的内半径
  G4double ring_R2 = (ring_R1 + cryst_dZ) / cosdPhi;//环的外半径
  //
  G4double detector_dZ = nb_rings * cryst_dX;
  //
  G4NistManager* nist = G4NistManager::Instance();//获取材料管理器
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");//默认材料是空气
  G4Material* cryst_mat = nist->FindOrBuildMaterial("Lu2SiO5");//晶体材料是LuSiO

  //
  // World
  //
  G4double world_sizeXY = 2.4 * ring_R2;
  G4double world_sizeZ = 1.2 * detector_dZ;

  auto solidWorld =
    new G4Box("World",  // its name
              0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size//G4Box的参数是半长不是全长

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        default_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     fCheckOverlaps);  // checking overlaps

  //
  // ring
  //
  auto solidRing = new G4Tubs("Ring", ring_R1, ring_R2, 0.5 * cryst_dX, 0., twopi);//名字，内半径，外半径，z方向半长，起始角，扫过角度

  auto logicRing = new G4LogicalVolume(solidRing,  // its solid
                                       default_mat,  // its material
                                       "Ring");  // its name

  //
  // define crystal
  //
  G4double gap = 0.5 * mm;  // a gap for wrapping//晶体之间的间隙，实际尺寸小于环单元占用的理想尺寸
  G4double dX = cryst_dX - gap, dY = cryst_dY - gap;//何意味减个gap？
  auto solidCryst = new G4Box("crystal", dX / 2, dY / 2, cryst_dZ / 2);//定义了单个晶体

  auto logicCryst = new G4LogicalVolume(solidCryst,  // its solid
                                        cryst_mat,  // its material
                                        "CrystalLV");  // its name

  // place crystals within a ring
  //
  for (G4int icrys = 0; icrys < nb_cryst; icrys++) { //循环32次
    G4double phi = icrys * dPhi;//第i块晶体对应的角度
    G4RotationMatrix rotm = G4RotationMatrix();//定义一个旋转矩阵
    rotm.rotateY(90 * deg);//绕Y轴旋转90度
    rotm.rotateZ(phi);//绕Z轴旋转phi度
    G4ThreeVector uz = G4ThreeVector(std::cos(phi), std::sin(phi), 0.);//定义一个单位向量，平面内径向方向
    G4ThreeVector position = (ring_R1 + 0.5 * cryst_dZ) * uz;//晶体中心的位置
    G4Transform3D transform = G4Transform3D(rotm, position);//定义一个变换，包含旋转和平移

    new G4PVPlacement(transform,  // rotation,position//放置晶体 
                      logicCryst,  // its logical volume
                      "crystal",  // its name
                      logicRing,  // its mother  volume
                      false,  // no boolean operation
                      icrys,  // copy number
                      fCheckOverlaps);  // checking overlaps
  }

  //
  // full detector
  //
  auto solidDetector = new G4Tubs("Detector", ring_R1, ring_R2, 0.5 * detector_dZ, 0., twopi);

  auto logicDetector = new G4LogicalVolume(solidDetector,  // its solid
                                           default_mat,  // its material
                                           "Detector");  // its name

  //
  // place rings within detector
  //
  G4double OG = -0.5 * (detector_dZ + cryst_dX);
  for (G4int iring = 0; iring < nb_rings; iring++) {
    OG += cryst_dX;
    new G4PVPlacement(nullptr,  // no rotation
                      G4ThreeVector(0, 0, OG),  // position
                      logicRing,  // its logical volume
                      "ring",  // its name
                      logicDetector,  // its mother  volume
                      false,  // no boolean operation
                      iring,  // copy number
                      fCheckOverlaps);  // checking overlaps
  }

  //
  // place detector in world
  //
  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicDetector,  // its logical volume
                    "Detector",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps

  //
  // patient
  //
  G4double patient_radius = 8 * cm;
  G4double patient_dZ = 10 * cm;
  G4Material* patient_mat = nist->FindOrBuildMaterial("G4_BRAIN_ICRP");

  auto solidPatient = new G4Tubs("Patient", 0., patient_radius, 0.5 * patient_dZ, 0., twopi);

  auto logicPatient = new G4LogicalVolume(solidPatient,  // its solid
                                          patient_mat,  // its material
                                          "PatientLV");  // its name

  //
  // place patient in world
  //
  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicPatient,  // its logical volume
                    "Patient",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    fCheckOverlaps);  // checking overlaps

  // Visualization attributes
  //
  logicRing->SetVisAttributes(G4VisAttributes::GetInvisible());
  logicDetector->SetVisAttributes(G4VisAttributes::GetInvisible());

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()//定义探测器和场
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // declare crystal as a MultiFunctionalDetector scorer
  //
  auto cryst = new G4MultiFunctionalDetector("crystal");
  G4SDManager::GetSDMpointer()->AddNewDetector(cryst);
  G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");
  cryst->RegisterPrimitive(primitiv1);
  SetSensitiveDetector("CrystalLV", cryst);

  // declare patient as a MultiFunctionalDetector scorer
  //
  auto patient = new G4MultiFunctionalDetector("patient");
  G4SDManager::GetSDMpointer()->AddNewDetector(patient);
  G4VPrimitiveScorer* primitiv2 = new G4PSDoseDeposit("dose");
  patient->RegisterPrimitive(primitiv2);
  SetSensitiveDetector("PatientLV", patient);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B3
