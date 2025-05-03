#include "run.hh"

MyRunAction::MyRunAction()
{

  G4AnalysisManager *man = G4AnalysisManager::Instance();


  // setup the root  file output 
  // first root tree
  man->CreateNtuple("Photons","Photons");
  man->CreateNtupleIColumn("fEvent");
  man->CreateNtupleDColumn("fX");
  man->CreateNtupleDColumn("fY");
  man->CreateNtupleDColumn("fZ");
  man->CreateNtupleDColumn("fWlen");
  man->CreateNtupleDColumn("fTime");
  man->FinishNtuple(0);

  // second root tree
  man->CreateNtuple("PhotonHits","PhotonHits");
  man->CreateNtupleIColumn("fEvent");
  man->CreateNtupleDColumn("fX");
  man->CreateNtupleDColumn("fY");
  man->CreateNtupleDColumn("fZ");
  man->FinishNtuple(1);

  // third root tree
  man->CreateNtuple("Scoring","Scoring");
  man->CreateNtupleIColumn("fEvent");     // 0 
  man->CreateNtupleIColumn("fIsPrimary"); // 1
  man->CreateNtupleDColumn("fTime");      // 2
  man->CreateNtupleDColumn("fInitialEnergy");    // 3
  man->CreateNtupleDColumn("fDetEnergy");    // 3
  man->CreateNtupleDColumn("fHitPosX");   // 4
  man->CreateNtupleDColumn("fHitPosY");   // 5
  man->CreateNtupleDColumn("fHitPosZ");   // 6
  man->CreateNtupleDColumn("fDetPosX");   // 7
  man->CreateNtupleDColumn("fDetPosY");   // 8
  man->CreateNtupleDColumn("fDetPosZ");   // 9
  man->CreateNtupleSColumn("fParticleName");  // 10
  man->CreateNtupleIColumn("fCopyNumber"); // 11
  man->FinishNtuple(2);

}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  G4int runID = run->GetRunID();

  std::stringstream strRunID;
  strRunID << runID;

  man->OpenFile("texneutsim-output_run" + strRunID.str() + ".root");

}

void MyRunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->Write();
  man->CloseFile();

}