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
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4VSolid.hh"

#include <array>
#include <cmath>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace B1
{

namespace
{

  constexpr G4double kEps = 0.02 * mm;

  struct BlindHoleSpec
  {
    G4double x;
    G4double y;
    G4double diameter;
    G4double depth;
    bool fromPlusZ;
  };

  struct ThroughHoleSpec
  {
    G4double x;
    G4double y;
    G4double diameter;
  };

  struct CounterboreSpec
  {
    G4double x;
    G4double y;
    G4double throughD;
    G4double plusFaceD;
    G4double plusFaceDepth;
    G4double minusFaceD;
    G4double minusFaceDepth;
  };

  struct PocketSpec
  {
    G4double x;
    G4double y;
    G4double r1;
    G4double r2;
    G4double armRadius;
    G4double depth;
    bool fromPlusZ;
    std::array<G4double, 3> armAnglesDeg;
  };

  struct ColdPlateDims
  {
    // ===== 来自图纸且基本可确认的主尺寸 =====
    G4double outerDiameter         = 850.0 * mm;
    G4double totalThickness        = 28.0  * mm;
    G4double globalReliefDiameter  = 680.0 * mm;
    G4double globalReliefDepth     = 3.0   * mm;

    // ===== 主大孔 — 根据 sheet 3 / section A-A 高保真抽象 =====
    // 左右两个最大孔
    G4double bigPairCenterX        = 230.0 * mm;
    G4double bigPairThroughD       = 160.0 * mm;
    G4double bigPairPlusFaceD      = 190.0 * mm;
    G4double bigPairPlusFaceDepth  = 9.8   * mm;   // 28 - 18.2
    G4double bigPairMinusFaceD     = 200.0 * mm;
    G4double bigPairMinusFaceDepth = 3.0   * mm;

    // 上下两个中孔
    G4double axialPairCenterY      = 145.0 * mm;   // 图面可读关系 + 视觉校核
    G4double axialPairThroughD     = 102.0 * mm;
    G4double axialPairPlusFaceD    = 130.0 * mm;
    G4double axialPairPlusFaceDepth= 9.8   * mm;
    G4double axialPairMinusFaceD   = 145.0 * mm;   // 由另一面图形近似
    G4double axialPairMinusDepth   = 3.0   * mm;

    // 四个对角中孔
    G4double diagPairCenter        = 133.0 * mm;   // 由图像几何比例反推
    G4double diagPairThroughD      = 102.0 * mm;
    G4double diagPairPlusFaceD     = 130.0 * mm;
    G4double diagPairPlusFaceDepth = 9.8   * mm;
    G4double diagPairMinusFaceD    = 145.0 * mm;
    G4double diagPairMinusDepth    = 3.0   * mm;

    // 中心孔
    G4double centerThroughD        = 60.0  * mm;   // 中心孔在 PNG 中不够清晰，等效近似
    G4double centerPlusFaceD       = 102.0 * mm;
    G4double centerPlusFaceDepth   = 9.8   * mm;
    G4double centerMinusFaceD      = 102.0 * mm;
    G4double centerMinusDepth      = 3.0   * mm;

    // ===== 外圈孔系 =====
    G4double outerM8PCD            = 740.0 * mm;
    G4int    outerM8Count          = 12;
    G4double outerM8NominalD       = 8.0   * mm;
    G4double outerM8Depth          = 10.0  * mm;
    G4double outerM8StartAngle     = 15.0  * deg;

    G4double outerM8x4PCD          = 820.0 * mm;
    G4int    outerM8x4Count        = 4;
    G4double outerM8x4NominalD     = 8.0   * mm;
    G4double outerM8x4Depth        = 16.0  * mm;
    G4double outerM8x4StartAngle   = 45.0  * deg;

    // 12x Ø12 贯穿孔（由 sheet 2 可见）
    G4double outerD12PCD           = 800.0 * mm;
    G4int    outerD12Count         = 12;
    G4double outerD12              = 12.0  * mm;
    G4double outerD12StartAngle    = 0.0   * deg;

    // ===== 近中心小孔 =====
    // 这些孔位来自 sheet 7 / 8；这里只放入最清晰、最关键的一批
    G4double m6BlindDepth          = 10.0 * mm;
    G4double m5BlindDepth          = 10.0 * mm;
    G4double m10BlindDepth         = 10.0 * mm;

    // ===== 局部 relief pocket =====
    // A-F 口袋在 PNG 中可见，但边界圆角控制半径不全；这里做高保真近似
    G4double pocketDepthShallow    = 1.5 * mm;
    G4double pocketDepthMedium     = 3.0 * mm;
  };

  inline G4Transform3D MakeTransform(const G4ThreeVector& t, G4double rotZ = 0.0)
  {
    G4RotationMatrix r;
    if (rotZ != 0.0) r.rotateZ(rotZ);
    return G4Transform3D(r, t);
  }

  inline void AddThroughTube(G4MultiUnion* mu,
                             const G4String& name,
                             G4double plateT,
                             const ThroughHoleSpec& h)
  {
    auto* s = new G4Tubs(name,
                         0.0,
                         0.5 * h.diameter + kEps,
                         0.5 * plateT + 2.0 * kEps,
                         0.0,
                         360.0 * deg);

    mu->AddNode(*s, MakeTransform(G4ThreeVector(h.x, h.y, 0.0)));
  }

  inline void AddBlindTube(G4MultiUnion* mu,
                           const G4String& name,
                           G4double plateT,
                           const BlindHoleSpec& h)
  {
    const G4double hz = 0.5 * h.depth + kEps;
    const G4double zc = h.fromPlusZ
                      ? (+0.5 * plateT - 0.5 * h.depth + 0.5 * kEps)
                      : (-0.5 * plateT + 0.5 * h.depth - 0.5 * kEps);

    auto* s = new G4Tubs(name,
                         0.0,
                         0.5 * h.diameter + kEps,
                         hz,
                         0.0,
                         360.0 * deg);

    mu->AddNode(*s, MakeTransform(G4ThreeVector(h.x, h.y, zc)));
  }

  inline void AddCounterboredHole(G4MultiUnion* mu,
                                  const G4String& baseName,
                                  G4double plateT,
                                  const CounterboreSpec& c)
  {
    // 贯穿芯孔
    AddThroughTube(mu, baseName + "_through", plateT, {c.x, c.y, c.throughD});

    // +Z 面台阶
    if (c.plusFaceD > c.throughD && c.plusFaceDepth > 0.0)
    {
      AddBlindTube(mu, baseName + "_plus",
                   plateT,
                   {c.x, c.y, c.plusFaceD, c.plusFaceDepth, true});
    }

    // -Z 面台阶
    if (c.minusFaceD > c.throughD && c.minusFaceDepth > 0.0)
    {
      AddBlindTube(mu, baseName + "_minus",
                   plateT,
                   {c.x, c.y, c.minusFaceD, c.minusFaceDepth, false});
    }
  }

  inline G4VSolid* MakeTriLobePocketSolid(const G4String& name,
                                          G4double r1,
                                          G4double r2,
                                          G4double armRadius,
                                          G4double depth,
                                          const std::array<G4double,3>& armAnglesDeg)
  {
    auto* core = new G4Tubs(name + "_core", 0.0, r1, 0.5 * depth + kEps, 0.0, 360.0 * deg);
    G4VSolid* pocket = core;

    for (std::size_t i = 0; i < armAnglesDeg.size(); ++i)
    {
      const G4double a = armAnglesDeg[i] * deg;
      const G4double x = armRadius * std::cos(a);
      const G4double y = armRadius * std::sin(a);
      auto* arm = new G4Tubs(name + "_arm_" + std::to_string(i),
                             0.0, r2, 0.5 * depth + kEps, 0.0, 360.0 * deg);
      pocket = new G4UnionSolid(name + "_u_" + std::to_string(i),
                                pocket, arm, nullptr, G4ThreeVector(x, y, 0.0));
    }

    return pocket;
  }

  inline void AddTriLobePocket(G4MultiUnion* mu,
                               const G4String& name,
                               G4double plateT,
                               const PocketSpec& p)
  {
    auto* s = MakeTriLobePocketSolid(name, p.r1, p.r2, p.armRadius, p.depth, p.armAnglesDeg);

    const G4double zc = p.fromPlusZ
                      ? (+0.5 * plateT - 0.5 * p.depth + 0.5 * kEps)
                      : (-0.5 * plateT + 0.5 * p.depth - 0.5 * kEps);

    mu->AddNode(*s, MakeTransform(G4ThreeVector(p.x, p.y, zc)));
  }

  inline std::vector<G4ThreeVector> RingPoints(G4double pcd,
                                               G4int n,
                                               G4double startPhi)
  {
    std::vector<G4ThreeVector> pts;
    pts.reserve(n);
    const G4double r = 0.5 * pcd;
    const G4double dphi = 360.0 * deg / n;
    for (G4int i = 0; i < n; ++i)
    {
      const G4double phi = startPhi + i * dphi;
      pts.emplace_back(r * std::cos(phi), r * std::sin(phi), 0.0);
    }
    return pts;
  }

  G4ThreeVector GetAsciiStlCenter(const G4String& path)
  {
    std::ifstream input(path);
    if (!input) {
      G4Exception("DetectorConstruction::GetAsciiStlCenter",
                  "StlOpenError",
                  FatalException,
                  ("Cannot open STL file: " + path).c_str());
    }

    G4double minX = std::numeric_limits<G4double>::max();
    G4double minY = std::numeric_limits<G4double>::max();
    G4double minZ = std::numeric_limits<G4double>::max();
    G4double maxX = std::numeric_limits<G4double>::lowest();
    G4double maxY = std::numeric_limits<G4double>::lowest();
    G4double maxZ = std::numeric_limits<G4double>::lowest();

    G4String token;
    G4bool foundVertex = false;
    while (input >> token) {
      if (token != "vertex") continue;

      G4double x = 0.0;
      G4double y = 0.0;
      G4double z = 0.0;
      input >> x >> y >> z;

      minX = std::min(minX, x);
      minY = std::min(minY, y);
      minZ = std::min(minZ, z);
      maxX = std::max(maxX, x);
      maxY = std::max(maxY, y);
      maxZ = std::max(maxZ, z);
      foundVertex = true;
    }

    if (!foundVertex) {
      G4Exception("DetectorConstruction::GetAsciiStlCenter",
                  "StlVertexError",
                  FatalException,
                  ("No vertices found in STL file: " + path).c_str());
    }

    return G4ThreeVector(0.5 * (minX + maxX),
                         0.5 * (minY + maxY),
                         0.5 * (minZ + maxZ));
  }
}

DetectorConstruction::DetectorConstruction() = default;
DetectorConstruction::~DetectorConstruction() = default;

G4LogicalVolume* DetectorConstruction::GetScoringVolume() const
{
  return fScoringVolume;
}

void DetectorConstruction::DefineMaterials()
{
  auto* nist = G4NistManager::Instance();

  fWorldMat = nist->FindOrBuildMaterial("G4_AIR");

  // 默认先用铝；如果你的冷盘实际是 OFHC 铜，只需改成 "G4_Cu"
  // 例如：fPlateMat = nist->FindOrBuildMaterial("G4_Cu");
  fPlateMat = nist->FindOrBuildMaterial("G4_Al");
}

G4VSolid* DetectorConstruction::BuildColdPlateSolid()
{
  const G4String stlPath = "models/Al.STL";
  const auto stlCenter = GetAsciiStlCenter(stlPath);

  auto mesh = CADMesh::TessellatedMesh::FromSTL(stlPath);
  mesh->SetScale(mm);
  mesh->SetOffset(-stlCenter * mm);

  return mesh->GetSolid();
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();

  // World
  auto* worldSolid = new G4Box("World", 4000.0*mm, 4000.0*mm, 4000.0*mm);
  auto* worldLV    = new G4LogicalVolume(worldSolid, fWorldMat, "WorldLV");
  auto* worldPV    = new G4PVPlacement(nullptr,
                                       G4ThreeVector(),
                                       worldLV,
                                       "WorldPV",
                                       nullptr,
                                       false,
                                       0,
                                       true);

  // Cold plate (single overall part)
  auto* plateSolid = BuildColdPlateSolid();
  auto* plateLV    = new G4LogicalVolume(plateSolid, fPlateMat, "ColdPlateLV");
  fScoringVolume   = plateLV;

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    plateLV,
                    "ColdPlatePV",
                    worldLV,
                    false,
                    0,
                    true);

  // Visualisation
  auto* worldVis = new G4VisAttributes();
  worldVis->SetVisibility(false);
  worldLV->SetVisAttributes(worldVis);

  auto* plateVis = new G4VisAttributes(G4Colour(0.55, 0.58, 0.63, 1.0));
  plateVis->SetForceSolid(true);
  plateLV->SetVisAttributes(plateVis);

  return worldPV;
}
}
