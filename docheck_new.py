from ROOT import *
import numpy as np

'''
Things to try: 
- seems like fq and fg are both shifted - maybe tracking efficiency?  What if you reweight that?
- do maximum likelihood instead of solving system?
- also repeat at truth level?

'''

doreweight = 1

def myText(x,y,text, color = 1):
    l = TLatex()
    l.SetTextSize(0.025)
    l.SetNDC()
    l.SetTextColor(color)
    l.DrawLatex(x,y,text)
    pass




ntrackall = TFile("dijet_sherpa_d_py.root")
ntrackall2 = TFile("dijet_sherpa_d_j1234.root")

forward_quark = ntrackall.Get("HighEta_QuarkJet_Ntrk_500_to_600")
forward_gluon = ntrackall.Get("HighEta_GluonJet_Ntrk_500_to_600")
central_quark = ntrackall.Get("LowEta_QuarkJet_Ntrk_500_to_600")
central_gluon = ntrackall.Get("LowEta_GluonJet_Ntrk_500_to_600")

forward_data = ntrackall2.Get("HighEtaJet_Ntrk_500_to_600_Data")
central_data = ntrackall2.Get("LowEtaJet_Ntrk_500_to_600_Data")
#forward_data_stat = ntrackall2.Get("HighEtaJet_Ntrk_500_to_600_Data")
#central_data_stat = ntrackall2.Get("LowEtaJet_Ntrk_500_to_600_Data")
#print  forward_data_stat.GetBinContent(10), central_data_stat.GetBinContent(10)

forward_quark_s = ntrackall2.Get("HighEta_QuarkJet_Ntrk_500_to_600")
forward_gluon_s = ntrackall2.Get("HighEta_GluonJet_Ntrk_500_to_600")
central_quark_s = ntrackall2.Get("LowEta_QuarkJet_Ntrk_500_to_600")
central_gluon_s = ntrackall2.Get("LowEta_GluonJet_Ntrk_500_to_600")

j1_pt_mc_f = ntrackall2.Get("Jet1_or_Jet2_highEta_AllJet_pT_MC")
j1_pt_mc_c = ntrackall2.Get("Jet1_or_Jet2_lowEta_AllJet_pT_MC")
#j1_pt_frac_f = ntrackall2.Get("Jet1_or_Jet2_highEta_QuarkJet_percentage_pT")
#j1_pt_frac_c = ntrackall2.Get("Jet1_or_Jet2_lowEta_QuarkJet_percentage_pT")
j1_pt_frac_f = ntrackall2.Get("Jet1_or_Jet2_highEta_GluonJet_percentage_pT")
j1_pt_frac_c = ntrackall2.Get("Jet1_or_Jet2_lowEta_GluonJet_percentage_pT")

fg = j1_pt_frac_f.GetBinContent(8)
cg = j1_pt_frac_c.GetBinContent(8)
fq = 1.-fg
cq = 1.-cg

c = TCanvas("c","c",500,500)
pdfList = []
for j in range(101):
	forward_quark = ntrackall.Get("HighEta_QuarkJet_Ntrk_500_to_600_pdf"+str(j))
	forward_gluon = ntrackall.Get("HighEta_GluonJet_Ntrk_500_to_600_pdf"+str(j))
	central_quark = ntrackall.Get("LowEta_QuarkJet_Ntrk_500_to_600_pdf"+str(j))
	central_gluon = ntrackall.Get("LowEta_GluonJet_Ntrk_500_to_600_pdf"+str(j))

	if (doreweight):
		for i in range(1,forward_quark.GetNbinsX()+1):
			if (central_quark.GetBinContent(i) > 0 and central_gluon.GetBinContent(i) > 0):
				#print i,forward_quark.GetBinContent(i)/central_quark.GetBinContent(i),forward_gluon.GetBinContent(i)/central_gluon.GetBinContent(i)
		#        factor_gluon = forward_gluon.GetBinContent(i)/central_gluon.GetBinContent(i)
				factor_quark = forward_quark.GetBinContent(i)/central_quark.GetBinContent(i)
				central_quark.SetBinContent(i,central_quark.GetBinContent(i)*factor_quark)
				central_gluon.SetBinContent(i,central_gluon.GetBinContent(i)*factor_quark)
				pass
			pass
		pass
	
	central_quark.Scale(1./central_quark.Integral())
	central_gluon.Scale(1./central_gluon.Integral())
	forward_quark.Scale(1./forward_quark.Integral())
	forward_gluon.Scale(1./forward_gluon.Integral())

	forward = forward_quark.Clone("")
	central = forward_quark.Clone("")

	for i in range(1,forward.GetNbinsX()+1):
		forward.SetBinContent(i,fg*forward_gluon.GetBinContent(i)+fq*forward_quark.GetBinContent(i))
		central.SetBinContent(i,cg*central_gluon.GetBinContent(i)+cq*central_quark.GetBinContent(i))
		pass

	#Now, let's solve.

	quark = forward_quark.Clone("")
	gluon = forward_quark.Clone("")

	for i in range(1,forward.GetNbinsX()+1):
		F = forward.GetBinContent(i)
		C = central.GetBinContent(i)
		Q = -(C*fg-F*cg)/(cg*fq-fg*cq)
		G = (C*fq-F*cq)/(cg*fq-fg*cq)
		quark.SetBinContent(i,Q)
		gluon.SetBinContent(i,G)
		#print "   ",i,G,forward_gluon.GetBinContent(i),central_gluon.GetBinContent(i)
		pass
	pdfList.append(quark)
	#pdfList.append(gluon)
	if j > 0 :
		pdfList[j].Divide(pdfList[0])

rms = 0.
#x = []
gluon_pdf = pdfList[0].Clone("")
gluon_pdf2 = pdfList[0].Clone("")
quark_pdf = pdfList[0].Clone("")
quark_pdf2 = pdfList[0].Clone("")
for i in range(1,pdfList[0].GetNbinsX()+1):
	x = 0.
	for j in range(101):
		x += (pdfList[j].GetBinContent(i) - pdfList[0].GetBinContent(i))**2
		#x.append((pdfList[j].GetBinContent(i) - pdfList[0].GetBinContent(i)))
	#y = np.array(x)
	#rms = np.sqrt(np.mean(y**2))
	rms = TMath.Sqrt(x/100)
	
	if pdfList[0].GetBinContent(i) != 0:
		#gluon_pdf.SetBinContent(i,rms-0.9)
		#gluon_pdf2.SetBinContent(i,0.9-rms)
	    #print i, rms, pdfList[0].GetBinContent(i)
	    quark_pdf.SetBinContent(i,rms-0.9)#/pdfList[0].GetBinContent(i))
	    quark_pdf2.SetBinContent(i,0.9-rms)#/pdfList[0].GetBinContent(i))


#xxxxx=input()
############################# above is pdf
if (doreweight):
    for i in range(1,forward_quark.GetNbinsX()+1):
        if (central_quark.GetBinContent(i) > 0 and central_gluon.GetBinContent(i) > 0):
            #print i,forward_quark.GetBinContent(i)/central_quark.GetBinContent(i),forward_gluon.GetBinContent(i)/central_gluon.GetBinContent(i)
    #        factor_gluon = forward_gluon.GetBinContent(i)/central_gluon.GetBinContent(i)
            factor_quark = forward_quark.GetBinContent(i)/central_quark.GetBinContent(i)
            central_quark.SetBinContent(i,central_quark.GetBinContent(i)*factor_quark)
            central_gluon.SetBinContent(i,central_gluon.GetBinContent(i)*factor_quark)
            central_data.SetBinContent(i,central_data.GetBinContent(i)*factor_quark)#*factor_quark)
            if (central_quark_s.GetBinContent(i) > 0 and central_gluon_s.GetBinContent(i) > 0):
				factor_quark_s = forward_quark_s.GetBinContent(i)/central_quark_s.GetBinContent(i)
				central_quark_s.SetBinContent(i,central_quark_s.GetBinContent(i)*factor_quark_s)
				central_gluon_s.SetBinContent(i,central_gluon_s.GetBinContent(i)*factor_quark_s)
				pass
            pass
        pass
    pass

#forward_gluon.Add(forward_quark)
#central_gluon.Add(central_quark)
#forward_gluon.Add(central_gluon)
#central_quark.Add(forward_quark)

central_quark.Scale(1./central_quark.Integral())
central_gluon.Scale(1./central_gluon.Integral())
forward_quark.Scale(1./forward_quark.Integral())
forward_gluon.Scale(1./forward_gluon.Integral())
#central_data.Scale(1./central_data.Integral())
#orward_data.Scale(1./forward_data.Integral())

forward_quark_s.Scale(1./forward_quark_s.Integral())
forward_gluon_s.Scale(1./forward_gluon_s.Integral())
central_quark_s.Scale(1./central_quark_s.Integral())
central_gluon_s.Scale(1./central_gluon_s.Integral())

j1_pt_mc_f_s = ntrackall2.Get("Jet1_or_Jet2_highEta_AllJet_pT_MC")
j1_pt_mc_c_s = ntrackall2.Get("Jet1_or_Jet2_lowEta_AllJet_pT_MC")
j1_pt_frac_f_s = ntrackall2.Get("Jet1_or_Jet2_highEta_GluonJet_percentage_pT")
j1_pt_frac_c_s = ntrackall2.Get("Jet1_or_Jet2_lowEta_GluonJet_percentage_pT")
#Tot_F = 0.
#ToT_Fg = 0.

#Tot_C =0.
#ToT_Cg = 0.

#for i in range (7,8):
#for i in range(1,j1_pt_mc_f.GetNbinsX()+1):
#    Tot_F+=j1_pt_mc_f.GetBinContent(i)
#    ToT_Fg+=j1_pt_mc_f.GetBinContent(i)*j1_pt_frac_f.GetBinContent(i)
#    Tot_C+=j1_pt_mc_c.GetBinContent(i)
#    ToT_Cg+=j1_pt_mc_c.GetBinContent(i)*j1_pt_frac_c.GetBinContent(i)
#    print i,j1_pt_frac_f.GetBinContent(i),j1_pt_frac_c.GetBinContent(i)
#
#    pass
#
#fg = ToT_Fg/Tot_F
#cg = ToT_Cg/Tot_C

fg2 = j1_pt_frac_f_s.GetBinContent(8)
cg2 = j1_pt_frac_c_s.GetBinContent(8)
fq2 = 1.-fg2
cq2 = 1.-cg2

forward = forward_quark.Clone("")
central = forward_quark.Clone("")
forward_s = forward_quark.Clone("")
central_s = forward_quark.Clone("")

for i in range(1,forward.GetNbinsX()+1):
    forward.SetBinContent(i,fg*forward_gluon.GetBinContent(i)+fq*forward_quark.GetBinContent(i))
    central.SetBinContent(i,cg*central_gluon.GetBinContent(i)+cq*central_quark.GetBinContent(i))
    pass

for i in range(1,forward_s.GetNbinsX()+1):
    forward_s.SetBinContent(i,fg2*forward_gluon_s.GetBinContent(i)+fq2*forward_quark_s.GetBinContent(i))
    central_s.SetBinContent(i,cg2*central_gluon_s.GetBinContent(i)+cq2*central_quark_s.GetBinContent(i))
    pass

#Now, let's solve.

quark = forward_quark.Clone("")
gluon = forward_quark.Clone("")
#quark_data = forward_data.Clone("")
#gluon_data = forward_data.Clone("")
quark_data2 = forward_quark.Clone("")
gluon_data2 = forward_gluon.Clone("")
quark_data2_ = forward_quark.Clone("")
gluon_data2_ = forward_gluon.Clone("")

quark_s = forward_quark_s.Clone("")
gluon_s = forward_gluon_s.Clone("")

for i in range(1,forward.GetNbinsX()+1):
    F = forward.GetBinContent(i)
    C = central.GetBinContent(i)
    Q = -(C*fg-F*cg)/(cg*fq-fg*cq)
    G = (C*fq-F*cq)/(cg*fq-fg*cq)
    quark.SetBinContent(i,Q)
    gluon.SetBinContent(i,G)
    #print "   ",i,G,forward_gluon.GetBinContent(i),central_gluon.GetBinContent(i)
    pass

for i in range(1,forward_s.GetNbinsX()+1):
    F = forward_s.GetBinContent(i)
    C = central_s.GetBinContent(i)
    Q = -(C*fg2-F*cg2)/(cg2*fq2-fg2*cq2)
    G = (C*fq2-F*cq2)/(cg2*fq2-fg2*cq2)
    quark_s.SetBinContent(i,Q)
    gluon_s.SetBinContent(i,G)
    #print "   ",i,G,forward_gluon.GetBinContent(i),central_gluon.GetBinContent(i)
    pass

########## for stat. uncertainty
nstraps = 5000 #should be a big number, ideally O(1000) or so.
Qvals = []
Gvals = []

#forward_data2.Scale(1000)
for i in range(1,forward_data.GetNbinsX()+1):
    Qvals+=[np.zeros(nstraps)]
    Gvals+=[np.zeros(nstraps)]
    pass

for k in range(nstraps):
    forward_data_strap = forward_data.Clone("f"+str(k))
    central_data_strap = central_data.Clone("c"+str(k))
    for i in range(1,forward_data.GetNbinsX()+1):
        forward_data_strap.SetBinContent(i,np.random.poisson(forward_data.GetBinContent(i)))
        central_data_strap.SetBinContent(i,np.random.poisson(central_data.GetBinContent(i)))
        #print i, forward_data.GetBinContent(i), central_data.GetBinContent(i)
        pass
    #forward_data_strap.Draw()
    #c.Print("test.pdf")
    #xx = input()
    forward_data_strap.Scale(1./forward_data_strap.Integral())
    central_data_strap.Scale(1./central_data_strap.Integral())
    for i in range(1,forward.GetNbinsX()+1):
        F = forward_data_strap.GetBinContent(i)
        C = central_data_strap.GetBinContent(i)
        Q = -(C*fg-F*cg)/(cg*fq-fg*cq)
        G = (C*fq-F*cq)/(cg*fq-fg*cq)
        Qvals[i-1][k] = Q
        Gvals[i-1][k] = G
        #print i,k," : ", F, C, Q, G,cq, cg, fq, fg
        pass
    pass

for i in range(1,quark_data2.GetNbinsX()+1):
    Qvals[i-1].sort()  #sort k different values in boostrap
    Gvals[i-1].sort()
    Q = np.median(Qvals[i-1])
    G = np.median(Gvals[i-1])
    sigmaQ = 0.5*(Qvals[i-1][int(0.84*len(Qvals[i-1]))] - Qvals[i-1][int(0.16*len(Qvals[i-1]))]) #this is the 68% inter quantile range - equivalent to 2 x the standard deviation for a Gaussian       1 sigma = 1/2 IQR = 0.5( Q84%-Q16% )
    sigmaG = 0.5*(Gvals[i-1][int(0.84*len(Gvals[i-1]))] - Gvals[i-1][int(0.16*len(Gvals[i-1]))])

    aa = TMath.Sqrt(nstraps)
#    bb = TMath.Sqrt(G)
#    cc = TMath.Sqrt(Q)
#    if bb != 0:
#        sigmaG = sigmaG*aa/bb
#    if cc != 0:
#        sigmaQ = sigmaQ*aa/cc
    #    print sigmaG, sigmaQ
    #quark_data2.SetBinContent(i,Q)
    if G != 0:
        sigmaG = sigmaG / G	
    if Q != 0:
        sigmaQ = sigmaQ / Q
#		sigmaG =(G - sigmaG )/G
#        print i,Q,sigmaQ
        
    #x2 = 0.
    #Q0 = quark.GetBinContent(i)
    #G0 = gluon.GetBinContent(i)
    #for k in range(nstraps):
    #    x2 += (Gvals[i-1][k] - G0)**2

    #rms2 = TMath.Sqrt(x2/(nstraps-1))
    #print i,G,sigmaG
    quark_data2.SetBinContent(i,sigmaQ) #this is the stat uncertainty from bootstrapping
    quark_data2_.SetBinContent(i,-sigmaQ) #this is the stat uncertainty from bootstrapping
    #gluon_data2.SetBinContent(i,G)
    #gluon_data2.SetBinContent(i,sigmaG) #this is the stat uncertainty from bootstrapping
    #gluon_data2_.SetBinContent(i,-sigmaG) #this is the stat uncertainty from bootstrapping
    pass


#gluon_data2.Draw()
#c.Print("11.ped")
#v = input()

########## end stat

central_data.Scale(1./central_data.Integral())
forward_data.Scale(1./forward_data.Integral())
quark_data = forward_data.Clone("")
gluon_data = forward_data.Clone("")

for i in range(1,forward_data.GetNbinsX()+1):
    F = forward_data.GetBinContent(i)
    C = central_data.GetBinContent(i)
    Q = -(C*fg-F*cg)/(cg*fq-fg*cq)
    G = (C*fq-F*cq)/(cg*fq-fg*cq)
    quark_data.SetBinContent(i,Q)
    gluon_data.SetBinContent(i,G)
	#print "   ",i,"  ",G,"   ",Q
    pass


gPad.SetLeftMargin(0.15)
gPad.SetTopMargin(0.05)
gPad.SetBottomMargin(0.15)

#quark_ratio = quark_data.Clone("")
#quark_ratio2 = quark.Clone("")
quark_ratio = forward_quark.Clone("")
quark_ratio2 = quark.Clone("")
quark_ratio_s = quark.Clone("")
quark_ratio_s2 = quark_s.Clone("")
quark_ratio_total = quark.Clone("")
quark_ratio_total2 = quark.Clone("")
#quark_ratio_stat = quark_data2.Clone("")
#quark_ratio_stat2 = quark.Clone("")

quark_ratio.Add(quark,-1)
quark_ratio2.Add(forward_quark,-1)
quark_ratio_s.Add(quark_s,-1)
quark_ratio_s2.Add(quark,-1)
#quark_ratio_stat.Divide(quark)
#quark_ratio_stat2.Divide(quark_data2)
#for i in range(quark_ratio.GetNbinsX()):
#    quark_ratio.SetBinContent(i,quark_ratio.GetBinContent(i)-1)
#    quark_ratio2.SetBinContent(i,quark_ratio2.GetBinContent(i)-1)
#    quark_ratio_s.SetBinContent(i,quark_ratio_s.GetBinContent(i)-1)
#    quark_ratio_s2.SetBinContent(i,quark_ratio_s2.GetBinContent(i)-1)

#quark_ratio.Divide(quark)
quark_ratio.Divide(quark)
quark_ratio2.Divide(quark)
quark_ratio_s.Divide(quark)
quark_ratio_s2.Divide(quark)
#quark_ratio2.Divide(forward_quark)
#quark_ratio_s.Divide(quark_s)
#quark_ratio_s2.Divide(quark)

####total
for i in range(quark_ratio.GetNbinsX()):
    total = TMath.Sqrt((quark_ratio.GetBinContent(i))**2+(quark_ratio_s.GetBinContent(i))**2+(quark_data2.GetBinContent(i))**2+(quark_pdf.GetBinContent(i))**2)
    quark_ratio_total.SetBinContent(i,total)
    quark_ratio_total2.SetBinContent(i,-total)

gStyle.SetOptStat(0)

######### for uncertainty plot
quark_ratio.SetTitle("")
quark_ratio.GetXaxis().SetTitle("N_{track}")
quark_ratio.GetYaxis().SetTitle("Uncertainties")
quark_ratio.GetYaxis().SetNdivisions(505)
quark_ratio.GetYaxis().SetRangeUser(-0.4,0.4)
#quark_ratio.GetYaxis().SetRangeUser(0.6,1.4)
quark_ratio.SetLineColor(4)
#quark_ratio.SetMarkerColor(46)
#quark_ratio.SetMarkerSize(12)
quark_ratio2.SetLineColor(4)
quark_ratio_s.SetLineColor(7)
quark_ratio_s2.SetLineColor(7)
#quark_ratio_stat.SetLineColor(8)
#quark_ratio_stat2.SetLineColor(8)
quark_data2.SetLineColor(8)
quark_data2_.SetLineColor(8)
quark_pdf.SetLineColor(2)
quark_pdf2.SetLineColor(2)
#quark_ratio_total.SetLineWidth(1.5)
#quark_ratio_total2.SetLineWidth(1.5)

line = TLine(0,0,60,0)

quark_ratio.Draw("")
quark_ratio2.Draw("same")
quark_ratio_s.Draw("same")
quark_ratio_s2.Draw("same")
#quark_ratio_stat.Draw("same")
#quark_ratio_stat2.Draw("same")
quark_data2.Draw("same")
quark_data2_.Draw("same")
quark_pdf.Draw("same")
quark_pdf2.Draw("same")
line.Draw("same")
quark_ratio_total.Draw("hist same")
quark_ratio_total2.Draw("hist same")


leg = TLegend(0.63,0.6,0.84,0.75) ##0.6,0.5,0.9,0.7
leg.SetTextFont(42)
#leg.SetFillColor(0)
leg.SetBorderSize(0)
#leg.SetFillStyle(0)
leg.SetNColumns(1)
leg.AddEntry(quark_ratio_total,"Total quark","l")
leg.AddEntry(quark_data2,"Stat.","l")
leg.AddEntry(quark_ratio,"MC closure","l")
leg.AddEntry(quark_pdf,"Pdf","l")
leg.AddEntry(quark_ratio_s,"Showering","l")
leg.Draw()

myText(0.18,0.84,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
myText(0.18,0.80,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")
myText(0.18,0.75,"#bf{#scale[1.5]{pT range: 500 - 600 GeV}}")

c.Print("quark_500_"+str(doreweight)+"_uncer.pdf")
aaa=input()

######################## for ratio plot
c.Divide(2,1)

top = c.cd(1)
top.SetPad(0.0,0.0,1.0,1.0) 
top.SetFillColor(0)
top.SetBorderMode(0)
top.SetBorderSize(2)
top.SetTickx(1)
top.SetTicky(1)
top.SetLeftMargin(0.14)
top.SetRightMargin(0.055)
top.SetBottomMargin(0.25)
top.SetFrameBorderMode(0)
#top.SetLogy(1)

bot = c.cd(2)
bot.SetPad(0.0,0.0,1.0,0.3) 
bot.SetFillColor(0)
bot.SetBorderMode(0)
bot.SetBorderSize(2)
bot.SetTickx(1)
bot.SetTicky(1)
bot.SetLeftMargin(0.14)
bot.SetRightMargin(0.055)
bot.SetTopMargin(0.045)
bot.SetBottomMargin(0.4)
bot.SetFrameBorderMode(0)

gluon.SetTitle("")
#gluon.GetXaxis().SetTitle("n_{track}")
gluon.GetYaxis().SetTitle("Normalized to unity")
gluon.GetYaxis().SetNdivisions(505)
gluon.GetYaxis().SetRangeUser(-0.01,gluon_data.GetMaximum()*1.5)
gluon.SetMarkerColor(1)
gluon.SetLineColor(1)
gluon.SetMarkerSize(0.5) 
gluon.SetMarkerStyle(20)

gluon_data.SetMarkerColor(8)
gluon_data.SetLineColor(8)
gluon_data.SetMarkerSize(0.8) 
gluon_data.SetMarkerStyle(29)

forward_gluon.SetMarkerColor(2)
forward_gluon.SetLineColor(2)
forward_gluon.SetMarkerSize(0.5)
forward_gluon.SetMarkerStyle(21)

central_gluon.SetMarkerColor(4)
central_gluon.SetLineColor(4)
central_gluon.SetMarkerSize(0.5)
central_gluon.SetMarkerStyle(22)


gluon_ratio.SetTitle("")
gluon_ratio.GetYaxis().SetRangeUser(0.7,1.3)
gluon_ratio.GetXaxis().SetTitle("N_{track}")
gluon_ratio.GetXaxis().SetTitleOffset(1)
gluon_ratio.GetXaxis().SetTitleSize(0.11)
gluon_ratio.GetXaxis().SetLabelSize(0.1)
gluon_ratio.GetXaxis().SetLabelOffset(0.03)
gluon_ratio.GetYaxis().SetTitle("Data/MC")
gluon_ratio.GetYaxis().SetTitleSize(0.1)
gluon_ratio.GetYaxis().SetTitleOffset(0.5)
#gluon_ratio.GetYaxis().SetLabelSize(0.2)
gluon_ratio.GetYaxis().SetLabelOffset(0.01)


top.cd()
gluon.Draw()
gluon_data.Draw("same")
forward_gluon.Draw("same")
central_gluon.Draw("same")
#forward_data.Draw("same")
#central_data.Draw("same")

leg = TLegend(0.6,0.5,0.9,0.7) ##0.6,0.5,0.9,0.7
leg.SetTextFont(42)
leg.SetFillColor(0)
leg.SetBorderSize(0)
leg.SetFillStyle(0)
leg.SetNColumns(1)
leg.AddEntry(gluon_data,"Extracted gluon (data)","p")
leg.AddEntry(gluon,"Extracted gluon (mc)","p")
leg.AddEntry(forward_gluon,"forward gluon","p")
leg.AddEntry(central_gluon,"central gluon","p")
leg.Draw()

myText(0.18,0.84,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
myText(0.18,0.80,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")
myText(0.18,0.75,"#bf{#scale[1.5]{pT range: 500 - 600 GeV}}")


line = TLine(0,1,60,1)

bot.cd()
gluon_ratio.Draw()
line.Draw("same")
c.Print("gluon_500_"+str(doreweight)+"_pythia.pdf")
aaaa=input()

quark.SetTitle("")
quark.GetXaxis().SetTitle("n_{track}")
quark.GetYaxis().SetTitle("Normalized to unity")
quark.GetYaxis().SetNdivisions(505)
quark.GetYaxis().SetRangeUser(-0.01,quark_data.GetMaximum()*1.5)
quark.SetMarkerColor(1)
quark.SetLineColor(1)
quark.SetMarkerSize(0.5)
quark.SetMarkerStyle(20)

quark_data.SetMarkerColor(8)
quark_data.SetLineColor(8)
quark_data.SetMarkerSize(0.8)
quark_data.SetMarkerStyle(29)

forward_quark.SetLineColor(2)
forward_quark.SetMarkerColor(2)
forward_quark.SetMarkerSize(0.5)
forward_quark.SetMarkerStyle(21)

central_quark.SetLineColor(4)
central_quark.SetMarkerColor(4)
central_quark.SetMarkerSize(0.5)
central_quark.SetMarkerStyle(22)

quark.Draw()
quark_data.Draw("same")
forward_quark.Draw("same")
central_quark.Draw("same")
#forward_data.Draw("same")
#central_data.Draw("same")

leg = TLegend(0.78,0.77,0.98,0.94)
leg.SetTextFont(42)
leg.SetFillColor(0)
#leg.SetBorderSize(0)
#leg.SetFillStyle(0)
leg.SetNColumns(1)
leg.AddEntry(quark_data,"Extracted quark (data)","p")
leg.AddEntry(quark,"Extracted quark (mc)","p")
leg.AddEntry(forward_quark,"Forward quark (mc)","p")
leg.AddEntry(central_quark,"Central quark (mc)","p")
#leg.AddEntry(forward_data,"Forward (data)","p")
#leg.AddEntry(central_data,"Central (data)","p")
leg.Draw()

myText(0.18,0.89,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
myText(0.18,0.84,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")
myText(0.18,0.78,"#bf{#scale[1.5]{pT range: 500 - 600 GeV}}")

c.Print("quark_500_"+str(doreweight)+"_pythia_q.pdf")
x=input()
