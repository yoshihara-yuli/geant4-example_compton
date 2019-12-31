#!/bin/sh

# Parameters for scan
N=1000 # nunber of scan
DIR="data/" # directory for saved data`
# Make directory for saved data
mkdir -p ${DIR}

# Scan, simulate, and save data
for i in `seq 0 ${N}`
do

  echo "${i}-times loop"

  time ./exampleB3 run.mac > /dev/null 2>&1
  mv result.txt ${DIR}result_3_pipe-air_img-"$i".txt

done

echo "done"



