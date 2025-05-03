#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"

#include "Randomize.hh"

#include <cmath>


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

  private:
    G4ParticleGun *fParticleGun;
    G4ThreeVector fConeAxis;
    G4double fConeAngle;
    G4double fConeApexRadius;
    G4String fParticleName;
};

#endif