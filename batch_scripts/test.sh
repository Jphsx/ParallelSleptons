#/bin/sh

sbatch job_launch.sh testdata_R1 KUAnalysis DY
#for signal ive repurposed TAGS to be the input treename list
#sbatch job_launch.sh testdata_R2 SMS SS

exit
