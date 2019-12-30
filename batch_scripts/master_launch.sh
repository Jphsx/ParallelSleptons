#/bin/sh

#sbatch job_launch.sh testdata_R1 KUAnalysis DY
#for signal ive repurposed TAGS to be the input treename list
#sbatch job_launch.sh testdata_R2 SMS SS

#DY JOBS
sbatch job_launch.sh DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-4to50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY
sbatch job_launch.sh DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X KUAnalysis DY


exit
