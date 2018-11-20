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
// $Id$
//
/// \file UserRunAction.cc
/// \brief Implementation of the UserRunAction class

#include "UserRunAction.hh"
#include "UserPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

extern std::ofstream ofs;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserRunAction::UserRunAction()
 : G4UserRunAction(),
   fGoodEvents(0),
   fSumDose(0.)  
{  
  //add new units for dose
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);       

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserRunAction::~UserRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UserRunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  
  fGoodEvents = 0;
  fSumDose = 0.;
   
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // ofs is defined in the main program file.
  // open the output file
  ofs.open("result.txt", std::ios::out);
  ofs  << "eventID"  // [yy]
    << "\t"  << "moduleNo" // [yy] granma
    << "\t"  << "boxNo"    // [yy] mother
    << "\t"  << "voxelNo"  // [yy] person
    << "\t"  << "edep_MeV"
    << "\t"  << "posx_mm"
    << "\t"  << "posy_mm"
    << "\t"  << "posz_mm"
    << "\t"  << "time_ns"
    << "\t"  << "trackID"
    << "\t"  << "particle"
    << std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UserRunAction::EndOfRunAction(const G4Run* run)
{


  G4int NbOfEvents = run->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  // run conditions
  //const UserPrimaryGeneratorAction* kinematic 
  //  = static_cast<const UserPrimaryGeneratorAction*>(
  //      G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  //G4ParticleDefinition* particle 
  //  = kinematic->GetParticleGun()->GetParticleDefinition();
  //G4String partName = particle->GetParticleName();                       
        
  // print
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << " The run was " << NbOfEvents 
     << "\n------------------------------------------------------------\n"
     << G4endl;

  // close the output file.
  ofs.close();
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
