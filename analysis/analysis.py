""" Basic analysis code for Geant4 simulation """

import os,sys
import numpy as np
import matplotlib.pylab as plt

# Geant4 output data format 
geant4_dtype = np.dtype([("event",np.int),  # beam number 
                         ("module",np.int), # module (granma) copy no.
                         ("box",np.int),    # box (mother) copy no.
                         ("voxel",np.int),  # voxel (person) copy np.
                         ("edep",np.float), # energy deposition [MeV]
                         ("posx",np.float), # position x [mm]
                         ("posy",np.float), # position y [mm]
                         ("posz",np.float), # position z [mm]
                         ("time",np.float), # time [ns] (time after beam generated)
                         ("track",np.int),  # track id (the order of interaction process)
                         ("particle",np.int)]) # particle id (interacted particle)

# Note:
# Geant4 output data format is defined in ExN02TrackerSD.cc. 
# (Please modify ExN02TrackerSD.cc if you want to change data format.)

# [Used information]
# module's id ... (deafult:0) if using muptiple Compton cameras, this indicates in which Comtpon camera the interaction is occured.
# box's id ... 0=Scatter, 1=Absorber 
# voxel's id ... 0-63 corresponding to each GAGG crystal.
# edep ... energy deposition at the specific GAGG crystal.

# [Not used information]
# You don't have to analize 'posx', 'posy', 'posz', 'time', 'track', 'particle',
# These are information only for simulation, and can be used for simulation check.
# These are the information for interaction at last step for the GAGG crystal.

def main():

    filname = sys.argv[1]

    # load data
    data = np.genfromtxt(filname,delimiter='\t',skip_header=1,dtype=geant4_dtype)
    print data[0]

    # pixel hear map
    voxel = data["voxel"]+64*data["box"] # absorber voxel -> 64-127
    plt.figure()
    x = np.arange(129)
    y = np.histogram(voxel,bins=x)[0]
    map = y.reshape((16,8))
    plt.pcolor(map)

    # energy spectrum (ideal energy resolution)
    plt.figure()
    x = np.linspace(0,0.700,100)
    y = np.histogram(data["edep"],bins=x)[0]
    plt.plot(x[:-1],y)
    plt.xlabel("Energy [MeV]")
    plt.ylabel("Counts [#]")

    # position calibration (module,box,voxel) -> (x,y,z)
    scamask = data["box"]==0
    absmask = data["box"]==1
    data["posx"][scamask] = 102.5 # distance_source_sca+scavoxel_sizeX/2
    data["posx"][absmask] = 190.  # distance_source_sca+scavoxel_sizeX+distance_sca_abs+absvoxel_sizeX/
    data["posy"] = (data["voxel"]/8)*10. - 35. # (i+0.5)*scavoxel_sizeY-scabox_sizeY*0.5
    data["posz"] = (data["voxel"]%8)*10. - 35. # (j+0.5)*scavoxel_sizeZ-scabox_sizeZ*0.5
    # x,y,z after converting
    plt.figure()
    plt.hist(data["posx"],bins=10,label="x")
    plt.hist(data["posy"],bins=8,label="y")
    plt.hist(data["posz"],bins=8,label="z")
    plt.legend()
    plt.show()

if __name__=='__main__':
    main()
    sys.exit("Fin: geatn4 simulation analysis!")
