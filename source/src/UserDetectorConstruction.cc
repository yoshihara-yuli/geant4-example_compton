//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Softwar1e License,  included in the file *
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
/// \file UserDetectorConstruction.cc
/// \brief Implementation of the UserDetectorConstruction class

#include "UserDetectorConstruction.hh"
#include "ExN02TrackerSD.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#define pi 3.141592

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserDetectorConstruction::UserDetectorConstruction()
: G4VUserDetectorConstruction(),
  logicScavoxel(0), // sensitive detector (scatter voxel)
  logicAbsvoxel(0), // sensitive detector (absorber voxel)
  fLogicPatient(0), // sensitive detector (phantom, not used now)
  fCheckOverlaps(true)
{
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserDetectorConstruction::~UserDetectorConstruction(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UserDetectorConstruction::DefineMaterials(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* UserDetectorConstruction::Construct()
{  

  // --------------- Materials Definition -------------------

  // Air 
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air = nist->FindOrBuildMaterial("G4_AIR", false);  
  G4double a, z, density;

  // GAGG 
  G4Element* elGd = new G4Element("Gadolinium", "Gd", z=64, a=157.25    *g/mole);
  G4Element* elAl = new G4Element("Aluminium" , "Al", z=13, a= 26.981539*g/mole);
  G4Element* elGa = new G4Element("Gallium"   , "Ga", z=31, a= 69.723   *g/mole);
  G4Element* elO  = new G4Element("Oxygen"    , "O" , z= 8, a= 15.9994  *g/mole);
  G4Material* gagg = new G4Material("GAGG", density=6.63*g/cm3, 4);
  gagg -> AddElement(elGd,  3);
  gagg -> AddElement(elAl,  2);
  gagg -> AddElement(elGa,  3);
  gagg -> AddElement(elO , 12);

  // --------------- Geometory Definition -------------------

  // --- Overview of geometry ---
  // world - module - scabox - scavoxel*
  //                  absbox - absvoxel*     
  // (* ... Sensitive Detector)


  // --- Parameters for geometry ---

  // world
  G4double world_sizeX = 100*cm; 
  G4double world_sizeY = 100*cm; 
  G4double world_sizeZ = 100*cm;

  // scabox
  G4double scabox_sizeX =      5.0  *mm;
  G4double scabox_sizeY =     88.0  *mm; 
  G4double scabox_sizeZ =     88.0  *mm;

  // scavoxel
  G4double scavoxel_sizeX  =    5.0  *mm;
  G4double scavoxel_sizeY  =   10.0  *mm;
  G4double scavoxel_sizeZ  =   10.0  *mm;
  G4double scavoxel_pitchY =   11.0  *mm;
  G4double scavoxel_pitchZ =   11.0  *mm;
  G4int number_of_scavoxel_dimY  =   8; 
  G4int number_of_scavoxel_dimZ  =   8; 

  // absbox
  G4double absbox_sizeX =   10.0  *mm;
  G4double absbox_sizeY =   88.0  *mm;
  G4double absbox_sizeZ =   88.0  *mm;

  // absvoxel
  G4double absvoxel_sizeX =    10.0  *mm;
  G4double absvoxel_sizeY =    10.0  *mm;
  G4double absvoxel_sizeZ =    10.0  *mm;
  G4double absvoxel_pitchY =   11.0  *mm;
  G4double absvoxel_pitchZ =   11.0  *mm;
  G4int number_of_absvoxel_dimY  =  8; 
  G4int number_of_absvoxel_dimZ  =  8;  

  // distances
  G4double distance_source_sca  = 400.0  *mm; // distance between scatter and origin(0,0,0)
  G4double distance_sca_abs     = 110.0  *mm; // distance between absorber and scatter

  // module
  G4double module_sizeX = scabox_sizeX+distance_sca_abs+absbox_sizeX; 
  G4double module_sizeY = absbox_sizeY; // under supposition that scabox and absbos are same  
  G4double module_sizeZ = absbox_sizeZ; // under supposition that scabox and absbos are same  

  G4int number_of_modules = 1;  // 1: single Compton camera module
  G4double angle_rot_modules = 360.0/number_of_modules *deg;

  // --- Geometry --- 

  // --- World

  // solid definition (size)
  G4Box* solidWorld =    
    new G4Box("World",                       // its name
	      0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ); // its size
  
  // logical volume definition (material)
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          // its solid
                        air,                 // its material
                        "World");            // its name

  // physical volume definition (position, relation, copy number)
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     // no rotation
                      G4ThreeVector(),       // at (0,0,0)
                      logicWorld,            // its logical volume
                      "World",               // its name
                      0,                     // its mother  volume
                      false,                 // no boolean operation
                      0,                     // copy number
                      fCheckOverlaps);       // checking overlaps 


  // --- Module
  
  // solid definition (size)
  G4Box* solidModule =    
    new G4Box("Module",                       // its name
	      0.5*module_sizeX, 0.5*module_sizeY, 0.5*module_sizeZ); // its size
  
  // logical volume definition (material)
  G4LogicalVolume* logicModule =                         
    new G4LogicalVolume(solidModule,          // its solid
                        air,                  // its material
                        "Module");            // its name

  // physical volume definition (position, relation, copy number)
  for (G4int i = 0; i < number_of_modules ; i++) {
  
    G4ThreeVector armVec= G4ThreeVector((distance_source_sca+module_sizeX*0.5),0,0);
    G4RotationMatrix armRot= G4RotationMatrix();
    armVec.rotateZ(i*angle_rot_modules);
    armRot.rotateZ(i*angle_rot_modules);

    new G4PVPlacement(G4Transform3D(armRot, armVec),    
                      logicModule,            // its logical volume
                      "Module",               // its name
                      logicWorld,             // its mother  volume
                      false,                  // no boolean operation
                      i,                      // copy number
                      fCheckOverlaps);        // checking overlaps 
   }


  // --- Scatter

  // -- scatter box

  // solid definition (size)
  G4Box* solidScabox =    
    new G4Box("Scabox",                       // its name
	      0.5*scabox_sizeX, 0.5*scabox_sizeY, 0.5*scabox_sizeZ); // its size
  
  // logical volume definition (material)
  G4LogicalVolume* logicScabox =                         
    new G4LogicalVolume(solidScabox,          // its solid
                        air,              // its material 
                        "Scabox");            // its name

  // physical volume definition (position, relation, copy number)

  new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(scabox_sizeX*0.5 - module_sizeX*0.5, 0, 0),  
                      logicScabox,            // its logical volume
                      "Scabox",               // its name
                      logicModule,            // its mother  volume
                      false,                  // no boolean operation
                      0,                      // copy number
                      fCheckOverlaps);        // checking overlaps 

  
  // -- scatter vocel

  // solid definition (size)
  G4Box* solidScavoxel =    
    new G4Box("scavoxel",                       // its name
	      0.5*scavoxel_sizeX, 0.5*scavoxel_sizeY, 0.5*scavoxel_sizeZ); // its size
  
  // logical volume definition (material)
  logicScavoxel =                         
    new G4LogicalVolume(solidScavoxel,          // its solid
                        gagg,                   // its material
                        "Scavoxel");            // its name

  // physical volume definition (position, relation, copy number)
  for (G4int i = 0; i < number_of_scavoxel_dimY ; i++) {
    for (G4int j = 0; j < number_of_scavoxel_dimZ ; j++) {
       new G4PVPlacement(0,                     // no rotation
                      G4ThreeVector(0,(i+0.5)*scavoxel_pitchY-scabox_sizeY*0.5,(j+0.5)*scavoxel_pitchZ-scabox_sizeZ*0.5), //location
                      logicScavoxel,            // its logical volume
                      "Scavoxel",               // its name
                      logicScabox,              // its mother  volume
                      false,                    // no boolean operation
                      i*number_of_scavoxel_dimZ+j,      // copy number
                      0);                       // checking overlaps
    }
  } 
  

  // --- Absorber

  // -- absober box

  // solid definition (size)
  G4Box* solidAbsbox =    
    new G4Box("Absbox",                       // its name
	      0.5*absbox_sizeX, 0.5*absbox_sizeY, 0.5*absbox_sizeZ); // its size
  
  // logical volume definition (material)
   G4LogicalVolume* logicAbsbox =                         
    new G4LogicalVolume(solidAbsbox,          // its solid
                        air,                  // its material
                        "Absbox");            // its name

  // physical volume definition (position, relation, copy number)
    new G4PVPlacement(0,                      // no rotation
                      G4ThreeVector(module_sizeX*0.5 - absbox_sizeX*0.5, 0, 0),      
                      logicAbsbox,            // its logical volume
                      "Absbox",               // its name
                      logicModule,             // its mother  volume
                      false,                  // no boolean operation
                      1,                      // copy number
                      fCheckOverlaps);        // checking overlaps 
  

  // absorber voxel

  // solid definition (size)
  G4Box* solidAbsvoxel =    
    new G4Box("Absvoxel",                       // its name
	      0.5*absvoxel_sizeX, 0.5*absvoxel_sizeY, 0.5*absvoxel_sizeZ); // its size
  
  // logical volume definition (material)
   logicAbsvoxel =                         
    new G4LogicalVolume(solidAbsvoxel,          // its solid
                        gagg,                   // its material
                        "Absvoxel");            // its name

  // physical volume definition (position, relation, copy number)
  for (G4int i = 0; i < number_of_absvoxel_dimY ; i++) {
    for (G4int j = 0; j < number_of_absvoxel_dimZ ; j++) {
      new G4PVPlacement(0,                    // no rotation
                      G4ThreeVector(0,(i+0.5)*absvoxel_pitchY-absbox_sizeY*0.5,(j+0.5)*absvoxel_pitchZ-absbox_sizeZ*0.5), // location
                      logicAbsvoxel,          // its logical volume
                      "Absvoxel",             // its name
                      logicAbsbox,            // its mother  volume
                      false,                  // no boolean operation
                      i*number_of_absvoxel_dimZ+j,  // copy number
                      fCheckOverlaps);        // checking overlaps
    }
  } 


  // -- patient (not used now)

  G4double patient_radius = 1*mm;
  G4double patient_dZ = 1*mm;  

  G4Material* patient_mat = nist->FindOrBuildMaterial("G4_AIR", false);

  G4Tubs* solidPatient =
    new G4Tubs("Patient", 0., patient_radius, 0.5*patient_dZ, 0., 2*3.14);
      
  fLogicPatient =                         
    new G4LogicalVolume(solidPatient,        //its solid
                        patient_mat,         //its material
                        "Patient");          //its name
                              
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    fLogicPatient,           //its logical volume
                    "Patient",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps 


  // --------------- Other Settings -------------------
		    
  // -- Visualization attributes
  //logicWorld->SetVisAttributes(G4VisAttributes(G4VisAttributes::Invisible)); // invisible

  logicWorld->SetVisAttributes(G4VisAttributes(true, G4Colour(0, 0, 0))); // black

  logicModule->SetVisAttributes(G4VisAttributes(true, G4Colour(0, 0, 0))); // black

  logicScabox->SetVisAttributes(G4VisAttributes(true, G4Colour(1, 0, 0))); // red

  logicAbsbox->SetVisAttributes(G4VisAttributes(true, G4Colour(1, 0, 0))); // red

  logicScavoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(1, 1, 0))); // yellow

  logicAbsvoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(1, 1, 0))); // yellow

  fLogicPatient->SetVisAttributes(G4VisAttributes(G4VisAttributes::Invisible)); // patient (not used) invisible

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl; 
    
  // Creating SensitiveDetectors
  CreateScorers();
                                                                                
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UserDetectorConstruction::CreateScorers()
{

  // Creating Sensitive Detector to get information.
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(2);
  ExN02TrackerSD* sensorSD = new ExN02TrackerSD("/sensorSD");
  SDman-> AddNewDetector(sensorSD);

  logicScavoxel -> SetSensitiveDetector(sensorSD); // scatter : sensitive detector
  logicAbsvoxel -> SetSensitiveDetector(sensorSD); // absorber : sensitive detector
  
  // declare patient as a MultiFunctionalDetector score
  G4MultiFunctionalDetector* patient = new G4MultiFunctionalDetector("patient");
  G4VPrimitiveScorer* primitiv2 = new G4PSDoseDeposit("dose");
  patient->RegisterPrimitive(primitiv2);
  SDman->AddNewDetector(patient);
  fLogicPatient->SetSensitiveDetector(patient);  

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
