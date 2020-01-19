
def make_cylinder():
    import os,sys
    import numpy as np

    theta = np.random.rand()*np.pi
    L = (np.random.rand()*7/2+41.6/2)
    posX = L*np.cos(theta)
    posY = (np.random.rand()*50 - 25)
    posZ = L*np.sin(theta)

    print("centre %.3f %.3f %.3f mm"%(posX,posY,posZ))

def main():
    make_cylinder()

if __name__=="__main__":
    main()
