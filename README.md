# README
Sample geometry: Compton camera with GAGG (Geant4 4.9.6.p03 code)

## History
 - 2016.11.11 created  
 - 2018.9.6. revised  
 
## 1) Build & Run 
 See [**HowToBuild**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/HowToBuild.md) about how to build and run.

## 2) Geometry

 There is a Compton camera (**module**) in atrmospheric air.
 Each module has a scatter box (**Scabox**) and an absorber box (**Absbox**).

 Scabox consists of 8x8 scatter voxels (**scavoxel**), which is 5x10x10mm.
 Absbox consists of 8x8 scatter voxels (**absvoxel**), which is 10x10x10mm.
 The distance between Scabox and the origin is 10 cm.
 The distance between Scabox and Absbox is 8 cm.

 Both scavoxel and absvoxel are assign as “Sensitive Detector (SD)”. 
 
These characteriscis of geometry are defined in [**UserDetectorConstruction.cc**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/source/src/UserDetectorConstruction.cc) in source/src/. Please modify it if you want to change geometry.

![compton_geometry](geometry.png)

## 3) Source

Point source: uniform irradiation of Cs-137 at the origin (0,0,0). 
 
The source is defined by using **G4GeneralParticleSource (GPS)** class, instead of G4ParticleGun class. (simple change of UserPrimaryGeneratorAction.cc by globally replacing G4ParticleGun with G4GeneralParticleSource.)

See p.22-26 in [**Geant4 User's Guide for
Application Developers**](https://gentoo.osuosl.org/distfiles/BookForAppliDev-4.10.2.pdf) about How to use GPS class.

The characteriscis of source are now defined in macro file [**run.mac**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/source/run.mac) in source/. Please modify it if you want to change the definition of source.

Besides, if you want to define **mutiple sources**, you can uncomment several sentences (L.34-39) corresponding to the definition of multiple sources in run.mac (They are commented in the current run.mac). 

(Modified on Nov.20, 2018)

## 4) Data Output
 The simulation results are output in **result.txt**.  
 (See [**AboutCode**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/AboutCode.md) and the corresponding source files for more information.)
 
 [**analysis.py**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/analysis/analysis.py) is uploaded for basic analysis of the simulation data. 
 You can start to check and analize data by using this code.
 
 Geant4 output data format is defined in [**ExN02TrackerSD.cc**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/source/src/ExN02TrackerSD.cc) in source/src/. 
 Please modify it if you want to change data format.

# Geant4 output data format 

The data format of Geant4 output has Eight information for the interactions given at a single GAGG crystal.

 - **event**  (int)  Beam number
 - **module** (int)  Module (granma) copy no.
 - **box**    (int)  Box (mother) copy no.
 - **voxel**  (int)  Voxel (person) copy np.
 - **edep**   (float) Energy deposition [MeV]
 - **posx**   (float) Position x [mm]
 - **posy**   (float) Position y [mm]
 - **posz**   (float) Position z [mm]
 - **time**   (float) Time [ns]\(time after beam generated)
 - **track** (int)   Track ID (the order of interaction process)
 - **particle** (int) # Particle ID (interacted particle)

Especially in the eight information, these four information should be in your concern.

 - **module's id** : (deafult:0) if using muptiple Compton cameras, this indicates in which Comtpon camera the interaction is occured.
 - **box's id** : 0=Scatter, 1=Absorber 
 - **voxel's id** : 0-63 corresponding to each GAGG crystal.
 - **edep** : the summed energy deposition at a specific GAGG crystal.

Generally, you don't have to analize **posx**, **posy**, **posz**, **time**, **track**, **particle**, which are information obtained only through simulation (not experiment). There might be sometimes useful for simulation check.  

**Caution!** These six information (posx, poy, posz, time, track, particle) are only for the interaction at the end step in the GAGG crystal. (You can see the definitions of 'steps' or 'cuts', in [this doumentation](https://agenda.infn.it/getFile.py/access?sessionId=2&resId=0&materialId=0&confId=10937).)
