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
/// \file UserPrimaryGeneratorAction.hh
/// \brief Definition of the UserPrimaryGeneratorAction class

#ifndef UserPrimaryGeneratorAction_h
#define UserPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

class G4ParticleGun;
class G4Event;

class UserPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    
    UserPrimaryGeneratorAction();    
    virtual ~UserPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
    //G4double posX,posY,posZ;
    //const G4ParticleGun* GetParticleGun() const { return fParticleGun; } // for not gps
  
  private:
    
    G4ParticleGun*  fParticleGun; // for not gps
    G4GeneralParticleSource* gpsParticleGun; // for gps
    
    
    // for not gps
    G4double phi;
    G4double dirX, dirY, dirZ;
    G4double posX,posY,posZ;
    G4double theta,L;
    G4double particleEnergy; 
    G4int nuclide;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


