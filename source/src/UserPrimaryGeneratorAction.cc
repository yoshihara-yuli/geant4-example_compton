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
/// \file UserPrimaryGeneratorAction.cc
/// \brief Implementation of the UserPrimaryGeneratorAction class

#include "UserPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "math.h"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh" // for GPS

#define PI 3.141592

//extern std::ofstream ofs2;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserPrimaryGeneratorAction::UserPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
gpsParticleGun(0) // for gps
//fParticleGun(0) // for not gps
{
    
    gpsParticleGun = new G4GeneralParticleSource(); // for gps
    
    /*
    // if not use gps, these must be necessary to define the initial condition of the source
    fParticleGun  = new G4ParticleGun(1);
    particleEnergy = 1173*keV;
    
    // Pipe
    theta = G4UniformRand()*PI;
    L = (G4UniformRand()*7/2+41.6/2) *mm;
    posX = L*cos(theta);
    posY = (G4UniformRand()*50 - 25) *mm;
    posZ = L*sin(theta);
    
    
    // Initial parameters
    //posX=0*cm, posY=0*cm, posZ=0*cm; // Source position (constant)
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma"); // particle type
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(G4ThreeVector(posX,posY,posZ)); // paricle position
    fParticleGun->SetParticleEnergy(particleEnergy);            // paricle energy
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));  // irradiation direction
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserPrimaryGeneratorAction::~UserPrimaryGeneratorAction()
{
    //delete fParticleGun; // for not gps
    delete gpsParticleGun;  // for gps
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UserPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    
    gpsParticleGun->GeneratePrimaryVertex(anEvent); // for gps
    
    
    //4pi uniform irradiation (for not gps)
    
    // plane
    /*
     posX = (G4UniformRand()*100-50) *mm;
     posY = (G4UniformRand()*100-50) *mm;
     posZ = 0*mm;
     */
    
     /*
     // Pipe
     theta = G4UniformRand()*PI;
     L = (G4UniformRand()*7/2+41.6/2) *mm;
     posX = L*cos(theta);
     posY = (G4UniformRand()*50 - 25) *mm;
     posZ = L*sin(theta);
     */
    
  //particleEnergy = 1*eV; // for checking
    /*
    particleEnergy = 1173*keV;
    dirZ = G4UniformRand()*2-1;
    phi = G4UniformRand()*2*PI;
    dirX = sqrt(1-dirZ*dirZ)*cos(phi);
    dirY = sqrt(1-dirZ*dirZ)*sin(phi);
    
    fParticleGun->SetParticleEnergy(particleEnergy);            // paricle energy
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dirX,dirY,dirZ));
    //fParticleGun->SetParticlePosition(G4ThreeVector(posX,posY,posZ)); // paricle position
    fParticleGun->GeneratePrimaryVertex(anEvent); // generate perticle
    
    particleEnergy = 1333*keV;
    dirZ = G4UniformRand()*2-1;
    phi = G4UniformRand()*2*PI;
    dirX = sqrt(1-dirZ*dirZ)*cos(phi);
    dirY = sqrt(1-dirZ*dirZ)*sin(phi);
    
    fParticleGun->SetParticleEnergy(particleEnergy);            // paricle energy
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dirX,dirY,dirZ));
    //fParticleGun->SetParticlePosition(G4ThreeVector(posX,posY,posZ)); // paricle position
    fParticleGun->GeneratePrimaryVertex(anEvent); // generate perticle
     */
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




