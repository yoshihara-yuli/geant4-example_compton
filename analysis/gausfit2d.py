""" gaussian fitting for 2D Cross section """

import sys,os
import numpy as np
import matplotlib.pylab as plt
import matplotlib.cm as cm
from scipy.optimize import curve_fit

def main():
    
    data = np.load(sys.argv[1])
    #print(data)
    data = data/data.max()
    xbin = int(sys.argv[2])

    gausfit2d(data,xbin,ybin=0.,xmin=-1,xmax=-1,ymin=-1,ymax=-1)


def gausfit2d(data,xbin,ybin=0.,xmin=-1,xmax=-1,ymin=-1,ymax=-1):

    # -- parameters
    #(these parameters should be input when you import and use it.)
    if ybin==0: ybin=xbin
    
    index_row_max, index_col_max = np.where(data == data.max())

    print "peak index is (row,col) = (%d,%d)"%(index_row_max[0],index_col_max[0])
    print data[index_row_max[0]][index_col_max[0]]
    
    # --------------------------------------
    # Row
    print "========== Row fitting... ==========="
    # Gaussian fitting
    xwidth,ywidth = np.shape(data)
    if xmin==-1 and xmax==-1:
        size = xwidth/2*xbin
        xmin,xmax=-size,size
 
    x=np.linspace(xmin,xmax,xwidth)
    y=data[index_row_max[0]]

    print(len(x),len(y))

    mean,sigma,base = 0,30.,0.1
    print "Initial (mean, sigma, base) = (%s,%s,%s)"%(mean, sigma, base)

    def gaus(x,a,x0,sigma,b):
        return a/((2*np.pi*sigma)**0.5) * np.exp(-(x-x0)**2/(2*sigma**2)) + b
    
    plt.figure();
    plt.plot(x,y,'b',linestyle='steps-pre',label='data')
    popt, pcov = curve_fit(gaus,x,y,p0=[1,mean,sigma,base])
    mean, sigma, base = popt[1], popt[2],popt[3]
    err_mean = np.sqrt(pcov[1][1])
    err_sigma = np.sqrt(pcov[2][2])
    err_base = np.sqrt(pcov[3][3])
    plt.plot(x,gaus(x,*popt),'r',linewidth=2,label='fit')
    plt.legend()
    
    print "New (mean, sigma, base) = (%s,%s,%s)"%(mean, sigma, base)

    plt.text(-(size-5), 0.90, 'Mean: %.3f +/- %.3f'%(mean, err_mean),
             ha = 'left', va = 'top', fontsize=16)
    plt.text(-(size-5), 0.82, 'Sigma: %.3f +/- %.3f'%(sigma, err_sigma),
             ha = 'left', va = 'top', fontsize=16)
    plt.text(-(size-5), 0.74, 'FWHM: %.3f +/- %.3f'%(sigma*2.35,err_sigma*2.35),
             ha = 'left', va = 'top', fontsize=16)
    plt.text(-(size-5), 0.66, 'Base: %.3f +/- %.3f'%(base, err_base),
             ha = 'left', va = 'top', fontsize=16)

    plt.xlabel('y [mm]')
    plt.ylabel('Normalized intensity [a.u]')

    plt.xlim([xmin,xmax])
    plt.ylim([0,1.05])
    plt.savefig('yfit.eps',format='eps',dpi=1000)
 
    # --------------------------------------
    # Column   
    print "========== Column fitting... ===========";
    # Gaussian fitting
    y=data.T[index_col_max[0]];

    if ymin==-1 and ymax==-1:
        size = ywidth/2*ybin
        ymin,ymax=-size,size
    
    x=np.linspace(xmin,xmax,xwidth)
    y=data.T[index_row_max[0]]
    print "Initial (mean, sigma) = (%s,%s)"%(mean, sigma)

    plt.figure()
    plt.plot(x,y,'b',linestyle='steps-pre',label='data')
    popt, pcov = curve_fit(gaus,x,y,p0=[1,mean,sigma,base])#
    mean, sigma, base = popt[1],popt[2],popt[3]# input optimized parameters
    err_mean  = np.sqrt(pcov[1][1])
    err_sigma = np.sqrt(pcov[2][2])
    err_base = np.sqrt(pcov[3][3])
    plt.plot(x,gaus(x,*popt),'r',linewidth=2,label='fit') #'ro:'
    plt.legend()
    
    print "New (mean, sigma, base) = (%s,%s,%s)"%(mean, sigma, base)


    plt.text(-(size-5), 0.90, 'Mean: %.3f +/- %.3f'
             %(mean, err_mean),ha = 'left', va = 'top', fontsize=16)
    plt.text(-(size-5), 0.82, 'Sigma: %.3f +/- %.3f'%(sigma, err_sigma),
             ha = 'left', va = 'top', fontsize=16)
    plt.text(-(size-5), 0.74, 'FWHM: %.3f +/- %.3f'%(sigma*2.35, err_sigma*2.35),
             ha = 'left', va = 'top', fontsize=16)
    plt.text(-(size-5), 0.66, 'Base: %.3f +/- %.3f'%(base, err_base),
             ha = 'left', va = 'top', fontsize=16)

    plt.xlim([ymin,ymax])
    plt.ylim([0,1.05])

    plt.xlabel('z [mm]')
    plt.ylabel('Normalized intensity [a.u]')
    plt.savefig('zfit.eps',format='eps',dpi=1000)
    plt.show();
    
if __name__=='__main__':
    main()
    sys.exit('Fin!');
