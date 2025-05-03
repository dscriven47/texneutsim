#ifndef PHOTOCATHSD_HH
#define PHOTOCATHSD_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "g4root.hh"
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