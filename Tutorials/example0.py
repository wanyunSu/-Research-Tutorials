from ROOT import *
import numpy as np

MapHist = {}
#JetList[{"Type":"data","pT":800},{"Type":"quark","pT":1000}]

f1 = TFile("example0.root","recreate")

for i in range(100):
	h1 = TH1F("h"+str(i),"",60,0,5)
	h1.FillRandom("gaus",500)
	MapHist[h1.GetName()] = h1
	h1.Write()

#print MapHist["h2"].Draw()
#xx = input()
f1.Write()

