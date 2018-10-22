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
// $Id: ExN02TrackerHit.hh 69899 2013-05-17 10:05:33Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02TrackerHit_h
#define ExN02TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02TrackerHit : public G4VHit
{
  public:

      ExN02TrackerHit();
     ~ExN02TrackerHit();
      ExN02TrackerHit(const ExN02TrackerHit&);
      const ExN02TrackerHit& operator=(const ExN02TrackerHit&);
      G4int operator==(const ExN02TrackerHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

  public:
    
    void Set(int event, int granma_copy,int mum_copy,int copy, const G4Track* track ,G4double eDep); // [yy] add granma_copy and mum_copy
    
    G4int GetEventID()    const  { return eventID; };    // [yy]
    G4int GetCopyNO0()    const  { return copyNO; }; // [yy]
    G4int GetCopyNO1()    const  { return mum_copyNO; }; // [yy]
    G4int GetCopyNO2()    const  { return granma_copyNO; }; // [yy]
    
    G4int GetTrackID()            const  { return trackID; };
    G4int GetPDGcode()            const  { return codePDG; };
    G4double GetCharge()          const  { return charge; };
    G4double GetEnergy()          const  { return energy; };
    const G4ThreeVector& GetMomentum() const  { return momentum; };
    const G4ThreeVector& GetPos()      const  { return pos; };
    G4double GetTime()                 const  { return time; };
    G4double GetEdep() const { return eDep;};
    void AddEdep(G4double val){ eDep += val;};
      
  private:
  
    G4int         eventID;    // [yy]
    G4int         granma_copyNO; // [yy]
    G4int         mum_copyNO; // [yy]
    G4int         copyNO;
    G4int         trackID;
    G4int         codePDG;
    G4double      charge;
    G4double      energy;
    G4ThreeVector momentum;
    G4ThreeVector pos;
    G4double      time;
    G4double         eDep;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<ExN02TrackerHit> ExN02TrackerHitsCollection;

extern G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* ExN02TrackerHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) ExN02TrackerHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ExN02TrackerHit::operator delete(void *aHit)
{
  ExN02TrackerHitAllocator.FreeSingle((ExN02TrackerHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
