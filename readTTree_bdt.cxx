#include "Riostream.h"
#include "TMultiGraph.h"
#include "vector"
#include "TROOT.h"
#include "TStyle.h"
#include "TMarker.h"
#include "TH1.h"
#include "TGraph.h"
#include "TChain.h"
#include "TString.h"
#include "TLegend.h"
#include "TTree.h"
#include "TPad.h"
#include "TLine.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"
using namespace std;
void SetAtlasStyle();
TStyle* AtlasStyle();
void ATLASLabel(Double_t x,Double_t y,const char* text,Color_t color);
void myText(Double_t x,Double_t y,Color_t color, const char *text);
void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,const char *text);
void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle, const char *text,Float_t msize);

void readTTree_bdt(){

SetAtlasStyle();
float   scaleFactor = 1.5;
//float   minX = -4;
//float   maxX = 4;
int     logy = 1;
TString xtitle="pT [GeV] ";
//TString xtitle="Ntracks";
TString ytitle="N Events / bin";
TString outputName="j1_pT_mc16d.png";

TCanvas* c1 = new TCanvas("c1","c1",500,600);
TLegend *leg=new TLegend(0.7,0.4,0.95,0.8);
TFile *f = new TFile("dijet_sherpa_bdt_d.root");
TFile *f_out = new TFile("sherpa_test.root","recreate");
//TFile *f2 = new TFile("dijet_tree_data.root");
//TFile *f = new TFile("dijet_j1_tree.root");
TTree *t = (TTree*)f->Get("AntiKt4EMPFlow_dijet_insitu");
//TTree *t2 = (TTree*)f2->Get("AntiKt4EMTopo_dijet_insitu");

TH1F *h_g1 = new TH1F("j1_mc_g1","",60,-0.9,0.8);
TH1F *h_g2 = new TH1F("j1_mc_g2","",60,-0.9,0.8);
TH1F *h_q1 = new TH1F("j1_mc_q1","",60,-0.9,0.8);
TH1F *h_q2 = new TH1F("j1_mc_q2","",60,-0.9,0.8);
TH1F *h_fq = new TH1F("j1_mc_fq","",60,-0.9,0.8);
TH1F *h_fg = new TH1F("j1_mc_fg","",60,-0.9,0.8);
TH1F *h_cq = new TH1F("j1_mc_cq","",60,-0.9,0.8);
TH1F *h_cg = new TH1F("j1_mc_cg","",60,-0.9,0.8);
//TH1F *hn_qf = new TH1F("forward_quark","",60,0,60);
//TH1F *hn_gf = new TH1F("forward_gluon","",60,0,60);
//TH1F *hn_qc = new TH1F("central_quark","",60,0,60);
//TH1F *hn_gc = new TH1F("central_gluon","",60,0,60);
//TH1F *hn_f = new TH1F("forward_data","",60,0,60);
//TH1F *hn_c = new TH1F("central_data","",60,0,60);
//TH1F *hp_f_1 = new TH1F("ntrack_quark_1","",60,0,60);
//TH1F *hp_c_1 = new TH1F("ntrack_gluon_1","",60,0,60);
//TH1F *hp_f_2 = new TH1F("ntrack_quark_2","",60,0,60);
//TH1F *hp_c_2 = new TH1F("ntrack_gluon_2","",60,0,60);
//TH1F *hp_f_3 = new TH1F("ntrack_quark_3","",60,0,60);
//TH1F *hp_c_4 = new TH1F("ntrack_gluon_3","",60,0,60);
//TH1F *hp_f_g = new TH1F("j1_pt_frac_f","",60,0,60);
//TH1F *hp_c_g = new TH1F("j1_pt_frac_c","",60,0,60);
TH1F *h0 = new TH1F("j0","",100,0,3000);
//TH1F *h1 = new TH1F("j1","",100,0,3000);
//TH1F *h2 = new TH1F("j2","",100,0,3000);
//TH1F *h3 = new TH1F("j3","",100,0,3000);
//TH1F *h4 = new TH1F("j4","",100,0,3000);
//TH1F *h5 = new TH1F("j5","",100,0,3000);
//TH1F *h6 = new TH1F("j6","",100,0,3000);
//TH1F *h7 = new TH1F("j7","",100,0,3000);
//TH1F *h8 = new TH1F("j8","",100,0,3000);
//TH1F *h9 = new TH1F("j9","",100,0,3000);
//TH1F *h10 = new TH1F("j10","",100,0,3000);
//TH1F *h11 = new TH1F("j11","",100,0,3000);
//TH1F *h12 = new TH1F("j12","",100,0,3000);
//TH1F *h13 = new TH1F("j13","",100,0,3000);
//TH1F *h4 = new TH1F("j2_pt_data","",100,500,6000);
//TH1F *h5 = new TH1F("j3_pt_mc","",100,500,3000);
//TH1F *h6 = new TH1F("j3_pt_data","",100,500,3000);
//TH1F *h7 = new TH1F("j1_eta_mc","",100,-5,5);
//TH1F *h8 = new TH1F("j1_eta_data","",100,-5,5);
//TH1F *h9 = new TH1F("j2_eta_mc","",100,-5,5);
//TH1F *h10 = new TH1F("j2_eta_data","",100,-5,5);
//TH1F *h11 = new TH1F("j3_eta_mc","",100,-5,5);
//TH1F *h12 = new TH1F("j3_eta_data","",100,-5,5);
//
//TH1F *h14 = new TH1F("j1_ntrack","",100,0,120);
//TH1F *h15 = new TH1F("j2_ntrack","",100,0,120);
//TH1F *h16 = new TH1F("j3_ntrack","",100,0,120);
//
//TH1F *hf1 = new TH1F("jet1g_fraction","",100,500,6000);
//TH1F *hf2 = new TH1F("jet1q_fraction","",100,500,6000);
//TH1F *hf3 = new TH1F("jet2g_fraction","",100,500,6000);
//TH1F *hf4 = new TH1F("jet2q_fraction","",100,500,6000);
//TH1F *hf5 = new TH1F("jet3g_fraction","",100,500,3000);
//TH1F *hf6 = new TH1F("jet3q_fraction","",100,500,3000);
//
//TH1F *h_1f = new TH1F("j1_eta_f","",100,-3,3);
//TH1F *h_1c = new TH1F("j1_eta_c","",100,-3,3);
//TH1F *h_1f_data = new TH1F("j1_eta_f_data","",100,-3,3);
//TH1F *h_1c_data = new TH1F("j1_eta_c_data","",100,-3,3);

//TH1F *h_ratio = new TH1F("j1_ratio","",60,0,60);
//TH1F *h_ratio_c = new TH1F("j1_pT_ratio_c","",100,500,6000);
//TH1F *h_all = new TH1F("j1_pT_all","",100,-3,3);
//TH1F *h_all_data = new TH1F("j1_pT_all_data","",100,-3,3);
//TH1F *h_ratio = new TH1F("j1_eta_ratio","",100,-5,5);

UInt_t mcChannelNumber; 
Bool_t j1_is_truth_jet; 
Bool_t j2_is_truth_jet; 
Bool_t pass_HLT_j400; 
Float_t j1_pT;
Float_t j1_pT_truth;
Float_t j2_pT_truth;
Float_t j2_pT;
Float_t j1_eta;
Float_t j2_eta;
Float_t j1_eta_truth;
Float_t j2_eta_truth;
Float_t weight;
Float_t weight_ptslice;
Float_t weight_pileup;
Int_t j1_NumTrkPt500;
Int_t j2_NumTrkPt500;
Float_t j1_bdt_resp;
Float_t j2_bdt_resp;
Int_t j1_partonLabel;
Int_t j2_partonLabel;


//TProof::Open("");
//fChain->SetProof();

Long64_t entry = t->GetEntries();
std::cout<<"entry= "<<entry<<std::endl;

t->SetBranchAddress("mcChannelNumber",&mcChannelNumber);
t->SetBranchAddress("pass_HLT_j400",&pass_HLT_j400);
t->SetBranchAddress("j1_is_truth_jet",&j1_is_truth_jet);
t->SetBranchAddress("j2_is_truth_jet",&j2_is_truth_jet);
t->SetBranchAddress("j1_pT",&j1_pT);
t->SetBranchAddress("j2_pT",&j2_pT);
t->SetBranchAddress("j1_eta",&j1_eta);
t->SetBranchAddress("j2_eta",&j2_eta);
t->SetBranchAddress("j1_eta_truth",&j1_eta_truth);
t->SetBranchAddress("j2_eta_truth",&j2_eta_truth);
t->SetBranchAddress("weight_ptslice",&weight_ptslice);
t->SetBranchAddress("weight",&weight);
t->SetBranchAddress("weight_pileup",&weight_pileup);
t->SetBranchAddress("j1_NumTrkPt500",&j1_NumTrkPt500);
t->SetBranchAddress("j2_NumTrkPt500",&j2_NumTrkPt500);
t->SetBranchAddress("j1_bdt_resp",&j1_bdt_resp);
t->SetBranchAddress("j2_bdt_resp",&j2_bdt_resp);
t->SetBranchAddress("j1_partonLabel",&j1_partonLabel);
t->SetBranchAddress("j2_partonLabel",&j2_partonLabel);
//for(Long64_t i=0;i<10000000;i++){
for(Long64_t i=0;i<entry;i++){
	t->GetEntry(i);
//	if(j1_pT>500 && j2_pT>500 && j1_pT<6000 && j2_pT<6000 && abs(j1_eta)<2.1 && abs(j2_eta)<2.1 && j1_NumTrkPt500>0 &&  j2_NumTrkPt500>0 && j3_NumTrkPt500>0){
		if(mcChannelNumber != 0){
			if (weight_pileup == -1)  	weight_pileup = 1;
			if (weight_ptslice == -1) 	weight_ptslice = 1;
			if(j1_is_truth_jet ){//&& pass_HLT_j400){
				h0->Fill(j1_pT,weight*weight_ptslice);
					if(0<j1_partonLabel&& j1_partonLabel<5) {
						h_q2->Fill(j1_bdt_resp,weight*weight_ptslice);
					}
					else if(j1_partonLabel==21 || j1_partonLabel==9){
						h_g2->Fill(j1_bdt_resp,weight*weight_ptslice);
					}
			}				
//				h13->Fill(j1_bdt_resp,weight*weight_ptslice);
//				if(mcChannelNumber == 364700) {h0->Fill(j1_pT,weight*4.91E+03);}
//				else if(mcChannelNumber == 364701){h1->Fill(j1_pT,weight*1.19E+02);}
//				else if(mcChannelNumber == 364702){h2->Fill(j1_pT,weight*4.06E+03);}
//				else if(mcChannelNumber == 364703){h3->Fill(j1_pT,weight*2.94E+03);}
//				else if(mcChannelNumber == 364704){h4->Fill(j1_pT,weight*9.57E+02);}
//				else if(mcChannelNumber == 364705){h5->Fill(j1_pT,weight*4.95E+02);}
//				else if(mcChannelNumber == 364706){h6->Fill(j1_pT,weight*1.10E+02);}
//				else if(mcChannelNumber == 364707){h7->Fill(j1_pT,weight*5.88E+01);}
//				else if(mcChannelNumber == 364708){h8->Fill(j1_pT,weight*7.18E+00);}
//				else if(mcChannelNumber == 364709){h9->Fill(j1_pT,weight*1.38E+00);}
//				else if(mcChannelNumber == 364710){h10->Fill(j1_pT,weight*1.41E-11);}
//				else if(mcChannelNumber == 364711){h11->Fill(j1_pT,weight*2.27E-13);}
//				else if(mcChannelNumber == 364712){h12->Fill(j1_pT,weight*8.91E-16);}

			if(j1_is_truth_jet && j2_is_truth_jet && j2_pT/j1_pT > 0.5){
				if( 500 <= j1_pT && j1_pT<600){
					if(abs(j1_eta)>abs(j2_eta)) {
						if(0<j1_partonLabel&& j1_partonLabel<5) {
							h_fq->Fill(j1_bdt_resp,weight*weight_ptslice);
						}
						if(j1_partonLabel==21 || j1_partonLabel==9){
							h_fg->Fill(j1_bdt_resp,weight*weight_ptslice);
						}
						if(0<j2_partonLabel&& j2_partonLabel<5) {
							h_cq->Fill(j2_bdt_resp,weight*weight_ptslice);
						}
						if(j2_partonLabel==21 || j2_partonLabel==9){
							h_cg->Fill(j2_bdt_resp,weight*weight_ptslice);
						}
					}
					else {
						if(0<j1_partonLabel&& j1_partonLabel<5) {
							h_cq->Fill(j1_bdt_resp,weight*weight_ptslice);
						}
						if(j1_partonLabel==21 || j1_partonLabel==9){
							h_cg->Fill(j1_bdt_resp,weight*weight_ptslice);
						}
						if(0<j2_partonLabel&& j2_partonLabel<5) {
							h_fq->Fill(j2_bdt_resp,weight*weight_ptslice);
						}
						if(j2_partonLabel==21 || j2_partonLabel==9){
							h_fg->Fill(j2_bdt_resp,weight*weight_ptslice);
						}
					}
				}
			}
/*
					if(abs(j1_eta)>abs(j2_eta)) {
						if(j1_partonLabel==21 || j1_partonLabel==9){
							hn_gf->Fill(j1_NumTrkPt500,weight*weight_ptslice);
						}
						else if(0<j1_partonLabel && j1_partonLabel< 9){
							hn_qf->Fill(j1_NumTrkPt500,weight*weight_ptslice);
						}
						if(j2_partonLabel==21 || j2_partonLabel==9){
							hn_gc->Fill(j2_NumTrkPt500,weight*weight_ptslice);
						}
						else if(0<j2_partonLabel && j2_partonLabel<9){
							hn_qc->Fill(j2_NumTrkPt500,weight*weight_ptslice);
						}
					}
					else{
						if(j1_partonLabel==21 || j1_partonLabel==9){
							hn_gc->Fill(j1_NumTrkPt500,weight*weight_ptslice);
						}
						else if(0<j1_partonLabel && j1_partonLabel< 9){
							hn_qc->Fill(j1_NumTrkPt500,weight*weight_ptslice);
						}
						if(j2_partonLabel==21 || j2_partonLabel==9){
							hn_gf->Fill(j2_NumTrkPt500,weight*weight_ptslice);
						}
						else if(0<j2_partonLabel && j2_partonLabel<9){
							hn_qf->Fill(j2_NumTrkPt500,weight*weight_ptslice);
						}
					}
			}
*/
				//	if(j1_partonLabel==21 || j1_partonLabel==9)	hp_f_g->Fill(j1_NumTrkPt500,weight*weight_ptslice);    					if(j2_partonLabel==21 || j2_partonLabel==9)	hp_c_g->Fill(j2_NumTrkPt500,weight*weight_ptslice);
				
//					h_1f->Fill(j1_eta,weight*weight_ptslice);
//					h_1c->Fill(j2_eta,weight*weight_ptslice);
//					hn_1f->fill(j1_numtrkpt500,weight*weight_ptslice);
//					hn_2c_->fill(j2_numtrkpt500,weight*weight_ptslice);
//				}
//				else{
//					hp_f->Fill(j2_NumTrkPt500,weight*weight_ptslice);
//					hp_c->Fill(j1_NumTrkPt500,weight*weight_ptslice);
//					if(j1_partonLabel==21 || j1_partonLabel==9)	hp_c_g->Fill(j1_NumTrkPt500,weight*weight_ptslice);
//					if(j2_partonLabel==21 || j2_partonLabel==9)	hp_f_g->Fill(j2_NumTrkPt500,weight*weight_ptslice);
////					h_1f->Fill(j2_eta,weight*weight_ptslice);
////					h_1c->Fill(j1_eta,weight*weight_ptslice);
//				}
//				h14->Fill(j1_NumTrkPt500,weight*weight_ptslice);
//			}
//			if(j2_is_truth_jet){
//				h2->Fill(j2_pT,weight*weight_ptslice);
//			}
//				h3->Fill(j2_pT,weight*weight_ptslice);
//				h9->Fill(j2_eta,weight*weight_ptslice);
//				if(j2_partonLabel==21 || j2_partonLabel==9) hf3->Fill(j2_pT,weight*weight_ptslice);
//				if(0<j2_partonLabel&& j2_partonLabel<9) hf4->Fill(j2_pT,weight*weight_ptslice);
//				h15->Fill(j2_NumTrkPt500,weight*weight_ptslice);
//			}
//			if(j3_is_truth_jet){
//				h5->Fill(j3_pT,weight*weight_ptslice);
 //				h11->Fill(j3_eta,weight*weight_ptslice);
//				if(j3_partonLabel==21 || j3_partonLabel==9) hf5->Fill(j3_pT,weight*weight_ptslice);
//				if(0<j3_partonLabel&& j3_partonLabel<9) hf6->Fill(j3_pT,weight*weight_ptslice);
//				h16->Fill(j3_NumTrkPt500,weight*weight_ptslice);
//			}
//		}
//		else{
//			h2->Fill(j1_eta);
//			h4->Fill(j2_pT);
//			h6->Fill(j3_pT);
//			h8->Fill(j1_eta);
//			h10->Fill(j2_eta);
//			h12->Fill(j3_eta);
//			if(j2_pT/j1_pT>0.5){
//				if(500<= j1_pT && j1_pT<750){
//					if(abs(j1_eta)>abs(j2_eta)) {
//						hn_f->Fill(j1_NumTrkPt500);
//						hn_c->Fill(j2_NumTrkPt500);
//					}
//					else {
//						hn_c->Fill(j1_NumTrkPt500);
//						hn_f->Fill(j2_NumTrkPt500);
//					}
//				}
//			}
//		}
	}
}
//	hn_qf->SetLineColor(9);
//	hn_qc->SetLineColor(9);
    //    hn_gc->SetLineColor(2);
  //      hn_gf->SetLineColor(2);
	//hp_c->SetLineColor(2);
        //hp_f->SetLineColor(9);
        h_q1->SetLineColor(2);
        h_q2->SetLineColor(2);
        h_g1->SetLineColor(9);
        h_g2->SetLineColor(9);
        h_fq->SetLineColor(2);
        h_fg->SetLineColor(9);
        h_cg->SetMarkerColor(9);
        h_cq->SetMarkerColor(2);
		h_cq->SetMarkerSize(0.5);
		h_cg->SetMarkerSize(0.5);
//        h5->SetLineColor(2);
//        h6->SetLineColor(9);
        //h0->SetLineColor(2);
        //h1->SetLineColor(3);
        //h2->SetLineColor(4);
        //h3->SetLineColor(5);
        //h4->SetLineColor(6);
        //h5->SetLineColor(7);
        //h6->SetLineColor(8);
        //h7->SetLineColor(9);
        //h8->SetLineColor(46);
        //h9->SetLineColor(30);
        //h10->SetLineColor(40);
        //h11->SetLineColor(38);
        //h12->SetLineColor(41);
        //h9->SetLineColor(2);
        //h10->SetLineColor(9);
        //h11->SetLineColor(2);
        //h12->SetLineColor(9);

        //h14->SetLineColor(2);
        //h15->SetLineColor(9);
        //h16->SetLineColor(8);

	//hf3->SetLineColor(2);
	//hf4->SetLineColor(9);
	//hf5->SetLineColor(2);
	//hf6->SetLineColor(9);


	h_q1->Scale(1/h_q1->Integral());
	h_q2->Scale(1/h_q2->Integral());
	h_g1->Scale(1/h_g1->Integral());
	h_g2->Scale(1/h_g2->Integral());
//	hn_qf->Scale(1/hn_qf->Integral());
//	hn_qc->Scale(1/hn_qc->Integral());
//	hn_gf->Scale(1/hn_gf->Integral());
//	hn_gc->Scale(1/hn_gc->Integral());
//	hn_f->Scale(1/hn_f->Integral());
//	hn_c->Scale(1/hn_c->Integral());
//	hp_c->Scale(1/hp_c->Integral());
//	hp_f->Scale(1/hp_f->Integral());
//	h1->Scale(h2->Integral()/h1->Integral());
//	h3->Scale(h4->Integral()/h3->Integral());
//	h5->Scale(h6->Integral()/h5->Integral());
//	h7->Scale(h8->Integral()/h7->Integral());  //normalize mc to data
//	h9->Scale(h10->Integral()/h9->Integral());  //normalize mc to data
//	h11->Scale(h12->Integral()/h11->Integral());  //normalize mc to data
//	hp_f->Scale(hp_f_g->Integral()/hp_f->Integral());
//	hp_c->Scale(hp_c_g->Integral()/hp_c->Integral());

//	cout <<"integral mc and date: "<< h1->Integral()<<" "<<h2->Integral() << endl;

	// get the maximum bin content dynamically for the upper panel
//	float max_mc = h1->GetBinContent(h1->GetMaximumBin());
//	float max_da = 1;
//	//float max_da = h2->GetBinContent(h2->GetMaximumBin());
////	float max_mc = hf3->GetBinContent(hf3->GetMaximumBin());
////	float max_da = hf4->GetBinContent(hf4->GetMaximumBin());
//
////	cout << "maximums : " << max_mc << "   " << max_da << endl;
//
//	float maxVal = 1.0;
//
//	if(max_mc>max_da)
//		maxVal = max_mc*scaleFactor;
//	else if(max_da>max_mc)
//		maxVal = max_da*scaleFactor;
//	else
//		cout << "You shouldn't see this, but check your code if you do" << endl;
	
//	h_all->Add(hf1,hf2);
//	h_ratio->Divide(h_1f_data,h_1f);
//	h_ratio->Divide(h2,h1);
//	h_ratio_c->Divide(h_1c_data,h_1c);
//	h_all->Add(h_1f,h_1c);
//	h_all_data->Add(h_1f_data,h_1c_data);
//	h_1c->Divide(h_all);
//	h_1f->Divide(h_all);
//	h_1c_data->Divide(h_all_data);
//	h_1f_data->Divide(h_all_data);

//	hp_f_g->Scale(1/hp_f->Integral());
//	hp_c_g->Scale(1/hp_c->Integral());
//	hp_f_g->Divide(hp_f);
//	hp_c_g->Divide(hp_c);

//	h_1f_data->Scale(1/h_1f_data->Integral());
//	h_1c_data->Scale(1/h_1c_data->Integral());
//	hp_f_g->Sumw2();
//	hp_c_g->Sumw2();
//	h_ratio->Divide(h8,h7);
//	h_ratio->Divide(hp_f_g,hp_c_g);
//	h_ratio->Sumw2();
//	h_ratio_c->Divide(hp_c_g,hp_c);
//	h_ratio_c->Sumw2();
//	h_1f->Sumw2();
//	h_1c->Sumw2();
//	h_1f_data->Sumw2();
//	h_1c_data->Sumw2();
//	hp_f_g->SetLineColor(2);
//	hp_c_g->SetLineColor(9);
//	hp_f_g->SetLineWidth(2);
//	hp_c_g->SetLineWidth(2);
//	h_ratio->SetMarkerSize(0.4);
//	h_ratio->SetMarkerStyle(22);
//	h_ratio_c->SetMarkerStyle(23);
//	h_ratio_c->SetMarkerSize(0.8);
//	h_ratio_c->SetMarkerColor(2);

    h_fg->GetXaxis()->SetTitle("BDT score");
    h_fg->GetYaxis()->SetTitle("Normalized Entries");
	//h_g1->GetYaxis()->SetRangeUser(2,10e-12);
	//h0->GetYaxis()->SetRangeUser(10e-19,10e5);
	//h1->SetMaximum(maxVal);
//	gPad->SetLogy(logy);
//	h0->Draw("hist");
	h_fg->Draw("hist");
	h_cg->Draw("hist p same");
	h_cq->Draw("hist p same");
	h_fq->Draw("hist same");
//	h2->Draw("hist same");
//	h3->Draw("hist same");
//	h4->Draw("hist same");
//	h5->Draw("hist same");
//	h6->Draw("hist same");
//	h7->Draw("hist same");
//	h8->Draw("hist same");
//	h9->Draw("hist same");
//	h10->Draw("hist same");
//	h11->Draw("hist same");
//	h12->Draw("hist same");
//	h1->SetFillColor(5);
	//hp_f_g->GetYaxis()->SetTitle("Gluon Fraction");
//	h_q1->GetYaxis()->SetTitle("Normalized to unity");
//	h_q1->GetXaxis()->SetTitle("Ntracks");
//	h_q1->GetYaxis()->SetTitleOffset(1.3);
//	h_q1->GetYaxis()->SetLabelSize(0.04);
//	h_q1->GetXaxis()->SetTitleSize(0.05);
//	h_q1->GetXaxis()->SetTitleOffset(1.2);
//	h_q1->SetMarkerSize(0.4);
//	h_g1->SetMarkerSize(0.4);
//	//hn_qf->SetMarkerStyle(22);
//	h_q1->SetMarkerColor(9);
//	//hp_c->SetMarkerStyle(23);
//	//h2->SetMarkerSize(1);//0.4
//	h_g1->SetMarkerColor(2);
//	h1->SetLineWidth(2);
//	h2->SetLineWidth(2);

//	h_1f->GetYaxis()->SetRangeUser(0,0.05);

//	hf3->SetMaximum(maxVal);
//	hf5->GetYaxis()->SetRangeUser(10e-5,10e4);
//	h_q1->GetYaxis()->SetRangeUser(0,0.08);//0.08
//	hn_g->GetXaxis()->SetRangeUser(0,65);
//	h1->GetYaxis()->SetTitle("N Events / Bin");
//	h1->GetYaxis()->SetTitleOffset(1.2);

	//h_ratio->SetMinimum(1.0);
	//h_ratio->SetMaximum(0.5);
//	h_ratio->GetYaxis()->SetRangeUser(0.7,1.3);
//	h_ratio->GetXaxis()->SetTitle(xtitle);
//	h_ratio->GetXaxis()->SetTitleSize(0.1);
//	h_ratio->GetXaxis()->SetLabelSize(0.12);
//	h_ratio->GetXaxis()->SetLabelOffset(0.03);
//	h_ratio->GetYaxis()->SetTitle("Data/MC");
//	//h_ratio->GetYaxis()->SetTitle("Forward/Central");
//	h_ratio->GetYaxis()->SetTitleSize(0.1);
//	h_ratio->GetYaxis()->SetTitleOffset(0.5);
//	h_ratio->GetYaxis()->SetLabelSize(0.08);
//	h_ratio->GetYaxis()->SetLabelOffset(0.01);

	//TLine *line = new TLine(-5,1,5,1);
//	TLine *line = new TLine(-3,1,3,1);

  //   c1->Divide(2,1);

     //TPad *top = (TPad*)c1->cd();//cd(1)
     //top->SetPad(0.0,0.0,1.0,1.0); //0,0.3,1,1
     //top->SetFillColor(0);
     //top->SetBorderMode(0);
     //top->SetBorderSize(2);
     //top->SetTickx(1);
     //top->SetTicky(1);
     //top->SetLeftMargin(0.14);
     //top->SetRightMargin(0.055);
     //top->SetBottomMargin(0.14);
     //top->SetFrameBorderMode(0);
     //top->SetLogy(logy);

//     TPad *bot = (TPad*)c1->cd(2);
//     bot->SetPad(0.0,0.0,1.0,0.3); //0,0,1,0.3
//     bot->SetFillColor(0);
//     bot->SetBorderMode(0);
//     bot->SetBorderSize(2);
//     bot->SetTickx(1);
//     bot->SetTicky(1);
//     bot->SetLeftMargin(0.14);
//     bot->SetRightMargin(0.055);
//     bot->SetTopMargin(0.045);
//     bot->SetBottomMargin(0.4);
//     bot->SetFrameBorderMode(0);

//	top->cd();
	//h_q1->Draw("hist c");
	//h_g1->Draw("hist c same");
//	hn_qf->Draw("hist p");
//	hn_qc->Draw("hist c same");
//	hn_gf->Draw("hist p same");
//	hn_gc->Draw("hist c same");
//	hn_f->Draw("hist p same");
//	hn_c->Draw("hist c same");

//float inte_q1[60],inte_g1[60],inte_q2[60],inte_g2[60],inte_q3[60],inte_g3[60];
//TMultiGraph* gr = new TMultiGraph();
//
//	for(int j=0; j<60; j++){
//		inte_q1[j] = h_q1->Integral(0,j)/h_q1->Integral();
//		inte_g1[j] =1. -  h_g1->Integral(0,j)/h_g1->Integral();
//
//		inte_q2[j] = h_q2->Integral(0,j)/h_q2->Integral();
//		inte_g2[j] =1. -  h_g2->Integral(0,j)/h_g2->Integral();
//
//		inte_q3[j] = h_q3->Integral(0,j)/h_q3->Integral();
//		inte_g3[j] =1. -  h_g3->Integral(0,j)/h_g3->Integral();
//	}
//
//
//	TGraph* g_1 = new TGraph(60,inte_q1, inte_g1);
//	TGraph* g_2 = new TGraph(60,inte_q2, inte_g2);
//	TGraph* g_3 = new TGraph(60,inte_q3, inte_g3);
//	g_1->SetLineColor(9);
//	g_2->SetLineColor(2);
//	g_3->SetLineColor(8);
//	gr->Add(g_1);
//	gr->Add(g_2);
//	gr->Add(g_3);
//	gr->Draw("AC");
//	gr->GetXaxis()->SetTitle("Quark Efficiency");
//	gr->GetYaxis()->SetTitle("Gluon Efficiency");
//	gr->GetXaxis()->SetRangeUser(0,1);
//	gr->GetYaxis()->SetRangeUser(0,1);

//	bot->cd();
//	h_ratio->Draw("pe");
//	h_ratio_c->Draw("psame");
//	line->Draw("same");

//	top->cd();
//	h1->Draw("hist");
//	h2->Draw("pesame");
	//ATLASLabel(0.15,0.85,"  Work in Progress", 1);
	//ATLASLabel(0.5,0.8,"  Internal", 1);
//	myText(0.5, 0.7, 1, "500<pT<750 GeV");

	leg->SetBorderSize(0.0);
	leg->SetFillStyle(0);
///	leg->SetTextSize(0.015);
//	leg->AddEntry(g_1,"pT:500 - 750 GeV","l");
//	leg->AddEntry(g_2,"pT:750 - 1000 GeV","l");
//	leg->AddEntry(g_3,"pT:1000 - 1250 GeV","l");
	//leg->AddEntry(h_q1,"quark","l");
	//leg->AddEntry(h_g1,"gluon","l");
	leg->AddEntry(h_fq,"Forward Quark","p");
	leg->AddEntry(h_cq,"Central Quark","l");
	leg->AddEntry(h_fg,"Forward Gluon","p");
	leg->AddEntry(h_cg,"Central Gluon","l");
//	leg->AddEntry(hp_f_g,"More Forward (Data)");
//	leg->AddEntry(hp_c_g,"More Central(Data)");
//	leg->AddEntry(h0,"JZ0","l");
//	leg->AddEntry(h1,"JZ1","l");
//	leg->AddEntry(h2,"JZ2","l");
//	leg->AddEntry(h3,"JZ3","l");
//	leg->AddEntry(h4,"JZ4","l");
//	leg->AddEntry(h5,"JZ5","l");
//	leg->AddEntry(h6,"JZ6","l");
//	leg->AddEntry(h7,"JZ7","l");
//	leg->AddEntry(h8,"JZ8","l");
//	leg->AddEntry(h9,"JZ9","l");
//	leg->AddEntry(h10,"JZ10","l");
//	leg->AddEntry(h11,"JZ11","l");
//	leg->AddEntry(h12,"JZ12","l");
	leg->Draw("same");

	f_out->Write();
	c1->SaveAs(outputName);
}



void SetAtlasStyle(){
	static TStyle* atlasStyle = 0;
	std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;
	if ( atlasStyle==0 ) atlasStyle = AtlasStyle();
	gROOT->SetStyle("ATLAS");
	gROOT->ForceStyle();
}

TStyle* AtlasStyle() {
	  TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");
	  Int_t icol=0; // WHITE
	  atlasStyle->SetFrameBorderMode(icol);
	  atlasStyle->SetFrameFillColor(icol);
	  atlasStyle->SetCanvasBorderMode(icol);
	  atlasStyle->SetCanvasColor(icol);
	  atlasStyle->SetPadBorderMode(icol);
	  atlasStyle->SetPadColor(icol);
	  atlasStyle->SetStatColor(icol);

	  atlasStyle->SetPaperSize(20,26);

	  atlasStyle->SetPadTopMargin(0.05);
	  atlasStyle->SetPadRightMargin(0.05);
	  atlasStyle->SetPadBottomMargin(0.16);
	  atlasStyle->SetPadLeftMargin(0.16);

	  atlasStyle->SetTitleXOffset(1.4);
	  atlasStyle->SetTitleYOffset(1.4);

	  Int_t font=42; // Helvetica
	  Double_t tsize=0.05;
	  atlasStyle->SetTextFont(font);

	  atlasStyle->SetTextSize(tsize);
	  atlasStyle->SetLabelFont(font,"x");
	  atlasStyle->SetTitleFont(font,"x");
	  atlasStyle->SetLabelFont(font,"y");
	  atlasStyle->SetTitleFont(font,"y");
	  atlasStyle->SetLabelFont(font,"z");
	  atlasStyle->SetTitleFont(font,"z");

	  atlasStyle->SetLabelSize(tsize,"x");
	  atlasStyle->SetTitleSize(tsize,"x");
	  atlasStyle->SetLabelSize(tsize,"y");
	  atlasStyle->SetTitleSize(tsize,"y");
	  atlasStyle->SetLabelSize(tsize,"z");
	  atlasStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  atlasStyle->SetMarkerStyle(20);
  atlasStyle->SetMarkerSize(1.2);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars (as recommended in ATLAS figure guidelines)
  atlasStyle->SetErrorX(0.0001);
  // get rid of error bar caps
  atlasStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(0);
  //atlasStyle->SetOptStat(1111);
  atlasStyle->SetOptStat(0);
  //atlasStyle->SetOptFit(1111);
  atlasStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

  return atlasStyle;

}

void ATLASLabel(Double_t x,Double_t y,const char* text,Color_t color){
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);

  double delx = 0.08*696*gPad->GetWh()/(472*gPad->GetWw());

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p;
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+delx,y,text);
    //    p.DrawLatex(x,y,"#sqrt{s}=900GeV");
  }
}

void myText(Double_t x,Double_t y,Color_t color, const char *text) {

  Double_t tsize=0.04;
  TLatex l; //l.SetTextAlign(12);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}

void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,const char *text)
{

  Double_t tsize=0.06;

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize);
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  printf("x1= %f x2= %f y1= %f y2= %f \n",x1,x2,y1,y2);

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(1001);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(4);
  mline.SetLineColor(1);
  mline.SetLineStyle(1);
  Double_t y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y_new,x2,y_new);

}


void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle, const char *text,Float_t msize)
{
  Double_t tsize=0.06;
  TMarker *marker = new TMarker(x-(0.4*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(msize);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize);
  l.SetNDC();
  l.DrawLatex(x,y,text);
}
