#ifndef PHOTOCATHSD_HH
#define PHOTOCATHSD_HH

#include <G4PhysicsOrderedFreeVector.hh>

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"
#include "run.hh"

class PhotocathodeSD : public G4VSensitiveDetector
{
  public:
    PhotocathodeSD(G4String);
    ~PhotocathodeSD();

  private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);

    G4PhysicsOrderedFreeVector *quEff;
};

#endif
