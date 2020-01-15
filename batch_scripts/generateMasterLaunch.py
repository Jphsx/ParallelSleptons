


#this script looks in datalist file and collects all of the lists
#the lists that are found in the master-key-object here are written into a master launch script



import sys
import os
import subprocess

def bash( bashCommand ):
        process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
        output, error = process.communicate()
        return output ,error




#create a master key for treename, tag, filekey
# "filekey:treename:tag"
#simple dictionary-like, a vector of strings to be split with a delimiter
masterkey = ["DYJetsToLL:KUAnalysis:DY", "TSlepSlep:SMS:SS", "TTJets:KUAnalysis:TT", "WJetsToLNu:KUAnalysis:WJ", "WWTo2L2Nu:KUAnalysis:WW" , "WWToLNuQQ:KUAnalysis:WW", "WZTo1L1Nu2Q:KUAnalysis:WZ", "WZTo1L3Nu:KUAnalysis:WZ", "WZTo2L2Q:KUAnalysis:WZ", "WZTo3LNu:KUAnalysis:WZ", "ZZTo2L2Nu:KUAnalysis:ZZ", "ZZTo2L2Q:KUAnalysis:ZZ", "ZZTo4L:KUAnalysis:ZZ" ]

cmd = "ls datalists"
LS = bash(cmd)
LS = LS[0].split('\n')


out = open("master_launch.sh","w+")

#LS = [f for f in LS if ".root" in f]
#look specifically for elements in the master key
for ikey in range(len(masterkey)):
	name = masterkey[ikey].split(':')[0]
	treename = masterkey[ikey].split(':')[1]
	tag = masterkey[ikey].split(':')[2]
	
	key_lists = [f for f in LS if name in f]
	
	for keylist in key_lists:
		keyname = keylist.rstrip()
		keyname = keyname[0:-5]
		out.write("sbatch job_launch.sh "+keyname+" "+treename+" "+tag+"\n")




out.close()
bash("chmod 777 master_launch.sh");	
