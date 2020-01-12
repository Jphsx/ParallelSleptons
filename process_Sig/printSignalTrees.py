#this is a simple utility to print out mass point ttrees 
#the lists are manually copied and pasted into lists to parse and superimpose

from ROOT import TFile

f = TFile.Open("/home/j342a201/work/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");

#f.Print();
keys = f.GetListOfKeys()

print "dm10 list"
for key in keys:
	name = key.GetName()	
	name = name.split("_")
	
	if(name[0] == "SMS"):
	#	print name
		m1 = int(name[1])
		m2 = int(name[2])
		if( abs(m1-m2) == 10 ):
			print "_".join(name)		

print "dmX list 10 GeV"
dm = [5,10,20,30,40,50,60]
for key in keys:
	name = key.GetName()
	name = name.split("_")	
	if(name[0] == "SMS"):
		m1 = int(name[1])
		m2 = int(name[2])
		for x in dm:
			if( abs(m1-m2) == x ):
				print "_".join(name) +" "+ str(x)
				

