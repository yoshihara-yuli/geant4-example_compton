//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ExN02TrackerSD.cc 69899 2013-05-17 10:05:33Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh" 
#include "G4VProcess.hh" 
#include "G4EventManager.hh" 
#include "G4Event.hh" 
extern std::ofstream ofs;
extern std::ofstream ofs2;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::ExN02TrackerSD(G4String name)
:G4VSensitiveDetector(name)
{

  G4String HCname;
  collectionName.insert(HCname="trackerCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerSD::~ExN02TrackerSD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::Initialize(G4HCofThisEvent* HCE)
{

  trackerCollection = new ExN02TrackerHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, trackerCollection ); 

  // initialization
  total_edep = 0.0;
  cnt = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ExN02TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  G4double edep = aStep->GetTotalEnergyDeposit();
  G4int depth;

  if(edep==0.) return false; //yy if

  //newHit is the object of ExN02TrackerHit class which is defined as in ExN02TrackerHit.cc
  ExN02TrackerHit* newHit = new ExN02TrackerHit();
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetLayerNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(depth=1)); //depth=1: Mother 
  newHit->SetStripNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(depth=0)); //depth=0: Itself

  newHit->SetEdep     (edep);
  newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());

  trackerCollection->insert( newHit );
  
  newHit->Print();

  event = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  track = newHit->GetTrackID();
  parent = aStep->GetTrack()->GetParentID();

  point = aStep->GetPostStepPoint();
  pos   = point->GetPosition();
  process  =  aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  //G4StepPoint* prePoint       = aStep->GetPreStepPoint(); 
  G4StepPoint* postPoint      = aStep->GetPostStepPoint(); 
  G4TouchableHandle touch     = postPoint->GetTouchableHandle(); // choose Pre/Post Step 

  // person (main) -> sca/abs
  person             = touch    -> GetVolume();
  person_name        = person   -> GetName();
  person_copyNumber  = touch    -> GetCopyNumber();

  G4cout << "person:"  << "\t" << person_name << "\t" << person_copyNumber << "\t"
         << track << "\t" << parent << G4endl;

  if(strcmp(person_name,"World")==0) G4cout << "===========================" << G4endl;

  // if gamma is reacted with world air, core dumption occured! -> Select Sca/Abs
  if(strcmp(person_name,"Scavoxel")==0 || strcmp(person_name,"Absvoxel")==0){
 
    // mother -> scabox or absbox
    mother            = touch     -> GetVolume(depth=1);
    mother_name       = mother    -> GetName();
    mother_copyNumber = touch     -> GetCopyNumber(depth=1); 

    // grand mother (grandma) -> module 
    grandma            = touch    -> GetVolume(depth=2);
    grandma_name       = grandma  -> GetName();
    grandma_copyNumber = touch    -> GetCopyNumber(depth=2);  

    ofs << "event:" << "\t" << event << "\t" << particle << "\t" << process << "\t" 
        << pos << "\t" << track << "\t" << parent << "\t" << edep << "\t"
        << "person:"  << "\t" << person_name << "\t" << person_copyNumber << "\t"
        << "mother:"  << "\t" << mother_name << "\t" << mother_copyNumber << "\t" 
        << "grandma:" << "\t" << grandma_name << "\t" << grandma_copyNumber << "\t"
        << std::endl;
  }
  return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::EndOfEvent(G4HCofThisEvent*)
{

  if (verboseLevel>0) { 
     G4int NbHits = trackerCollection->entries();
     G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
            << " hits in the tracker chambers: " << G4endl;
     for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
    }

  if(total_edep>0.0){
    ofs << "event:" << "\t" << event << "\t" << particle << "\t" << process << "\t" 
        << pos << "\t" << track << "\t" << parent << "\t" << edep << "\t"
        << "person:"  << "\t" << person_name << "\t" << person_copyNumber << "\t"
        << "mother:"  << "\t" << mother_name << "\t" << mother_copyNumber << "\t" 
        << "grandma:" << "\t" << grandma_name << "\t" << grandma_copyNumber << "\t"
        << std::endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

