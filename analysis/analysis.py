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

    data["edep"] = data["edep"]*1000. # MeV -> keV

    # pixel hear map
    voxel = data["voxel"]+64*data["box"] # absorber voxel -> 64-127
    plt.figure()
    x = np.arange(129)
    y = np.histogram(voxel,bins=x)[0]
    map = y.reshape((16,8))
    plt.pcolor(map)

    # energy spectrum
    plt.figure()
    x = np.linspace(0,800,201)
    y = np.histogram(data["edep"],bins=x)[0]
    plt.plot(x[:-1],y,'b.',linestyle='solid')
    plt.xlabel("Energy [keV]")
    plt.ylabel("Counts [#]")

    plt.figure()
    plt.title("Ideal position")
    plt.hist(data["posx"],bins=30,label="x")
    plt.hist(data["posy"],bins=28,label="y")
    plt.hist(data["posz"],bins=28,label="z")
    plt.legend()

    # position calibration (module,box,voxel) -> (x,y,z)
    scamask = data["box"]==0
    absmask = data["box"]==1
    distance_source_sca = 400.
    scavoxel_sizeX = 5.
    absvoxel_sizeX = 10.
    distance_sca_abs = 110. ### parameters for this simulation
    print(distance_source_sca+scavoxel_sizeX*0.5)
    print(distance_source_sca+scavoxel_sizeX+distance_sca_abs+absvoxel_sizeX*0.5)

    data["posx"][scamask] = distance_source_sca+scavoxel_sizeX*0.5 # 400+2.5 = 402.5
    data["posx"][absmask] = distance_source_sca+scavoxel_sizeX+distance_sca_abs+absvoxel_sizeX*0.5 # 400+5+X+5 = X+410
    data["posy"] = (data["voxel"]/8)*11. - 37.5 # (i+0.5)*scavoxel_sizeY-scabox_sizeY*0.5
    data["posz"] = (data["voxel"]%8)*11. - 37.5 # (j+0.5)*scavoxel_sizeZ-scabox_sizeZ*0.5

    # x,y,z after converting
    plt.figure()
    plt.title("Calibrated position")
    plt.hist(data["posx"],bins=30,label="x")
    plt.hist(data["posy"],bins=28,label="y")
    plt.hist(data["posz"],bins=28,label="z")
    plt.legend()

    #plt.show()
    #sys.exit()

    j = 0
    firsts = np.zeros([],dtype=geant4_dtype)
    seconds = np.zeros([],dtype=geant4_dtype)
    for i,id_ in enumerate(np.unique(data["event"])):
        mask = data["event"]==id_
        if np.sum(mask)==2 and np.sum(data[mask]["box"])==1: # two coincidences and interplane
            j += 1
            datalet = data[mask]
            firsts  = np.hstack((firsts,datalet[datalet["box"]==0]))
            seconds = np.hstack((seconds,datalet[datalet["box"]==1]))
        if i%1000==0: print("i: %d, event id: %d"%(i,id_))
        #if i==10000: break
            
    print("All events: %d, Extracted interplane coincident events: %d (%.2f)"%(i,j,float(i)/j*100.))

    np.save("firsts.npy",firsts)
    np.save("seconds.npy",seconds)

    plt.figure()
    plt.title("Correlation plot")
    xmin,xmax,xbin = 0,900,10
    X = np.arange(xmin,xmax,xbin)
    xx = np.arange((xmax-xmin)/xbin)
    h,x,y=np.histogram2d(seconds['edep'],firsts['edep'],bins=X)
    plt.imshow(np.log(h+1)[:-1,:-1],interpolation='nearest')
    ax = plt.gca()
    ax.invert_yaxis()
    plt.colorbar()
    plt.xlabel('Scatter [keV]')
    plt.ylabel('Absorber [keV]')
    plt.xticks(xx[::20],X[::20])
    plt.yticks(xx[::20],X[::20])

    plt.show()

if __name__=='__main__':
    main()
    sys.exit("Fin: geatn4 simulation analysis!")
