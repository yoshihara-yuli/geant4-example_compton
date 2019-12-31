#!/bin/sh

# Parameters for scan
N=1000 # nunber of scan
REPEAT=1000000  # number of beam
DIR="data/" # directory for saved data
MACROFIL="run_randpoint.mac"
OUTFIL="${DIR}pos.info"
HEAD="centre posx posy posz mm"
echo "${OURFIL}"

# Make directory for saved data
mkdir -p ${DIR}

# Make Output file for source position
touch ${OUTFIL}
echo ${HEAD} >${OUTFIL}

# Replace /run/beamOn XX to the designated repeating times.
OLD="beamOn.*"                                                                             
NEW="beamOn "$REPEAT   
sed -i -e "s/${OLD}/${NEW}/g" run.mac

# Scan, simulate, and save data
for i in `seq 0 ${N}`
do

  echo "${i}-times loop"

  # replace words
  OLD="centre.*"
  #NEW="centre ${POSX} ${POSY} 0 cm
  NEW=`python -c "import rand_source as rc; rc.make_cylinder()"`
  echo ${NEW}
  echo ${NEW} >>${OUTFIL}
  
  sed -i  -e "s/${OLD}/${NEW}/g" ${MACROFIL}

  # run simulations
  time ./exampleB3 ${MACROFIL} > /dev/null 2>&1
  mv result.txt ${DIR}result_3_pipe-air_img-"$i".txt

done

echo "done"



