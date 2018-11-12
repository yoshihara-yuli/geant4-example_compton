# README
Sample geometry: Compton camera with GAGG (Geant4 4.9.6.p03 code)

## History
 - 2016.11.11 created  
 - 2018.9.6. revised  
 
## 1) Build & Run 
 See **HowToBuild.pdf**.

## 2) Geometry

 There is a Compton camera (**module**) in atrmospheric air.
 Each module has a scatter box (**Scabox**) and an absorber box (**Absbox**).

 Scabox consists of 8x8 scatter voxels (**scavoxel**), which is 5x10x10mm.
 Absbox consists of 8x8 scatter voxels (**absvoxel**), which is 10x10x10mm.
 The distance between Scabox and the origin is 10 cm.
 The distance between Scabox and Absbox is 8 cm.
 (See source files for more information about GAGG properties and detail geometry.)

 Both scavoxel and absvoxel are assign as “sensitive detector”. (See Geant4 guidance documents)

![compton_geometry](geometry.png)

## 3) Source

 Point source: uniform irradiation of Cs-137 at the origin (0,0,0)

## 4) Data Output
 The simulation results are output in **result.txt**.  
 (See **AboutCode.pdf** and the corresponding source files for more information.)
 
 Besides, [**analysis.py**](https://github.com/yoshihara-yuli/geant4-example_compton/blob/master/analysis/analysis.py) is uploaded for basic analysis of the simulation data. 
 You can start to check and analize data with this code.

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

Generally, you don't have to analize **posx**, **posy**, **posz**, **time**, **track**, **particle**, which are information obtained only through simulation (not experiment). There can be used for simulation check, sometimes.  
**Caution!** These six information (posx, poy, posz, time, track, particle) are only for the interaction at the end step in the GAGG crystal**

## The change of data format
Geant4 output data format is defined in **ExN02TrackerSD.cc**. 
(Please modify ExN02TrackerSD.cc if you want to change data format.)



