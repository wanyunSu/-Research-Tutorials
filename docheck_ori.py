from ROOT import *
import numpy as np


doreweight = 0

var = "BDT"
mc = "sherpa_d"

def myText(x,y,text, color = 1):
	l = TLatex()
	l.SetTextSize(0.025)
	l.SetNDC()
	l.SetTextColor(color)
	l.DrawLatex(x,y,text)
	pass

bin = [0, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1200, 1500, 2000]
for i in range(7,13):
#for i in range(13):
		min = bin[i]
		max = bin[i+1]


		#ntrackall = TFile("dijet_pythia_data_j1234_a.root")
		ntrackall = TFile("dijet_"+mc+"_py.root")
		ntrackall2 = TFile("dijet_"+mc+"_j1234.root")

		forward_quark = ntrackall.Get(str(min)+"_LeadingJet_Forward_Quark_bdt")
		forward_quark2 = ntrackall.Get(str(min)+"_SubJet_Forward_Quark_bdt")
		forward_gluon = ntrackall.Get(str(min)+"_LeadingJet_Forward_Gluon_bdt")
		forward_gluon2 = ntrackall.Get(str(min)+"_SubJet_Forward_Gluon_bdt")
		central_quark = ntrackall.Get(str(min)+"_LeadingJet_Central_Quark_bdt")
		central_quark2 = ntrackall.Get(str(min)+"_SubJet_Central_Quark_bdt")
		central_gluon = ntrackall.Get(str(min)+"_LeadingJet_Central_Gluon_bdt")
		central_gluon2 = ntrackall.Get(str(min)+"_SubJet_Central_Gluon_bdt")

#		forward_data = ntrackall.Get("HighEtaJet_"+var+"_"+str(min)+"_to_"+str(max)+"_Data")
#		central_data = ntrackall.Get("LowEtaJet_"+var+"_"+str(min)+"_to_"+str(max)+"_Data")

		forward_quark.Add(forward_quark2)
		forward_gluon.Add(forward_gluon2)
		central_quark.Add(central_quark2)
		central_gluon.Add(central_gluon2)

		j1_pt_mc_f = ntrackall2.Get("Jet1_or_Jet2_highEta_AllJet_pT_MC")
		j1_pt_mc_c = ntrackall2.Get("Jet1_or_Jet2_lowEta_AllJet_pT_MC")
		j1_pt_frac_f = ntrackall2.Get("Jet1_or_Jet2_highEta_GluonJet_percentage_pT")
		j1_pt_frac_c = ntrackall2.Get("Jet1_or_Jet2_lowEta_GluonJet_percentage_pT")

		fg = j1_pt_frac_f.GetBinContent(i+1)
		cg = j1_pt_frac_c.GetBinContent(i+1)
		fq = 1.-fg
		cq = 1.-cg

		c = TCanvas("c","c",500,500)
		if (doreweight):
			for i in range(1,forward_quark.GetNbinsX()+1):
				if (central_quark.GetBinContent(i) > 0 and central_gluon.GetBinContent(i) > 0):
					#print i,forward_quark.GetBinContent(i)/central_quark.GetBinContent(i),forward_gluon.GetBinContent(i)/central_gluon.GetBinContent(i)
					factor_gluon = forward_gluon.GetBinContent(i)/central_gluon.GetBinContent(i)
					factor_quark = forward_quark.GetBinContent(i)/central_quark.GetBinContent(i)
					central_quark.SetBinContent(i,central_quark.GetBinContent(i)*factor_gluon)
					central_gluon.SetBinContent(i,central_gluon.GetBinContent(i)*factor_gluon)
#					central_data.SetBinContent(i,central_data.GetBinContent(i)*factor_gluon)#*factor_quark)
					pass
				pass
			pass
		 

		if (central_quark.Integral() != 0):
			central_quark.Scale(1./central_quark.Integral())
		if(central_gluon.Integral() != 0):
			central_gluon.Scale(1./central_gluon.Integral())
		if(forward_quark.Integral() != 0):
			forward_quark.Scale(1./forward_quark.Integral())
		if(forward_gluon.Integral() != 0):
			forward_gluon.Scale(1./forward_gluon.Integral())
		#central_data.Scale(1./central_data.Integral())
		#forward_data.Scale(1./forward_data.Integral())



		forward = forward_quark.Clone("")
		central = forward_quark.Clone("")

		for i in range(1,forward.GetNbinsX()+1):
			forward.SetBinContent(i,fg*forward_gluon.GetBinContent(i)+fq*forward_quark.GetBinContent(i))
			central.SetBinContent(i,cg*central_gluon.GetBinContent(i)+cq*central_quark.GetBinContent(i))
			pass

		#Now, let's solve.

		quark = forward_quark.Clone("")
		gluon = forward_quark.Clone("")
#		quark_data = forward_data.Clone("")
#		gluon_data = forward_data.Clone("")


		for i in range(1,forward.GetNbinsX()+1):
			F = forward.GetBinContent(i)
			C = central.GetBinContent(i)
			if((cg*fq-fg*cq) != 0 ):
				Q = -(C*fg-F*cg)/(cg*fq-fg*cq)
				G = (C*fq-F*cq)/(cg*fq-fg*cq)
				quark.SetBinContent(i,Q)
				gluon.SetBinContent(i,G)
				#print "   ",i,G,forward_gluon.GetBinContent(i),central_gluon.GetBinContent(i)
			pass



		#central_data.Scale(1./central_data.Integral())
		#forward_data.Scale(1./forward_data.Integral())
		#quark_data = forward_data.Clone("")
		#gluon_data = forward_data.Clone("")

#		for i in range(1,forward_data.GetNbinsX()+1):
#			F = forward_data.GetBinContent(i)
#			C = central_data.GetBinContent(i)
#			if((cg*fq-fg*cq) != 0):
#				Q = -(C*fg-F*cg)/(cg*fq-fg*cq)
#				G = (C*fq-F*cq)/(cg*fq-fg*cq)
#				quark_data.SetBinContent(i,Q)
#				gluon_data.SetBinContent(i,G)
#				#print "   ",i,"  ",G,"   ",Q
#			pass


		gPad.SetLeftMargin(0.15)
		gPad.SetTopMargin(0.05)
		gPad.SetBottomMargin(0.15)

		#quark_ratio = quark_data.Clone("")
		quark_ratio = quark.Clone("") 
		quark_ratio.Divide(forward_quark)
		gluon_ratio = gluon.Clone("") 
		gluon_ratio.Divide(forward_gluon)

		gStyle.SetOptStat(0)
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

		quark.SetTitle("")
		quark.GetXaxis().SetTitle(var)
		quark.GetYaxis().SetTitle("Normalized to unity")
		quark.GetYaxis().SetNdivisions(505)
		quark.GetYaxis().SetRangeUser(-0.01,quark.GetMaximum()*1.5)
		quark.SetMarkerColor(1)
		quark.SetLineColor(1)
		quark.SetMarkerSize(0.5) 
		quark.SetMarkerStyle(20)

#		quark_data.SetMarkerColor(8)
#		quark_data.SetLineColor(8)
#		quark_data.SetMarkerSize(0.8) 
#		quark_data.SetMarkerStyle(29)

		forward_quark.SetMarkerColor(2)
		forward_quark.SetLineColor(2)
		forward_quark.SetMarkerSize(0.5)
		forward_quark.SetMarkerStyle(21)

		central_quark.SetMarkerColor(4)
		central_quark.SetLineColor(4)
		central_quark.SetMarkerSize(0.5)
		central_quark.SetMarkerStyle(22)


		quark_ratio.SetTitle("")
		quark_ratio.GetYaxis().SetRangeUser(0.7,1.3)
		quark_ratio.GetYaxis().SetTitle("MC Closure") #Data/MC
		quark_ratio.GetXaxis().SetTitle(var)
		quark_ratio.GetXaxis().SetTitleOffset(1)
		quark_ratio.GetXaxis().SetTitleSize(0.11)
		quark_ratio.GetXaxis().SetLabelSize(0.1)
		quark_ratio.GetXaxis().SetLabelOffset(0.03)
		quark_ratio.GetYaxis().SetTitleSize(0.1)
		quark_ratio.GetYaxis().SetTitleOffset(0.5)
		#quark_ratio.GetYaxis().SetLabelSize(0.2)
		quark_ratio.GetYaxis().SetLabelOffset(0.01)


		top.cd()
		quark.Draw()
		#quark_data.Draw("same")
		forward_quark.Draw("same")
		central_quark.Draw("same")
		#forward_data.Draw("same")
		#central_data.Draw("same")

		leg = TLegend(0.6,0.5,0.9,0.7) ##0.6,0.5,0.9,0.7
		leg.SetTextFont(42)
		leg.SetFillColor(0)
		leg.SetBorderSize(0)
		leg.SetFillStyle(0)
		leg.SetNColumns(1)
		#leg.AddEntry(quark_data,"Extracted quark (data)","p")
		leg.AddEntry(quark,"Extracted quark (mc)","p")
		leg.AddEntry(forward_quark,"forward quark (mc)","p")
		leg.AddEntry(central_quark,"central quark (mc)","p")
		leg.Draw()

		myText(0.18,0.84,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
		myText(0.18,0.80,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")
		myText(0.18,0.75,"#bf{#scale[1.5]{pT range: "+str(min)+" - "+str(max)+" GeV}}")


		line = TLine(-0.8,1,0.7,1)

		bot.cd()
		quark_ratio.Draw()
		line.Draw("same")
		c.Print("./plots_bdt/quark_"+str(min)+"_"+str(doreweight)+"_"+mc+"_"+var+".pdf")

		




		gluon.SetTitle("")
		gluon.GetXaxis().SetTitle(var)
		gluon.GetYaxis().SetTitle("Normalized to unity")
		gluon.GetYaxis().SetNdivisions(505)
		gluon.GetYaxis().SetRangeUser(-0.01,gluon.GetMaximum()*1.5)
		gluon.SetMarkerColor(1)
		gluon.SetLineColor(1)
		gluon.SetMarkerSize(0.5) 
		gluon.SetMarkerStyle(20)

		#gluon_data.SetMarkerColor(8)
		#gluon_data.SetLineColor(8)
		#gluon_data.SetMarkerSize(0.8) 
		#gluon_data.SetMarkerStyle(29)

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
		gluon_ratio.GetYaxis().SetTitle("MC Closure") #Data/MC
		gluon_ratio.GetXaxis().SetTitle(var)
		gluon_ratio.GetXaxis().SetTitleOffset(1)
		gluon_ratio.GetXaxis().SetTitleSize(0.11)
		gluon_ratio.GetXaxis().SetLabelSize(0.1)
		gluon_ratio.GetXaxis().SetLabelOffset(0.03)
		gluon_ratio.GetYaxis().SetTitleSize(0.1)
		gluon_ratio.GetYaxis().SetTitleOffset(0.5)
		#gluon_ratio.GetYaxis().SetLabelSize(0.2)
		gluon_ratio.GetYaxis().SetLabelOffset(0.01)


		top.cd()
		gluon.Draw()
		#gluon_data.Draw("same")
		forward_gluon.Draw("same")
		central_gluon.Draw("same")
		#forward_data.Draw("same")
		#central_data.Draw("same")

		leg = TLegend(0.2,0.5,0.5,0.7) ##0.6,0.5,0.9,0.7
		leg.SetTextFont(42)
		leg.SetFillColor(0)
		leg.SetBorderSize(0)
		leg.SetFillStyle(0)
		leg.SetNColumns(1)
		#leg.AddEntry(gluon_data,"Extracted gluon (data)","p")
		leg.AddEntry(gluon,"Extracted gluon (mc)","p")
		leg.AddEntry(forward_gluon,"forward gluon (mc)","p")
		leg.AddEntry(central_gluon,"central gluon (mc)","p")
		leg.Draw()

		myText(0.18,0.84,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
		myText(0.18,0.80,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")
		myText(0.18,0.75,"#bf{#scale[1.5]{pT range: "+str(min)+" - "+str(max)+" GeV}}")


		line = TLine(-0.8,1,0.7,1)

		bot.cd()
		gluon_ratio.Draw()
		line.Draw("same")
		c.Print("./plots_bdt/gluon_"+str(min)+"_"+str(doreweight)+"_"+mc+"_"+var+".pdf")
aaaa=input()

