#include <TH1.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include "AtlasUtils.h"
#include "AtlasStyle.h"
#include "AtlasLabels.h"
#include "AtlasLabels.C"
#include "AtlasUtils.C"
#include "AtlasStyle.C"
#include "TROOT.h"
#include "TRandom.h"
#include "TGraphErrors.h"


TGraph* GetROCCurve(TH1F* h1, TH1F* h2)
{
    double False_Positive_Total = h1->GetSumOfWeights();
    double True_Positive_Total = h2->GetSumOfWeights();
    
    double False_Positive[100];
    double True_Positive[100];

    for(int i = 0; i<100; i++)
    {
       False_Positive[i] = h1->Integral(0,i)/False_Positive_Total;
       //std::cout << "False_Positive for the " << i << "-th bin is " << False_Positive[i] << ".\n";

       True_Positive[i] = 1. - h2->Integral(0,i)/True_Positive_Total;
       //std::cout << "True_Positive for the " << i << "-th bin is " << True_Positive[i] << ".\n";
    } 

    TGraph* gr = new TGraph(100, False_Positive, True_Positive);

    return gr;
}

void ROC_plot_ATLAS_style(std::string file_ggF_1 = "./../BDT/Applied_BDT/ggF_sm_paper.root", std::string file_VBF_1 = "./../BDT/Applied_BDT/VBF_sm_paper.root", std::string file_ggF_2 = "./../BDT/Applied_BDT/ggF_sm_RB.root", std::string file_VBF_2 = "./../BDT/Applied_BDT/VBF_sm_RB.root", std::string Versus = "paper_RB1", std::string type1 = "paper", std::string type2 = "RB")
{  
    
    #ifdef __CINT__
    gROOT->LoadMacro("AtlasLabels.C");
    gROOT->LoadMacro("AtlasUtils.C");
    #endif

    SetAtlasStyle();

    
    //Set Histograms
    TFile* f1 = new TFile( file_ggF_1.c_str(), "UPDATE");
    TFile* f2 = new TFile( file_VBF_1.c_str(), "UPDATE");
    TH1F* h1 = (TH1F*)f1->Get("MVA_BDTG_signal");
    TH1F* h2 = (TH1F*)f2->Get("MVA_BDTG_signal");

    TFile* f3 = new TFile( file_ggF_2.c_str(), "UPDATE");
    TFile* f4 = new TFile( file_VBF_2.c_str(), "UPDATE");
    TH1F* h3 = (TH1F*)f3->Get("MVA_BDTG_signal");
    TH1F* h4 = (TH1F*)f4->Get("MVA_BDTG_signal");

    //Calculating False Positive and True Positive
    TGraph* gr1 = GetROCCurve(h1, h2);
    TGraph* gr2 = GetROCCurve(h3, h4);

     
    
    //Set Canvas
    TCanvas *c1 = new TCanvas("atlas_rectangular","Canvas title",0.,0.,800,600);
 
    //Set Axis Title
    std::string titleX;
    std::string titleY;
    titleX = "#font[52]{Signal Efficiency}";
    titleY = "#font[42]{Background Rejection}";
    gr1->GetXaxis()->SetTitle(titleX.c_str());
    gr1->GetYaxis()->SetTitle(titleY.c_str());
    
    
    //ploting
    gr1->SetLineColor(kBlue+1);
    gr1->SetLineWidth(2);
    gr2->SetLineColor(kRed+1);
    gr2->SetLineWidth(2);
 
    TLegend* leg1 = new TLegend(0.74,0.78,0.92,0.92);
    leg1->SetFillColor(0);
    leg1->SetTextSize(0.03);
    leg1->AddEntry(gr1,type1.c_str(),"L");
    leg1->AddEntry(gr2,type2.c_str(),"L");

    gr1->Draw("AC");
    gr2->Draw("C,same");

    leg1->Draw("same");

    std::string filename;
    filename = "BDT/ROC/" + Versus + ".png";

    c1->Print(filename.c_str());
    f1->Close();
    f2->Close();
    f3->Close();
    f4->Close();


}  
