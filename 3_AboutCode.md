# AboutCode 

## List of source codes
- **UserDetectorConstruction (define geometry)**
- UserEventAction
- **UserPhysicsList (set physics list)**
- **UserPrimaryGeneratorAction (define source)**
- UserRunAction
- UserStackingAction
- ExN02TrackerHit
- **ExN02TrackerSD (set output data you want)**  

<font color="Blue">NOTE: The only **bold** codes should be modified according to your simulation.</font>

-
### 1) UserDetectorConstruction
 See geometry tutorial [here](https://geant4.web.cern.ch/collaboration/working_groups/geometry) for detail.

 - Define materials for detector (**G4Material, G4Element**)  
 - Define detector geometry
   - Start with its shape and size (**G4VSolid**)
   - Add properties (**G4LogicalVolume**)
   - Set position and rotation in another volume  (**G4VPhysicalVolume**)   

 - Set Visualization attributes (**SetVisAttributes**)
 - Set sensitive detectors (**SetSensitiveDetector**)  
   The default SDs are scavoxel and absvoxel.  

<font color="Blue">Note: You should identify mulitple SDs from the same logic volume with **copy number of G4PVPlacement**</font>

-
### 2) UserEventAction
You don't have to change this code.

-
### 3) UserPhysicsList
 - Choose physicslist (standard or penelope) (default: penelope)
 - Choose cut value (dafault: 1 mm)

-
### 4) UserPrimaryGeneratorAction
 - Set particle (**particleTable->FindParticle**)
 - Set particle energy (**SetParticleEnergy**)
 - Set particle position (**SetParticlePosition**)
 - Set particle movement (**SetParticleMomentumDirection**)

<font color="Blue">NOTE: **B3PrimaryGeneratorAction** is setting only for the first beam, and **GeneratePrimaries** is setting for the other all beams. If you want to set different particle movement for every beam, you can change the above parameters in **GeneratePrimaries**.</font>

-
### 5) UserRunAction
You don't have to change this code.

-
### 6) UserStackingAction
You don't have to change this code.

-
### 7) ExN02TrackerHit
You don't have to change this code.

### 8) ExN02TrackerSD
You can extract the information from sensitive detectors you set in UserDetectorConstruction and output to textfile (result.txt). If you want to specific treatment here before extracting all raw data, you can use another output file (result2.txt).

## Appendix 
### 2.1. Random seeds
You can have different random seeds everytime regardless of simulation code itself.
If you want to use specific random seeds or fix random seeds for the all simulation, please modify example.cc.

### 2.2 General Paricle Soure (GPS)
If you want to change source condition in macro file, you need specific sentences in some files (please contact to Yoshihara, if you want to use)
- - - - - - - - - - - - - - - - - - - - - - 
