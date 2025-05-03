#include "scinthit.hh"


ScintillatorHit::ScintillatorHit()
{

  fEvent=-1;
  fIsPrimary = -1;
  fTime = 0.;
  fInitialEnergy = 0.;
  fDetEnergy = 0.;
  fHitPosition = G4ThreeVector();
  fParticleName = "";
  fCopyNumber = 0;
  fDetectorPosition = G4ThreeVector();
  fParticleOriginPos = G4ThreeVector();
  fParticleOriginTime = 0.;
  
  
  
}

ScintillatorHit::~ScintillatorHit()
{}