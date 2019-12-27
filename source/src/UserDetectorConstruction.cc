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
logicVoxel(0), // sensitive detector (scatter voxel)
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
    
    // NaI crystal
    G4Element* elNa = new G4Element("Sodium",  "Na", z=11., a= 22.989768*g/mole);
    G4Element* elI  = new G4Element("Iodine",  "I",  z=53., a= 126.90447*g/mole);
    G4Material* NaI = new G4Material("NaI", density=3.67 *g/cm3, 2);
    NaI-> AddElement(elNa, 1);
    NaI-> AddElement(elI,  1);
    
    // CsI crystal
    G4Element* elCs = new G4Element("Cesium",  "Cs", z=55., a= 132.90543*g/mole);
    G4Material* CsI = new G4Material("CsI", density=4.51 *g/cm3, 2);
    CsI-> AddElement(elCs, 1);
    CsI-> AddElement(elI,  1);
    
    // Al
    G4Material* Al = new G4Material("Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3);

    // Steel
    G4Element* elFe = G4NistManager::Instance()->FindOrBuildElement("Fe");
    G4Element* elNi = G4NistManager::Instance()->FindOrBuildElement("Ni");
    G4Element* elCr = G4NistManager::Instance()->FindOrBuildElement("Cr");
    G4Material* Steel = new G4Material("StainlessSteel", 7.80 * g/cm3, 3);
    Steel -> AddElement(elFe, 70 * perCent);
    Steel -> AddElement(elCr, 18 * perCent);
    Steel -> AddElement(elNi, 12 * perCent);
    
    // --------------- Geometory Definition -------------------
    
    // --- Overview of geometry ---
    // world - module - box - voxel*
    //                  absbox - absvoxel*
    // (* ... Sensitive Detector)
    
    
    // --- Parameters for geometry ---
    
    // world
    G4double world_sizeX = 200*cm;
    G4double world_sizeY = 200*cm;
    G4double world_sizeZ = 200*cm;
    
    // voxel
    G4double voxel_sizeX =   2.0  *mm;
    G4double voxel_sizeY =   2.0  *mm;
    G4double voxel_sizeZ =   2.0  *mm;
    G4double voxel_pitchX =  2.0  *mm;
    G4double voxel_pitchY =  2.0  *mm;
    G4int number_of_voxel_dimX  =   25;
    G4int number_of_voxel_dimY  =   25;
    
    // box
    G4double box_sizeX =  voxel_pitchX*number_of_voxel_dimX;
    G4double box_sizeY =  voxel_pitchY*number_of_voxel_dimY;
    G4double box_sizeZ =  voxel_sizeZ;
    
    // pipe height
    G4double pipe_outer_radius = 48.6*0.5 *mm;
    G4double pipe_inner_radius = 41.6*0.5 *mm;
    G4double pipe_height = pipe_outer_radius*2.0;
    
    // distances
    G4double distance_source_box  =  3.0  *mm; // distance between scatter and origin(0,0,0)
    
    // module
    G4double module_sizeX = box_sizeX;
    G4double module_sizeY = box_sizeY;
    G4double module_sizeZ = box_sizeZ;
    
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
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0, 0, pipe_outer_radius+distance_source_box+0.5*module_sizeZ),
                      logicModule,            // its logical volume
                      "Module",               // its name
                      logicWorld,             // its mother  volume
                      false,                  // no boolean operation
                      0,                      // copy number
                      fCheckOverlaps);        // checking overlaps
    
    // --- Detector
    
    // --  box
    
    // solid definition (size)
    G4Box* solidBox =
    new G4Box("Box",                       // its name
              0.5*box_sizeX, 0.5*box_sizeY, 0.5*box_sizeZ); // its size
    
    // logical volume definition (material)
    logicBox =
    new G4LogicalVolume(solidBox,          // its solid
                        air,              // its material
                        "Box");            // its name
    
    // physical volume definition (position, relation, copy number)
    
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0, 0, 0),
                      logicBox,            // its logical volume
                      "Box",               // its name
                      logicModule,            // its mother  volume
                      false,                  // no boolean operation
                      0,                      // copy number
                      fCheckOverlaps);        // checking overlaps
    
    
    // --  voxel
    
    // solid definition (size)
    G4Box* solidVoxel =
    new G4Box("Voxel",                       // its name
              0.5*voxel_sizeX, 0.5*voxel_sizeY, 0.5*voxel_sizeZ); // its size
    
    // logical volume definition (material)
    logicVoxel =
    new G4LogicalVolume(solidVoxel,          // its solid
                        NaI,                   // its material
                        "Voxel");            // its name
    
    // physical volume definition (position, relation, copy number)
    for (G4int i = 0; i < number_of_voxel_dimY ; i++) {
        for (G4int j = 0; j < number_of_voxel_dimX ; j++) {
            new G4PVPlacement(0,                     // no rotation
                              G4ThreeVector((j+0.5)*voxel_pitchX-box_sizeX*0.5,
                                            (i+0.5)*voxel_pitchY-box_sizeY*0.5,0), //location
                              logicVoxel,            // its logical volume
                              "Voxel",               // its name
                              logicBox,              // its mother  volume
                              false,                    // no boolean operation
                              i*number_of_voxel_dimX+j,      // copy number
                              0);                       // checking overlaps
        }
    }
    
    // --- Phantom
    
    // -- Pipe
    
    // solid definition (size)
    G4Tubs* solidPipe =
    new G4Tubs("Pipe",                      // its name
               pipe_inner_radius,           // inner radius
               pipe_outer_radius,           // outer radius
               pipe_height*0.5,                 // height
               0*deg,180*deg
               ); // its size
    
    // logical volume definition (material)
    logicPipe =
    new G4LogicalVolume(solidPipe,          // its solid
                        air,                // its material
                        "Pipe");             // its name
    
    // physical volume definition (position, relation, copy number)
    G4ThreeVector armVec= G4ThreeVector(0,0,0);
    G4RotationMatrix armRot= G4RotationMatrix();
    armVec.rotateX(90*deg);
    armRot.rotateX(90*deg);
    
    physPipe = new G4PVPlacement(G4Transform3D(armRot, armVec),                        // no rotation
                      logicPipe,            // its logical volume
                      "Pipe",               // its name
                      logicWorld,            // its mother  volume
                      false,                  // no boolean operation
                      0,                      // copy number
                      fCheckOverlaps);        // checking overlaps
    
    
    // --------------- Other Settings -------------------
    
    // -- Visualization attributes
    //logicWorld->SetVisAttributes(G4VisAttributes(G4VisAttributes::Invisible)); // invisible
    
    logicWorld->SetVisAttributes(G4VisAttributes(true, G4Colour(0, 0, 0))); // black
    
    logicModule->SetVisAttributes(G4VisAttributes(true, G4Colour(0, 0, 0))); // black
    
    //logicBox->SetVisAttributes(G4VisAttributes(true, G4Colour(1, 0, 0))); // red
    
    //logicVoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(0.2, 0.2, 0.8))); // yellow
    logicVoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(0.3, 0.3, 0.3))); // yellow
    
    logicPipe->SetVisAttributes(G4VisAttributes(true, G4Colour(0.5, 0.5, 0.5))); // gray
    
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
    
    logicVoxel -> SetSensitiveDetector(sensorSD); // scatter : sensitive detector
    
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



