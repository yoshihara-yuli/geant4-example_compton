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
// $Id: ExN02TrackerSD.hh 69899 2013-05-17 10:05:33Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02TrackerSD_h
#define ExN02TrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "ExN02TrackerHit.hh"


class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02TrackerSD : public G4VSensitiveDetector
{
  public:
      ExN02TrackerSD(G4String);
     ~ExN02TrackerSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      ExN02TrackerHitsCollection* trackerCollection;

  private:
      
      G4double edep;
      G4double total_edep;

      G4double total_edep_singlestrip;
      G4int event;
      G4int track;
      G4int track_orig;
      G4int parent;
      G4int parent_orig;
      G4int layer;
      G4int strip;
      ExN02TrackerHit* newHit;
      G4String process;
      G4String process_orig;
      G4String particle;
      G4String particle_orig;

      G4StepPoint* point; 
      G4ThreeVector pos;
      //G4double global_time; //yy
      //G4String  particle; //yy


      // person : scapoxel or abxvoxel
      G4VPhysicalVolume* person;
      G4String person_name;
      G4String person_name2;
      G4int person_copyNumber;
      G4int person_copyNumber2;

      // mother : scabox or absbox 
      G4VPhysicalVolume* mother;
      G4String mother_name;
      G4String mother_name2;
      G4int mother_copyNumber; 
      G4int mother_copyNumber2; 

      // grand mother (grandma) : module 
      G4VPhysicalVolume* grandma;
      G4String grandma_name;
      G4String grandma_name2;
      G4int grandma_copyNumber;  
      G4int grandma_copyNumber2; 

      G4int cnt; // [yy]
      G4ThreeVector pos2; // [yy]
      G4ThreeVector pos_orig; // [yy]

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

