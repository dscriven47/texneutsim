#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
  fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

  if(volume != fScoringVolume) return;

  //G4double edep = step->GetTotalEnergyDeposit();
  //fEventAction->AddEdep(edep);




  // here we are trying to find the time at which a neutron is created and where. 
  /*auto secondaries = step->GetSecondaryInCurrentStep();
  
  for (const auto& secondary : *secondaries) {
    if (secondary->GetDefinition() == G4Neutron::Definition()) {
      auto secondarytrack = secondary;
      G4cout << "Neutron created: " << G4endl
              << " trackID: " << secondarytrack->GetTrackID() << G4endl
              << "  Initial Energy: " << secondarytrack->GetKineticEnergy() / MeV << " MeV" << G4endl
              << "  Creation Time: " << secondarytrack->GetGlobalTime() / ns << " ns" << G4endl
              << "  Creation Position: " << secondarytrack->GetPosition() << G4endl;
    }
  }
  */
  
}