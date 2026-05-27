#include "LMO2x2DetectorConstruction.hh"

LMO2x2DetectorConstruction::LMO2x2DetectorConstruction()
{}

LMO2x2DetectorConstruction::~LMO2x2DetectorConstruction()
{}

G4VPhysicalVolume* LMO2x2DetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;
// Define Mo100,MoNat element
G4int zMo = 42;
G4int aMo100 = 100;

auto* Mo100_iso = new G4Isotope(
    "Mo100",
    zMo,
    aMo100,
    99.9075 * g / mole
);

auto* MoNat_iso = new G4Isotope(
    "MoNatRest",
    42,
    98,
    97.0 * g / mole
);

auto* MoEnriched = new G4Element(
    "EnrichedMolybdenum",
    "MoEnr",
    2
);

MoEnriched->AddIsotope(Mo100_iso, 97.0 * perCent);
MoEnriched->AddIsotope(MoNat_iso, 3.0 * perCent);


// Define LMO
G4NistManager *nist  = G4NistManager::Instance();
auto* Li = nist->FindOrBuildElement("Li");
auto* O  = nist->FindOrBuildElement("O");

G4double lmoDensity = 3.07 * g / cm3;
auto* LMO = new G4Material(
    "LMO",
    lmoDensity,
    3
);
LMO->AddElement(Li, 1);
LMO->AddElement(MoEnriched, 1);
LMO->AddElement(O, 3);

// Define air world
auto* air = nist->FindOrBuildMaterial("G4_AIR");
G4double worldSize = 3 * m;
G4Box* airWorld = new G4Box("airWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);

G4LogicalVolume* airLogic = new G4LogicalVolume(airWorld, air, "airLogic");

G4VPhysicalVolume* airPhys = new G4PVPlacement(
    nullptr,
    G4ThreeVector(),
    airLogic,
    "airPhys",
    nullptr,
    false,
    0,
    checkOverlaps
);

// Define Vacuum world

// 实验中的真空度
G4double mbar = 1.0e-3 * bar;
G4double vacuumPressure = 1.0e-5 * mbar;

//实验中的温度
G4double vacuumTemperature = 1.0e-3 * kelvin;

// 参考状态：空气在 1 atm, 273.15 K 的密度
G4double referencePressure = atmosphere;
G4double referenceTemperature = 273.15 * kelvin;
G4double referenceDensity = 1.29e-3 * g / cm3;

G4double vacuumDensity =
    referenceDensity
    * (vacuumPressure / referencePressure)
    * (referenceTemperature / vacuumTemperature);

auto* vacuum = new G4Material(
    "Vacuum_lab",
    7.0,
    14.0 * g / mole,
    vacuumDensity,
    kStateGas,
    vacuumTemperature,
    vacuumPressure
);
    G4double vacuumSizeX = 50.0 * cm;
    G4double vacuumSizeY = 50.0 * cm;
    G4double vacuumSizeZ = 1.2 * m;

G4Box* vacuumWorld = new G4Box("vacuumWorld", 0.5 * vacuumSizeX, 0.5 * vacuumSizeY, 0.5 * vacuumSizeZ);

G4LogicalVolume* vacuumLogic = new G4LogicalVolume(vacuumWorld, vacuum, "vacuumLogic");

G4VPhysicalVolume* vacuumPhys = new G4PVPlacement(
    nullptr,
    G4ThreeVector(),
    vacuumLogic,
    "vacuumPhys",
    airLogic,
    false,
    0,
    checkOverlaps
);


// Define LMO detector
    G4double crystalSizeX = 2.0 * cm;
    G4double crystalSizeY = 2.0 * cm;
    G4double crystalSizeZ = 2.0 * cm;

G4Box* crystalSolid = new G4Box("crystalSolid", 0.5 * crystalSizeX, 0.5 * crystalSizeY, 0.5 * crystalSizeZ);
G4LogicalVolume* crystalLogic = new G4LogicalVolume(crystalSolid, LMO, "crystalLogic");
G4VPhysicalVolume* crystalPhys = new G4PVPlacement(
    nullptr,
    G4ThreeVector(),
    crystalLogic,
    "crystalPhys",
    vacuumLogic,
    false,
    0,
    checkOverlaps
);

// Define radioactive source
G4double sourceRadius = 0.3 * cm;
G4double sourceThickness = 0.05 * cm;
auto* Cs = nist->FindOrBuildMaterial("G4_Cs");


    auto* Sourcesolid = new G4Tubs(
        "SourceSolid",
        0.0,
        sourceRadius,
        0.5 * sourceThickness,
        0.0,
        360.0 * deg
    );

    G4LogicalVolume* Sourcelogic = new G4LogicalVolume(Sourcesolid, Cs, "SourceLogic");
    new G4PVPlacement(
        nullptr,
        G4ThreeVector(0.0, 0.0, -5.0 * cm),
        Sourcelogic,
        "Source",
        airLogic,
        false,
        0,
        true
    );

// Visualization attributes
    vacuumLogic->SetVisAttributes(G4VisAttributes::GetInvisible());

    auto* vacuumVis = new G4VisAttributes(
        G4Colour(0.8, 0.8, 1.0, 0.15)
    );
    vacuumVis->SetVisibility(true);
    vacuumVis->SetForceWireframe(true);
    vacuumLogic->SetVisAttributes(vacuumVis);

    auto* crystalVis = new G4VisAttributes(
        G4Colour(0.1, 0.8, 1.0, 0.8)
    );
    crystalVis->SetVisibility(true);
    crystalVis->SetForceSolid(true);
    crystalLogic->SetVisAttributes(crystalVis);

    auto* sourceVis = new G4VisAttributes(
        G4Colour(1.0, 0.1, 0.1, 1.0)
    );
    sourceVis->SetVisibility(true);
    sourceVis->SetForceSolid(true);
    Sourcelogic->SetVisAttributes(sourceVis);

    return airPhys;
}