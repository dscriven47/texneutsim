#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
  fParticleGun = new G4ParticleGun(1);// define the number of primary particles created per event

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();


  fParticleName = "neutron"; // neutron cone
  //fParticleName = "geantino"; // decays

  G4ParticleDefinition *particle = particleTable->FindParticle(fParticleName);
  
  fParticleGun->SetParticleDefinition(particle);

}


MyPrimaryGenerator::~MyPrimaryGenerator()
{
  delete fParticleGun;
}


void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{

  G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

  if(fParticleName == "geantino")
  {
    // Cf-252
    G4int Z = 98;
    G4int A = 252;

    // Na-22
    //G4int Z = 11;
    //G4int A = 22;


    G4double charge = 0.*eplus; // give charge if needed
    G4double energy = 0.*keV;

    G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);
    
    G4ThreeVector sourcePosition(0.,0.,0.); // the position of the particle gun
    G4ThreeVector sourceMomentumDir(0.,0.,1.); // the particle momentum

    // set particle gun parameters
    fParticleGun->SetParticlePosition(sourcePosition);
    fParticleGun->SetParticleMomentumDirection(sourceMomentumDir);
    fParticleGun->SetParticleEnergy(0.*MeV);
    //fParticleGun->SetParticleDefinition(particle);

    fParticleGun->SetParticleCharge(charge);
    fParticleGun->SetParticleDefinition(ion);
  }



  if(fParticleName == "neutron")
  {
    fConeAxis = G4ThreeVector(0., 0., 1.);
    fConeAngle = 30.*deg;
    fConeApexRadius = 0.*cm;

    // Generate random direction within the cone
    G4double cosTheta = std::cos(fConeAngle);
    G4double z = G4UniformRand() * (1. - cosTheta) + cosTheta;  // Random z within cone
    G4double phi = G4UniformRand() * 2. * M_PI;                   // Random azimuthal angle
    G4double r = std::sqrt(1. - z * z);
    G4double x = r * std::cos(phi);
    G4double y = r * std::sin(phi);

    // Rotate the direction vector to align with fConeAxis
    G4ThreeVector randomDirection = G4ThreeVector(x,y,z).rotateUz(fConeAxis);

    // Generate random position within the apex radius (if non-zero)
    G4ThreeVector sourcePosition = G4ThreeVector(0.,0.,-0.5*m);

    if (fConeApexRadius > 0.) {
      G4double radius = fConeApexRadius * std::sqrt(G4UniformRand());
      G4double theta = G4UniformRand() * 2. * M_PI;
      sourcePosition = G4ThreeVector(radius * std::cos(theta), radius * std::sin(theta), 0.);
    }

    // Set particle position
    fParticleGun->SetParticlePosition(sourcePosition);
    // Set particle momentum direction
    fParticleGun->SetParticleMomentumDirection(randomDirection);
    // Set particle energy
    fParticleGun->SetParticleEnergy(0.5*MeV);
    // Set particle gun 
    fParticleGun->SetParticleDefinition(particle);

  }

  fParticleGun->GeneratePrimaryVertex(anEvent); // tell geant4 to generate the primary vertex
}
