""" Multiple Reconstruction Created on 10/19/2018 """
import os,sys
import csv
import numpy as np
import time 
import matplotlib.pylab as plt
import matplotlib.cm as cm
import datetime
import glob 
#import gausfit2d as gf2d
plt.rcParams["font.size"] = 18
# ==========================================
class Vector:
    def __init__(self,x,y,z):
        self.x = x
        self.y = y
        self.z = z
    def Norm(self):
        size = np.sqrt(self.x*self.x + self.y*self.y+self.z*self.z)
        self.x = self.x/size
        self.y = self.y/size
        self.z = self.z/size

class Event:
    def __init__(self,Es,Ea,E0):
        self.Ea = Ea
        self.Es = Es
        self.Omega = np.arccos((1.+511.*(((1.)/((Es)+(E0-Es)))-((1.)/((E0-Es))))))
        #self.Omega = np.arccos((1.+511.*(((1.)/((Es)+(Ea)))-((1.)/((Ea))))))
        
def MakeAxis(obj1,obj2):
    obj3 = Vector(0.,0.,0.)
    obj3.x = obj1.x - obj2.x
    obj3.y = obj1.y - obj2.y
    obj3.z = obj1.z - obj2.z
    return obj3

def Normal(X,Y,Z):
    size = (X*X + Y*Y + Z*Z)**0.5
    X = X/size
    Y = Y/size
    Z = Z/size
    return X,Y,Z

def GetInnerAngle2(X,Y,Z,obj):
    X,Y,Z = Normal(X,Y,Z)
    return np.arccos(X*obj.x + Y*obj.y + Z*obj.z)

def Gaussian2(O1,O2,ARM):
    index,mean,sig = np.fabs(O1-O2),0.,(ARM/2.35)*np.pi/180.
    val = np.exp(-(0.5*np.multiply((index-mean)/sig,(index-mean)/sig)))
    return val

def Project2D(first,second,X=[],Y=[],Z0=0.,cnt=0,Arm=10.,fwhm=0.12):

    E = 662.
    dE = E*fwhm/2.
    xs,ys,zs,es = first['posy'],first['posz'],first['posx'],first['edep']
    xa,ya,za,ea = second['posy'],second['posz'],second['posx'],second['edep']
    
    if 60<es and es<(E/(1+2*E/511.)) and (E-dE)<(es+ea) and (es+ea)<(E+dE):

        cnt += 1
        
        Scatter  = Vector(xs,ys,zs)
        Absorber = Vector(xa,ya,za)
        Energy   = Event(es,ea,E)
        Axis = MakeAxis(Scatter,Absorber)
        Axis.Norm()

        TempMap = np.zeros((len(X),len(Y)))
        return Mapping2D(TempMap,X,Y,Z0,Scatter,Axis,Energy.Omega,Arm),cnt

    else: 
        return np.zeros((len(X),len(Y))),cnt

def Mapping2D(TempMap,X,Y,Z0,Scatter,Axis,Omega,Arm):

    Z = np.ones(np.shape(X))*Z0
    InnerAngle = GetInnerAngle2(X-Scatter.x,Y-Scatter.y,Z-Scatter.z,Axis)
    TempMap    = Gaussian2(InnerAngle,Omega,Arm)

    TempMap[np.isnan(TempMap)]=0.
    if TempMap.max()>0.:
        TempMap = TempMap/TempMap.max()
        #TempMap = TempMap/Omega
        return TempMap
    else:
        return np.zeros(np.shape(TempMap))

def SetDisplay(xmin,xmax,ymin,ymax,n_bins,width=50):

    cbar = plt.colorbar()
    ax = plt.gca()
    ax.invert_yaxis()
    plt.clim(0,1)
    cbar.set_label('Normalized intensity [a.u.]')
    x = np.linspace(xmin,xmax,n_bins)
    xbin_width = np.diff(x)[0]
    y = np.linspace(ymin,ymax,n_bins)
    ybin_width = np.diff(y)[0]
    plt.xticks(np.arange(0,n_bins,width),np.arange(xmin,xmax,width*xbin_width))
    plt.yticks(np.arange(0,n_bins,width),np.arange(ymin,ymax,width*ybin_width))
    plt.xlabel('y [mm]')
    plt.ylabel('z [mm]')

###############################

def main():
    
    time0=time.clock()
    
    # -- setting for input file
    firsts = np.load(sys.argv[1])
    seconds = np.load(sys.argv[2])
    
    # -- projection parameters
    xmin,xmax = -200,200
    ymin,ymax = -200,200
    #zmin,zmax = 10,510
    bins = 201
    x = np.linspace(xmin,xmax,bins)
    y = np.linspace(ymin,ymax,bins)
    #z = np.linspace(zmin,zmax,bins)
    
    Xy = np.meshgrid(x,y)[0]
    Yx = np.meshgrid(x,y)[1]
    map_xy = np.zeros(np.shape(Xy))
    
    X0,Y0,Z0 = 0.,0.,0.
    
    cnt = 0
    
    for k in range(len(firsts)):
                
       tempmap,cnt = Project2D(firsts[k],seconds[k],Xy,Yx,Z0,cnt,Arm=4.)
       map_xy = map_xy + tempmap
       td = time.clock()-time0
                
       sys.stdout.write('\r')
       sys.stdout.write('Event # = %d, Cone # = %d, Time = %.2f sec.'%(k,cnt,td))
       sys.stdout.flush()

    print("Finish all data: %d"%k)

    np.save("bp_map.npy",map_xy)

    plt.figure()
    plt.imshow(map_xy[::-1,:]/map_xy.max(),cmap=cm.gist_earth,aspect='equal')
    SetDisplay(xmin,xmax,ymin,ymax,bins)
    plt.tight_layout()
    plt.savefig("map2d.eps",format='eps',dpi=100)
    plt.show()
                   
##################################
if __name__ =='__main__':
    main()
    sys.exit('Fin ! :)')
