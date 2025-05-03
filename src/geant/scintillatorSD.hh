#ifndef SCINTILLATORSD_HH
#define SCINTILLATORSD_HH

#include <map>

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

#include "scinthit.hh"
#include "G4AnalysisManager.hh"
#include "run.hh"
#include "construction.hh"


class ScintillatorSD : public G4VSensitiveDetector
{
  public:
    ScintillatorSD(const G4String);
    ~ScintillatorSD();

    virtual void Initialize(G4HCofThisEvent *) override;
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
    
    void SetCopyPositions(std::map<G4int, G4ThreeVector> copyPositions) { fCopyPositions = copyPositions; }
    
    
    std::map<G4int, G4ThreeVector> GetCopyPositions() { return fCopyPositions; }
    

  private:
    G4THitsCollection<ScintillatorHit>* fHitsCollection;
    std::map<G4int, G4ThreeVector> fCopyPositions;

};

#endif
