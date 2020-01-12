
#this is a utlility that takes in a mass point tree list, super imposes histograms with treename/tags and outputs them into a root file
from ROOT import TFile, TCanvas, TLegend
import sys


listfilename = sys.argv[1]
outfileopt = sys.argv[2]
histname = sys.argv[3]
color = int(sys.argv[4])

flist = open(listfilename)
froot= TFile.Open("/home/j342a201/slepton/ParallelSleptons/ExecutionDirectory/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_0/susyHists.root");

#master color/marker number lists
c = color
fillcol = [c, c+1, c+2, c+3, c+4, c+5]
markstyle = [20,22,21,29,39,48]

legendTitles = []
hists = []

maximums =[]
#read a tag from the list
lines = flist.readlines()
i = 0
for line in lines:
	line = line.rstrip('\n')
	h = froot.Get(line+histname)
	h.SetMarkerColor(fillcol[i])
	h.SetMarkerStyle(markstyle[i])
	h.SetMarkerSize(3)
	maximums.append( h.GetMaximum() )
	i = i + 1
	#exract naming
	tname = line.split("_")
	dm = abs(int(tname[1])-int(tname[2]))
	legendTitles.append("_".join(tname)+" "+"#Delta m = "+str(dm))
	hists.append(h)
#print maximums
#print legendTitles

dmname = listfilename[0:-5]
print "writing "+dmname+" " +outfileopt
frootout = TFile.Open("superSusy_"+dmname+".root",outfileopt)
c = TCanvas(histname)
#loop and draw max first
imax = maximums.index(max(maximums))
hists[imax].Draw()
for ih in range(len( hists)):
	if ih == imax:
		continue
	hists[ih].Draw("same")

#craft TLegend
ymax, xlmin, ylmin, dx, dy = 800.0, 0.7, 0.7, 0.28, 0.26
leg = TLegend(xlmin,ylmin,xlmin+dx,ylmin+dy)
for ih in range(len( hists)):
	leg.AddEntry(hists[ih],legendTitles[ih],"p")

leg.SetBorderSize(1)
leg.Draw()
c.Write()
frootout.Save()
frootout.Close()
	
