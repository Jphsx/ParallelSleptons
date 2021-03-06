#!/bin/bash
#SBATCH --job-name=testSleptons         # Job name
#SBATCH --partition=sixhour   # Partition Name (Required)
#SBATCH --mail-type=ALL                 # Mail events (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=j342a201@ku.edu     # Where to send mail	
#SBATCH --ntasks=1                      # Run 1 task on one node
#SBATCH --cpus-per-task=24              # Number of threads to use
#SBATCH --mem=2gb                       # Job memory request
#SBATCH --time=0-06:00:00               # Time limit days-hrs:min:sec
#SBATCH --output=test_%j.log     # Standard output and error log

VERSION=$1
TREE=$2
TAG=$3
echo 'Version '$VERSION

pwd
hostname
date

echo "PATH: "
echo $PATH
 
echo "LD_LIBRARY_PATH"
echo $LD_LIBRARY_PATH

echo "Run Test code "

python2 --version

#Need to source ROOT

. /home/j342a201/setenv.sh
echo $ROOTSYS
 
MYWDIR=/home/j342a201/slepton/ParallelSleptons/graham_testfiles/myTest
cd ${MYWDIR}
echo "Now in directory "
pwd

echo "Start execution"

./runmyanalysis.sh ${VERSION} ${TREE} ${TAG}
#./doHPC.sh

date

date

exit
