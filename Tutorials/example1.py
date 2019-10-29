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


#create histogram with different combination of JetList and EtaList value
for jet in JetList:
	for eta in EtaList:
		h1 = TH1F(jet+"_"+eta+"_Ntrk","",60,10,30)
		h2 = TH1F(jet+"_"+eta+"_BDT","",60,4,10)

		MapHist[h1.GetName()] = h1
		MapHist[h2.GetName()] = h2


# randomly choose the name of histograms and fill it
for i in range(100): #randomly choose different value from List
	jet = random.choice(JetList)
	eta = random.choice(EtaList)
#	var = random.choice(VarList)
	jetLabel = jet+"_"+eta#+"_"#+var
	if jetLabel+"_Ntrk" in MapHist:
	# check whether jetLab+"_Ntrk" exists in the MapHist then
		#if "Ntrk" in jetLabel:
		MapHist[jetLabel+"_Ntrk"].Fill(random.gauss(20,2))  
		#elif "BDT" in jetLabel:
		MapHist[jetLabel+"_BDT"].Fill(random.gauss(5, 2))
	else:
		print "Error "+jetlabel+" doesn't exist"
		continue

# reference codes
##	for histname in MapHist.keys():
##		if jet in histname and eta in histname: #check if the histogram contains the random picked up name
##			print jet, eta, MapHist[histname] 
##			MapHist[histname+"_Ntrk"].FillRandom("gaus",500)
##			MapHist[histname+"_BDT"].FillRandom("gaus",500)


#Create output file
#loop over to write and save histograms
f1 = TFile("example1.root","recreate")
for hist in MapHist.values():	#get the histogram
	hist.Write()
	
f1.Write()
f1.Close()

