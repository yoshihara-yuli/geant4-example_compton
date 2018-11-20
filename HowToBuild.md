
# HowToBuild

How to build and run this geant4 sample code.

#### 1. Change the current directory to “geant4-example\_compton"  

```
$ cd geant4-example_compton**
```


#### 2. Create a directory of "build", and change the current directory to "build" directory.  

```
$ mkdir build
$ cd build
```

#### 3. Cmake  

```
$ cmake -DGeant4_DIR=/Home/Geant4/einstall/lib64/Geant4-9.6.4 ../source   
```

 - The path of **-DGeant4_DIR (/Home/Geant4/...) should be changed according to your geant4 installation location**.
 - If cmake doesn't suceeded because of some of error, please **remove invisible files such as ~\*.cc or ~\*.hh in source/src or source/include**, which might cause cmake errors.  </font>


#### 4. Make  (Build)

```
$ make
```  

#### 5. Check geometry and beam condition with Qt.  

```
$ ./exampleB3
```

In Qt window, you can check beam condition by running the macro file by put **/control/execute run1.mac** in Session.

#### 6. Obtain data with macro file **run1.mac**  

```
$ ./exampleB3 run.mac
```

or

```
$ ./exampleB3 run.mac > /dev/null 2>&1
```


##### Caution!
Please **build (make) again** whenever changing source files or header files, otherwise, those changes happened after the last make (build), would not be reflected to the simulation.
