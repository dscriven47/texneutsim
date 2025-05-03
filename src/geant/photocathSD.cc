#include "photocathSD.hh"

PhotocathodeSD::PhotocathodeSD(G4String name) : G4VSensitiveDetector(name)
{
  quEff = new G4PhysicsOrderedFreeVector();

  std::ifstream datafile;
  datafile.open("efficiency.dat");

  while(1)
  {
    G4double wlen, queff;

    datafile>>wlen>>queff;

    if(datafile.eof())
      break;

    //G4cout <<wlen << " " << queff << G4endl;

    quEff->InsertValues(wlen,queff);
  }

  datafile.close();

  quEff->SetSpline(false);

}

PhotocathodeSD::~PhotocathodeSD()
{}

G4bool PhotocathodeSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{ 
  G4Track *track = aStep->GetTrack(); // this allows us to track our particle in the sensitive detector

  //track->SetTrackStatus(fStopAndKill);

  G4StepPoint *preStepPoint = aStep->GetPreStepPoint(); // when the photon enters the detector
  G4StepPoint *postStepPoint = aStep->GetPostStepPoint(); // when the photon leaves the detector

  G4double time = preStepPoint->GetGlobalTime();

  const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

  // get the detector ID for the detector that was hit
  G4int copyNo = touchable->GetCopyNumber();

  // get the position of the detector that was hit
  G4VPhysicalVolume *physVol = touchable->GetVolume();
  G4ThreeVector posDetector = physVol->GetTranslation();

  //#ifdef G4MULTITHREADED
  ////G4cout << "Copy number: " << copyNo << G4endl;
  ////G4cout << "Detector position: " << posDetector << G4endl;
  //#endif

  G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  
  
  

  // handle optical photons
  if(track->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
    // get the position of the photon when it hits the detector. 
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03; // calculate the wavelength from the momentum

    //#ifdef G4MULTITHREADED
    ////G4cout << "Photon position: " << posPhoton << G4endl;
    //#endif

    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, posPhoton[0]);
    man->FillNtupleDColumn(0, 2, posPhoton[1]);
    man->FillNtupleDColumn(0, 3, posPhoton[2]);
    man->FillNtupleDColumn(0, 4, wlen);
    man->FillNtupleDColumn(0, 5, time);
    man->AddNtupleRow(0);

    if(G4UniformRand() < quEff->Value(wlen)){
      man->FillNtupleIColumn(1, 0, evt);
      man->FillNtupleDColumn(1, 1, posDetector[0]);
      man->FillNtupleDColumn(1, 2, posDetector[1]);
      man->FillNtupleDColumn(1, 3, posDetector[2]);
      man->AddNtupleRow(1);
    }

  }





  // handle neutrons
  if(track->GetParticleDefinition()->GetParticleName() == "neutron"){
    // get the position of the neutron when it hits the detector. 
    G4ThreeVector posNeutron = preStepPoint->GetPosition();
    G4ThreeVector momNeutron = preStepPoint->GetMomentum();

    //#ifdef G4MULTITHREADED
    ////G4cout << "Neutron position: " << posNeutron << G4endl;
    //#endif
    man->FillNtupleIColumn(2, 0, evt);
    man->FillNtupleDColumn(2, 1,0);
    man->FillNtupleDColumn(2, 2, posNeutron[0]);
    man->FillNtupleDColumn(2, 3, posNeutron[1]);
    man->FillNtupleDColumn(2, 4, posNeutron[2]);
    man->FillNtupleDColumn(2, 5, posNeutron[0]);
    man->FillNtupleDColumn(2, 6, posNeutron[1]);
    man->FillNtupleDColumn(2, 7, posNeutron[2]);
    man->FillNtupleDColumn(2,8,0);
    man->FillNtupleDColumn(2,9,0);
    man->FillNtupleDColumn(2,10,0);
    man->AddNtupleRow(2);
    
  }







  // handle gamma rays
  if(track->GetParticleDefinition()->GetParticleName() == "gamma"){
    // get the position of the neutron when it hits the detector. 
    G4ThreeVector posGamma = preStepPoint->GetPosition();
    G4ThreeVector momGamma = preStepPoint->GetMomentum();
    //#ifdef G4MULTITHREADED
    ////G4cout << "Gamma position: " << posNeutron << G4endl;
    //#endif
    man->FillNtupleIColumn(2, 0, evt);
    man->FillNtupleDColumn(2, 1,0);
    man->FillNtupleDColumn(2, 2, posGamma[0]);
    man->FillNtupleDColumn(2, 3, posGamma[1]);
    man->FillNtupleDColumn(2, 4, posGamma[2]);
    man->FillNtupleDColumn(2,5,0);
    man->FillNtupleDColumn(2,6,0);
    man->FillNtupleDColumn(2,7,0);
    man->FillNtupleDColumn(2, 8, posGamma[0]);
    man->FillNtupleDColumn(2, 9, posGamma[1]);
    man->FillNtupleDColumn(2, 10, posGamma[2]);
    man->AddNtupleRow(2);

  }













  

  return true;
}