
# HowToBuild

How to build and run this geant4 sample code.

1. Change the current directory to “compton_gagg"  
**\$ cd geant4-example\_compton**

2. Make a directory of "build", and change the current directory to "build"  
**\$ mkdir build**  
**\$ cd build**  

2. Run the below cmake command  
	**\$	cmake -DGeant4_DIR= /Home/Geant4/einstall/lib64/Geant4-9.6.4 ../source**   
<font color="Blue">- The path of -DGeant4_DIR (/Home/Geant4/...') should be changed according to your geant4 installation location. </font>   
<font color="Blue">- If cmake doesn't suceeded because of some of error, please remove invisible files such as ~\*.cc or ~\*.hh in source/src or source/include, which might cause cmake errors.  </font>


3. Run make  
**\$ make**  

4. Check geometry and beam condition with Qt.  
**\$ ./exampleB3**   
<font color="Blue">(In Qt window, you can check beam condition by running the macro file like **execute/run run1.mac**.) </font>

5. Obtain data with macro file **run1.mac**  
**$ ./exampleB3 run1.mac** 

Please **run make** whenever changing source files or header files, otherwise those changes after the last make would not be reflected to the simulation.
