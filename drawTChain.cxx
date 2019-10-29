#include "Riostream.h"
#include "TH1.h"
#include "TChain.h"
#include "TString.h"
#include "TLegend.h"
#include "TTree.h"
#include "TPad.h"
#include "TLine.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"
#include <string.h> // used in another function within the same file
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void drawTChain(){

TCanvas* c1 = new TCanvas("c1","c1",600,400);
TFile *f = new TFile("/eos/user/w/wasu/AQT_dijet_data_bdt/dijet_data_bdt.root","recreate");

TChain* fChain = new TChain("AntiKt4EMPFlow_dijet_insitu");

string root_lists = "/eos/user/w/wasu/AQT_dijet_sherpa_bdt/dijet_data_bdt.txt";
string sLine="";
ifstream infile;
infile.open(root_lists.c_str());//Data());
while(!infile.eof())
{
	getline(infile,sLine);
	fChain->Add(sLine.c_str());
}
infile.close();


UInt_t mcChannelNumber; 
Bool_t pass_HLT_j400; 
Bool_t j1_is_truth_jet; 
Bool_t j2_is_truth_jet; 
Float_t j1_pT;
Float_t j2_pT;
Float_t j1_pT_truth;
Float_t j2_pT_truth;
Float_t j1_eta;
Float_t j2_eta;
Float_t j1_eta_truth;
Float_t j2_eta_truth;
Float_t weight;
Float_t weight_ptslice;
Float_t weight_pileup;
Float_t pdfWeights[101];
Float_t j2_bdt_resp;
Float_t j1_bdt_resp;
Int_t j1_NumTrkPt500;
Int_t j2_NumTrkPt500;
Int_t j1_partonLabel;
Int_t j2_partonLabel;




fChain->SetBranchStatus("*",0);
fChain->SetBranchStatus("mcChannelNumber",1);
fChain->SetBranchStatus("pass_HLT_j400",1);
fChain->SetBranchStatus("j1_is_truth_jet",1);
fChain->SetBranchStatus("j2_is_truth_jet",1);
fChain->SetBranchStatus("j1_pT",1);
fChain->SetBranchStatus("j2_pT",1);
fChain->SetBranchStatus("j1_pT_truth",1);
fChain->SetBranchStatus("j2_pT_truth",1);
fChain->SetBranchStatus("j1_eta",1);
fChain->SetBranchStatus("j2_eta",1);
fChain->SetBranchStatus("j1_eta_truth",1);
fChain->SetBranchStatus("j2_eta_truth",1);
fChain->SetBranchStatus("weight_ptslice",1);
fChain->SetBranchStatus("weight",1);
fChain->SetBranchStatus("weight_pileup",1);
fChain->SetBranchStatus("pdfWeights",1);
fChain->SetBranchStatus("j1_NumTrkPt500",1);
fChain->SetBranchStatus("j2_NumTrkPt500",1);
fChain->SetBranchStatus("j1_bdt_resp",1);
fChain->SetBranchStatus("j2_bdt_resp",1);
fChain->SetBranchStatus("j1_partonLabel",1);
fChain->SetBranchStatus("j2_partonLabel",1);


TTree *t2 = fChain->CloneTree(0);
t2->CopyEntries(fChain);


//TTree *t2 = fChain->CopyTree("j1_pT>500 && j1_is_truth_jet &&j2_is_truth_jet && j3_is_truth_jet");
//delete f;
t2->Print();
f->Write();
f->Close();
}
