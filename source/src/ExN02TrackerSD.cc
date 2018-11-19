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
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//#include "G4UnitsTable.hh"
#include "G4VProcess.hh" 
#include "G4EventManager.hh" 
#include "G4Event.hh"

#include "CLHEP/Random/Random.h" // [mtsuk]

#include <iostream>
#include <vector>

extern std::ofstream ofs;

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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ExN02TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

    const G4Track * aTrack =  aStep->GetTrack();
    
    //Check Energy deposit
    G4double eDep = aStep->GetTotalEnergyDeposit();
    if (eDep <= 0.0 ) return false;
    G4double time = aTrack->GetGlobalTime();
    
    G4int copyNO = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);
    G4int granma_copyNO = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2); //[yy]
    G4int mum_copyNO = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1); //[yy]
    
    G4int NbHits = trackerCollection->entries();
    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    G4bool found = false;
    for (G4int iHit=0; (iHit<NbHits) && (!found) ;iHit++) {
        found = (   (copyNO == (*trackerCollection)[iHit]->GetCopyNO0()) //);
                 && (mum_copyNO == (*trackerCollection)[iHit]->GetCopyNO1()) // [yy]
                 && (granma_copyNO == (*trackerCollection)[iHit]->GetCopyNO2()) ); // [yy]
        if (found) {
            (*trackerCollection)[iHit]->AddEdep(eDep);
            return true;
        }
    }
    
    /*
     G4cout << "[debug] : \t"
     << "Person: "  << copyNO << ",\t"
     << "Mother: "  << mum_copyNO    << ",\t"
     << "Granma: "  << granma_copyNO << "\t" << G4endl;
     */
    
    ExN02TrackerHit* newHit = new ExN02TrackerHit();
    newHit->Set(eventID, granma_copyNO,mum_copyNO,copyNO, aTrack, eDep);
    trackerCollection->insert( newHit );

  return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    G4int NbHits = trackerCollection->entries();
    if (verboseLevel>0) {
        G4cout << "\n-------->Hits Collection: in this event they are "
        << NbHits
        << " hits in the tracker chambers: " << G4endl;
    }
    
    G4bool isFirstHit = true;
    G4bool hasHit=false;
    
    for (G4int i=0;i<NbHits;i++){
        ExN02TrackerHit* hit = (*trackerCollection)[i];
        if (verboseLevel>1) hit->Print();
        
        // output hits other than trigger counters
        //if (hit->GetEdep() < eThreshold ) continue;
        if (isFirstHit) {
            isFirstHit = false;
            hasHit = true;
        }
        
        /*
        // for Debug
         G4cout << "[debug] : \t"
         << "Person: "  << hit->GetCopyNO0() << ",\t"
         << "Mother: "  << hit->GetCopyNO1()    << ",\t"
         << "Granma: "  << hit->GetCopyNO2() << "\t" << G4endl;
         */
        
        // ------------------------------
        // Energy resolution [mtsuk]
        // ------------------------------
        
        G4double edep_keV = 1000.*(hit->GetEdep()/MeV);
        
        G4double K = 1.314; // FWHM 12% @ 662keV
        //G4double K = 1.094; // FWHM 10% @ 662keV
        //G4double K = 0.657; // FWHM 6% @ 662keV
        //G4double K = 0.547; // FWHM 5% @ 662keV
        //G4double K = 0.438; // FWHM 4% @ 662keV
        
        G4double sigma = K * sqrt(edep_keV);
        G4double edep_keV_observed = CLHEP::RandGauss::shoot(edep_keV, sigma);
        G4double edep_MeV_observed = edep_keV_observed / 1000.;
        
        ofs  << hit->GetEventID()  // [yy]
        << "\t"  << hit->GetCopyNO2() // [yy] granma
        << "\t"  << hit->GetCopyNO1() // [yy] mother
        << "\t"  << hit->GetCopyNO0() // [yy] person
        << "\t"  << hit->GetEdep()/MeV
        << "\t"  << hit->GetPos().x()/mm
        << "\t"  << hit->GetPos().y()/mm
        << "\t"  << hit->GetPos().z()/mm
        << "\t"  << hit->GetTime()/ns
        << "\t"  << hit->GetTrackID()
        << "\t"  << hit->GetPDGcode()
        << std::endl;
    
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

