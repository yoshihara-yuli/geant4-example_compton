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
// $Id: ExN02TrackerHit.cc 69899 2013-05-17 10:05:33Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

ExN02TrackerHit::ExN02TrackerHit():
eventID(-1),         //[yy]
granma_copyNO(-1), //[yy]
mum_copyNO(-1),    //[yy]
copyNO(-1),
trackID(0),
codePDG(0),
charge(0.0),
energy(0.0),
time(0.0),
eDep(0.0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::~ExN02TrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(const ExN02TrackerHit& right):
G4VHit()
{
    eventID = right.eventID;
    granma_copyNO   = right.granma_copyNO; // [yy]
    mum_copyNO      = right.mum_copyNO; // [yy]
    copyNO      = right.copyNO;
    trackID     = right.trackID;
    codePDG     = right.codePDG;
    charge      = right.charge;
    energy      = right.energy;
    momentum    = right.momentum;
    pos         = right.pos;
    eDep         = right.eDep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ExN02TrackerHit& ExN02TrackerHit::operator=(const ExN02TrackerHit& right)
{
    if (this != &right) {
        eventID = right.eventID;
        granma_copyNO   = right.granma_copyNO; // [yy]
        mum_copyNO      = right.mum_copyNO; // [yy]
        copyNO      = right.copyNO;
        trackID     = right.trackID;
        codePDG     = right.codePDG;
        charge      = right.charge;
        energy      = right.energy;
        momentum    = right.momentum;
        pos         = right.pos;
        eDep         = right.eDep;
    }
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::operator==(const ExN02TrackerHit& right) const
{
    return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Set(int event, int granma_copy, int mum_copy, int copy, const G4Track* track,G4double eLoss)
{
    eventID  = event;
    granma_copyNO   =  granma_copy; // [yy]
    mum_copyNO   =  mum_copy;   // [yy]
    copyNO   =  copy;
    eDep     =  eLoss;
    trackID  =  track->GetTrackID();
    codePDG  =  track->GetDefinition()->GetPDGEncoding();
    charge   =  track->GetDefinition()->GetPDGCharge();
    energy   =  track->GetKineticEnergy();
    momentum =  track->GetMomentum();
    pos      =  track->GetPosition();
    if(time > track->GetGlobalTime()){
        time     =  track->GetGlobalTime();
    }
}

void ExN02TrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Print()
{
    G4cout << "Event ID : " << eventID << G4endl; // [yy]
    G4cout << "Copy Number (Grand Mother) : " << granma_copyNO << G4endl; // [yy]
    G4cout << "Copy Number (Mother) : " << mum_copyNO << G4endl; // [yy]
    G4cout << "Copy Number (Person): " << copyNO << G4endl;
    G4cout << "TrackID: " << trackID << G4endl;
    G4cout << "PDG code: "<< codePDG << "  "
    << "charge: " << charge/eplus << G4endl;
    G4cout << "enregy: " << energy /MeV  << G4endl;
    G4cout << "momentum: "<< momentum.x()/MeV << " "
    << momentum.y()/MeV << " "
    << momentum.z()/MeV << G4endl;
    G4cout << "position: "<< pos.x()/mm << " "
    << pos.y()/mm << " "
    << pos.z()/mm << G4endl;
    G4cout << "time: "<< time/ns << G4endl;
    G4cout << "Energy deposit" << eDep/MeV << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

