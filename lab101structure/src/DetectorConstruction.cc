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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "CADMesh.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Exception.hh"
#include "G4ios.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

namespace B1
{

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto nist = G4NistManager::Instance();

    // =========================
    // 1. 创建 World
    // =========================
    auto worldMat = nist->FindOrBuildMaterial("G4_AIR");

    // 这里给大一点，确保你的 STL 能完整装进去
    G4double worldSize = 2.0 * m;

    auto solidWorld = new G4Box(
        "World",
        0.5 * worldSize,
        0.5 * worldSize,
        0.5 * worldSize
    );

    auto logicWorld = new G4LogicalVolume(
        solidWorld,
        worldMat,
        "World"
    );

    auto physWorld = new G4PVPlacement(
        nullptr,
        G4ThreeVector(),
        logicWorld,
        "World",
        nullptr,
        false,
        0,
        true
    );

    // =========================
    // 2. 读入 STL
    // =========================
    auto mesh = CADMesh::TessellatedMesh::FromSTL(
        "/home/chao/geant4/workdir/lab101structure/models/100mK.STL"
    );

    // 如果你在 SolidWorks 导出 STL 时单位就是 mm，这里通常就是 1.0
    mesh->SetScale(1.0);

    // 如果原点不对，可以先注释运行看看，再回来调这个
    mesh->SetOffset(G4ThreeVector(-205.0*mm, 0.0*mm, -205.0*mm));
    auto solid100mK = mesh->GetSolid();
    if (!solid100mK) {
        G4Exception("DetectorConstruction::Construct()",
                    "B1CAD001",
                    FatalException,
                    "Failed to create solid from STL.");
    }

    // =========================
    // 3. 给 STL 指定材料
    // =========================
    // 这里先示例用铝。你后面按真实结构改成铜、不锈钢等都行。
    auto partMat = nist->FindOrBuildMaterial("G4_Al");

    auto logic100mK = new G4LogicalVolume(
        solid100mK,
        partMat,
        "100mK_LV"
    );

    // =========================
    // 4. 可视化属性
    // =========================
    auto vis = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    vis->SetForceSolid(true);
    logic100mK->SetVisAttributes(vis);

    // =========================
    // 5. 放进 World
    // =========================
    auto rot = new G4RotationMatrix();

    // 如果方向不对，就慢慢打开下面几句试
    // rot->rotateX(90.0 * deg);
    // rot->rotateY(90.0 * deg);
    // rot->rotateZ(90.0 * deg);

    auto pos = G4ThreeVector(0.0 * mm, 0.0 * mm, 0.0 * mm);

    new G4PVPlacement(
        rot,
        pos,
        logic100mK,
        "100mK_PV",
        logicWorld,
        false,
        0,
        true
    );

    G4cout << "Only STL geometry loaded successfully." << G4endl;

    fScoringVolume = nullptr;

    return physWorld;
}

}  // namespace B1