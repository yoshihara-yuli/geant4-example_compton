""" Basic analysis code for Geant4 simulation """

import os,sys
import numpy as np
import matplotlib.pylab as plt
import glob

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

def data_import(filname,format="txt"):
    data = np.genfromtxt(filname,delimiter='\t',skip_header=1,
                         dtype=geant4_dtype)
    filname_npy = filname.split(format)[0]+"npy"
    np.save(filname_npy,data)
    return data

def main():

    filname = sys.argv[1] # file or directory name
    beam_no = 1e+8 # sys.argv[2]

    if os.path.isfile(filname): # file
        format = filname.split(".")[-1]

        if format=="npy":
            print("npy file already created")
            data = np.load(filname)
            print data[0]

        else:
            data = data_import(filname)
            filname_npy = filname.split(format)[0]+"npy"
            np.save(filname_npy,data)
            sys.exit("fin. next action: python analysis %s"%filname_npy)

    else: # directory
        dirname = filname
        filnames = glob.glob(dirname+"/*.txt")
        for i,filname in enumerate(filnames):
            print(i,filname)
            format = filname.split(".")[-1]
            data = data_import(filname)
            filname_npy = filname.split(format)[0]+"npy"
            np.save(filname_npy,data)
        sys.exit("finish txt2npy for all %d files in %s"%(len(filnames),dirname))

    eth = 0 # energy threshold [MeV]
    mask = data["edep"]>=eth
    data_eth = data[mask]

    detect_no = len(np.unique(data_eth["event"]))

    print(beam_no*1.0/(detect_no*1.0))

    # pixel hear map
    voxel = data["voxel"] # absorber voxel -> 64-127
    print(np.max(data["voxel"]))
    plt.figure()
    x = np.arange(np.max(data["voxel"])+2)
    y = np.histogram(voxel,bins=x)[0]
    map = y.reshape((25,25))

    plt.figure()
    plt.pcolor(map)
    plt.colorbar()
    plt.savefig("heatmap.png")

    # energy spectrum (ideal energy resolution)
    plt.figure()
    x = np.linspace(0,1.400,100)
    y = np.histogram(data["edep"],bins=x)[0]
    plt.plot(x[:-1],y)
    plt.xlabel("Energy [MeV]")
    plt.ylabel("Counts [#]")
    plt.tight_layout()
    plt.savefig("energy.png")
    plt.show()

if __name__=='__main__':
    main()
    sys.exit("Fin: geatn4 simulation analysis!")
