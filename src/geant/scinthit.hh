#ifndef SCINTHIT_HH
#define SCINTHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

class ScintillatorHit : public G4VHit 
{
  public:
    ScintillatorHit();
    virtual ~ScintillatorHit();

    // Setters
    void SetEvent(const G4int& event) { fEvent = event; }
    void SetIsPrimary(const G4int& isprimary) { fIsPrimary = isprimary; }
    void SetTime(const G4double& time) { fTime = time; }
    void SetInitialEnergy(const G4double& energy) { fInitialEnergy = energy; }
    void SetDetEnergy(const G4double& energy) { fDetEnergy = energy; }
    void SetParticleName(const G4String& name) { fParticleName = name; }
    void SetCopyNumber(const G4int& number) { fCopyNumber = number; }
    void SetDetectorPosition(const G4ThreeVector& detpos) { fDetectorPosition = detpos; }
    void SetHitPosition(const G4ThreeVector& hitpos) { fHitPosition = hitpos; }
    void SetParticleOriginPos(const G4ThreeVector& originpos) { fParticleOriginPos = originpos; } 
    void SetParticleOriginTime(const G4double& origintime) { fParticleOriginTime = origintime; } 

    // Getters
    G4int GetEvent() const { return fEvent; } 
    G4int GetIsPrimary() const { return fIsPrimary; } 
    G4double GetTime() const { return fTime; }
    G4double GetInitialEnergy() const { return fInitialEnergy; }
    G4double GetDetEnergy() const { return fDetEnergy; }
    G4String GetParticleName() const { return fParticleName; }
    G4int GetDetCopyNumber() const { return fCopyNumber; }
    G4ThreeVector GetDetectorPosition() const { return fDetectorPosition; }
    G4ThreeVector GetHitPosition() const { return fHitPosition; }
    G4ThreeVector GetParticleOriginPos() const { return fParticleOriginPos; }
    G4double GetParticleOriginTime() const { return fParticleOriginTime; }


    
    

  private:
    G4int fEvent;
    G4int fIsPrimary;
    G4double fTime;
    G4double fInitialEnergy;
    G4double fDetEnergy;
    G4ThreeVector fPosition;
    G4String fParticleName;
    G4int fCopyNumber;
    G4ThreeVector fDetectorPosition;
    G4ThreeVector fHitPosition;
    G4ThreeVector fParticleOriginPos;
    G4double fParticleOriginTime;


};

#endif