#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include <map>

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4ProductionCutsTable.hh"
#include "G4UserLimits.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "scintillatorSD.hh"
#include "photocathSD.hh"



// Here we define a new detector construction which is inherited from the
// G4UserDetectorConstruction class supplied by Geant4.
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MyDetectorConstruction(); // constructor
    ~MyDetectorConstruction(); // destructor

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    virtual G4VPhysicalVolume *Construct(); // this is the main function that constructs the detector geometry

    
    
  private:
    G4Box *solidWorld, *solidRadiator, *solidDetector,*fScint_box,*fPad_box;
    G4SubtractionSolid *fHousing_box;
    G4Tubs *fPhotocath,*fPmt,*fMuShield_tub;
    G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector, 
                    *fScint_log,*fHousing_log,*fPmt_log,*fPhotocath_log,
                    *fMuShield_log,*fPad_log;
    G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, 
                      *cube_phys,*fmuMetal_phys,*pad;

    G4OpticalSurface *mirrorSurface;

    G4Material *SiO2, *H2O, *Aerogel, *worldMat, *NaI,*Air,*Al,*Vacuum,*OpticalPadSilicone,*pTerp,*Glass;
    G4Element *C, *Na,*I,*H,*O,*N;

    G4GenericMessenger *fMessenger;

    G4LogicalVolume *fScoringVolume;

    G4double xWorld,yWorld,zWorld;

    G4double fCube_mult;
    G4int imax;
    G4int jmax;

    std::map<G4int, G4ThreeVector> copyPositions;

    bool checkGeometry;
        
    void DefineMaterials();
    void ConstructScintillator();
    void ConstructTOF();
    virtual void ConstructSDandField();
    void VisAttributes();
};

#endif
