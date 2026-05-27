#ifndef LMO2x2DetectorConstruction_hh
#define LMO2x2DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"

class LMO2x2DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    LMO2x2DetectorConstruction();
    ~LMO2x2DetectorConstruction();

    G4VPhysicalVolume* Construct();
};

#endif