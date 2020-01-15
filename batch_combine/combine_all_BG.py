
import sys
import os
import subprocess
from operator import add

def bash( bashCommand ):
        process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
        output, error = process.communicate()
        return output ,error


#CURRENTLY ONLY RUN THIS ON BG, signal is already settled in 1 job (for now)

#masterkey = ["DYJetsToLL:KUAnalysis:DY", "TSlepSlep:SMS:SS", "TTJets:KUAnalysis:TT", "WJetsToLNu:KUAnalysis:WJ"]
#masterkey = ["DYJetsToLL:KUAnalysis:DY", "TTJets:KUAnalysis:TT", "WJetsToLNu:KUAnalysis:WJ"]
masterkey = ["DYJetsToLL:KUAnalysis:DY", "TTJets:KUAnalysis:TT", "WJetsToLNu:KUAnalysis:WJ", "WWTo2L2Nu:KUAnalysis:WW" , "WWToLNuQQ:KUAnalysis:WW", "WZTo1L1Nu2Q:KUAnalysis:WZ", "WZTo1L3Nu:KUAnalysis:WZ", "WZTo2L2Q:KUAnalysis:WZ", "WZTo3LNu:KUAnalysis:WZ", "ZZTo2L2Nu:KUAnalysis:ZZ", "ZZTo2L2Q:KUAnalysis:ZZ"  ]


#get a list of the job directories
cmd = "ls ./"
LS = bash(cmd)
LS = LS[0].split('\n')
PATH = "./"

w_table = []
now_table = []
histfile_list = []
tableheader_w = []
tableheader_now = []

tempname = masterkey[0].split(':')[0]
temptag = masterkey[0].split(':')[2]
with open(PATH+tempname+"/EventSelTables_"+temptag+".table","r") as f_in :
	lines = (line.rstrip() for line in f_in)
	lines = list(line for line in lines if line)
	cut_list = lines[1].split(" ")
        w_table = [0.] * len(cut_list)
        now_table = [0.] * len(cut_list)
        
        tableheader_now = lines[0]
        tableheader_w = lines[3]
	tableheader_now = tableheader_now.split(" ");
	tableheader_now[0] = "All"
	tableheader_w = tableheader_w.split(" ");
	tableheader_w[0] = "All"

        f_in.close()


#parse the master key
for ikey in range(len(masterkey)):
        name = masterkey[ikey].split(':')[0]
        treename = masterkey[ikey].split(':')[1]
        tag = masterkey[ikey].split(':')[2]

	#make a sublist of jobdirectories that match current keyname
	key_lists = [f for f in LS if name in f]


	#a key is the job directory name
	for key in key_lists:

		with open(PATH+key+"/EventSelTables_"+tag+".table","r") as f_in :
			lines = (line.rstrip() for line in f_in) 
			lines = list(line for line in lines if line)


		#first loop get unweighted
		for linenum in range(len(lines)):
			#a table occurs every 3 lines
			#print line	
			lineparse = lines[linenum].split(" ")
			if lineparse[2] == "no_w":
				#this is a no_w table
				temp_map = list(map(float, lines[linenum+2].split(" ") ))
				#print temp_map
				#add in the map
				now_table = list(map(add, now_table, temp_map))
				
			if lineparse[2] == "w":
				#this is a w table
				temp_map = list(map(float, lines[linenum+2].split(" ") ))
				w_table = list(map(add, w_table, temp_map))

			linenum = linenum + 2	

		#while looping over keys collect the root files
		histfile_list.append(PATH+key+"/susyHists.root")

	print now_table
	print w_table	
	#print histfile_list

	#specify output path and create key directory for output if it doesnt exist
#bash("mkdir "+name)
out = open("All_BG/EventSelTables_All.table","w+")
out.write(" ".join(map(str,tableheader_now))+"\n")
out.write(" ".join(map(str, cut_list))+"\n")
out.write(" ".join(map(str, now_table))+"\n")
out.write("\n");
out.write(" ".join(map(str,tableheader_w))+"\n")
out.write(" ".join(map(str, cut_list))+"\n")
out.write(" ".join(map(str, w_table))+"\n")
out.write("\n")	
out.close()

outroot = "All_BG/susyHists.root"
bash("hadd -f "+outroot+" "+" ".join(map(str, histfile_list)) )





		

