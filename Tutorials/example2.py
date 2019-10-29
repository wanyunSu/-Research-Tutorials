from ROOT import *
import numpy as np
import random

#This script is used for creating histograms with differents variables of a certain type of jets.
#Jets can be categoried into leading jet / subleading jet according to their pT, and forward jet / central jet according to their eta.
#For each type of jets has two vaiables: BDT and Ntrk.
#In this example we randomly choose values from different gaussian distribution to represent BDT and Ntrk values 
#
# Author : Wanyun Su 2019-10-17


# declare lists of different jet type 
MapHist = {}  #dict with histogramis'name as keys and histograms as  values
JetList = ["leading","subleading"]
EtaList = ["forward","central"]

# define a function which give differnet bin range according to histogram name
def GetBin(variable):
	if variable == "Ntrk":
		return 60, 10, 30
	elif variable == "BDT":
		return 60, 4, 10


# define a function which create histogram with different combination of JetList and EtaList value
def CreateHist(prefix, variable, value, maphist):
	histname = prefix+"_"+variable
	if histname in maphist:   # check if the histogram already existed
		maphist[histname].Fill(value)
#		if variable == "Ntrk":
#			maphist[histname].Fill(random.gauss(20,2))
#		if variable == "BDT":
#			maphist[histname].Fill(random.gauss(5,2))
	else:
		nbin, bmin, bmax = GetBin(variable)
		h1 = TH1F(histname,"",nbin, bmin, bmax)
		maphist[histname] = h1
		maphist[histname].Fill(value)
#		maphist[histname].Fill(
#		if variable == "Ntrk":
#			maphist[histname].Fill(random.gauss(20,2))
#		if variable == "BDT":
#			maphist[histname].Fill(random.gauss(5,2))



# randomly choose the name of histograms and fill it
for i in range(100): #randomly choose 100 different value for JetList and EtaList
	jet = random.choice(JetList)
	eta = random.choice(EtaList)
	luckyName = jet+"_"+eta
	CreateHist(luckyName,"Ntrk", random.gauss(20,2), MapHist)
	CreateHist(luckyName,"BDT", random.gauss(5,2), MapHist)



# loop over to write and save histograms
f1 = TFile("example2.root","recreate")
for hist in MapHist.values():
	hist.Write()
	
f1.Write()
f1.Close()
