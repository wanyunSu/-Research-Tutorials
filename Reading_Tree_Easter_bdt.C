#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TClass.h"
#include "Riostream.h"
#include "stdio.h"
#include "TPad.h"
#include "TLine.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"

int Find_bin_position(double jet_pT, int b, double* bins);

void Reading_Tree_Easter_bdt(std::string input1 = "/eos/user/w/wasu/AQT_dijet_sherpa_bdt/dijet_sherpa_bdt_d.root", std::string PDFtype = "sherpa_d")
{  
   //define file
   TFile* fname1 = new TFile( input1.c_str());
   TTree* theTree1 = (TTree*)fname1->Get("AntiKt4EMPFlow_dijet_insitu");
   Int_t event1 = theTree1->GetEntries();
   std::cout << "The number of event is " << event1 << ".\n";
  
   //define variables in input
   UInt_t mcChannelNumber;
   Bool_t j1_is_truth_jet, j2_is_truth_jet, j3_is_truth_jet, j4_is_truth_jet;
   Float_t j1_pT, j2_pT, j1_pT_truth, j2_pT_truth;
   Float_t j1_eta, j2_eta, j1_eta_truth, j2_eta_truth;
   Float_t j1_bdt_resp, j2_bdt_resp;
   Int_t j1_NumTrkPt500, j2_NumTrkPt500;
   Int_t j1_partonLabel, j2_partonLabel;
   Float_t weight, weight_ptslice, weight_pileup, pdfWeights[101];
  
   //define variables for filling histograms
   float jet_pT[4],jet_pT_truth[2],jet_eta_truth[2], jet_eta[4], jet_BDT[4];
   int jet_Ntrk[4], jet_partonLabel[4], highEtaJet[2];
   int DataType_Number, JetType_Number[4];
   int pT_bin[2];
   float total_weight;
   float j3_pT = 1 , j4_pT = 1, j3_eta = -1,j4_eta = -2 , j3_bdt_resp = -99 , j4_bdt_resp = -99;
   int  j3_NumTrkPt500 = -1, j4_NumTrkPt500 = -1, j3_partonLabel = -1, j4_partonLabel = -1;;
   std::string HistName;

   std::vector<std::string> JetNumber;
   JetNumber.reserve(4);
   JetNumber.push_back("Jet1");
   JetNumber.push_back("Jet2");
   JetNumber.push_back("Jet3");
   JetNumber.push_back("Jet4");
  
   std::vector<std::string> DataType;
   DataType.reserve(2);
   DataType.push_back("MC");
   DataType.push_back("Data");
   
   std::vector<std::string> JetType;
   JetType.reserve(4);
   JetType.push_back("All");
   JetType.push_back("Quark");
   JetType.push_back("Gluon");
   JetType.push_back("Others");
  

   //define histograms
  
   double bins[14] = {0, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1200, 1500, 2000};
   int BINS_Name[14] = {0, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1200, 1500, 2000};
   int  nbin = sizeof(bins)/sizeof(Double_t) - 1;
  
  
   TH1F** Hist_pT =  new TH1F *[4]; //4 jets
   TH1F** Hist_eta =  new TH1F *[4]; 
   TH1F** Hist_Ntrk =  new TH1F *[4];
   TH1F** Hist_BDT =  new TH1F *[4];
   TH1F* Hist_highEtaJet_percentage_pT =  new TH1F [4]; //2 sets of jets, (j1, j2) and (j3, j4). 2 kind of considerations: quark, and gluon.
   TH1F* Hist_lowEtaJet_percentage_pT =  new TH1F [4];
   for( int i=0; i<4; i++ )
   {
      Hist_pT[i] = new TH1F[2]; //2 set of input, 0 for MC, 1 for data
      Hist_eta[i] = new TH1F[2]; 
      Hist_Ntrk[i] = new TH1F[2];
      Hist_BDT[i] = new TH1F[2];
   }
   for( int i=0; i<4; i++)
   {
      for( int j=0; j<2; j++)
      {
         HistName = JetNumber[i] + "_pT_" + DataType[j];
         Hist_pT[i][j].SetName(HistName.c_str());
         Hist_pT[i][j].SetTitle(HistName.c_str());
         Hist_pT[i][j].SetBins( nbin, bins );

         HistName = JetNumber[i] + "_eta_" + DataType[j];
         Hist_eta[i][j].SetName(HistName.c_str());
         Hist_eta[i][j].SetTitle(HistName.c_str());
         Hist_eta[i][j].SetBins(50, -2.5, 2.5);


         HistName = JetNumber[i] + "_Ntrk_" + DataType[j];
         Hist_Ntrk[i][j].SetName(HistName.c_str());
         Hist_Ntrk[i][j].SetTitle(HistName.c_str());
         Hist_Ntrk[i][j].SetBins(60, 0., 60.);


         HistName = JetNumber[i] + "_BDT_" + DataType[j];
         Hist_BDT[i][j].SetName(HistName.c_str());
         Hist_BDT[i][j].SetTitle(HistName.c_str());
         Hist_BDT[i][j].SetBins(60, -0.8, 0.7);
      }
   }

  
   TH1F** Hist_highEtaJet_pT =  new TH1F *[8]; //2 sets of jets, (j1, j2) and (j3, j4). 4 kind of considerations: Total, quark, gluon, and others.
   TH1F** Hist_lowEtaJet_pT =  new TH1F *[8];
   for( int i=0; i<8; i++ )
   {
      Hist_highEtaJet_pT[i] = new TH1F[2]; //2 set of input, 0 for MC, 1 for data
      Hist_lowEtaJet_pT[i] = new TH1F[2]; 
   }
   for( int i=0; i<8; i++)
   {
      for( int j=0; j<2; j++)
      {
         int type = i/2;// The result can be only 0, 1, 2, or 3. They are the 4 kind of considerations.
         int k = i%2;// The result can be only 0 or 1. The are the 2 sets of jets, (j1, j2) or (j3, j4).

         HistName = JetNumber[2*k] + "_or_" + JetNumber[2*k+1] + "_highEta_" + JetType[type] + "Jet_pT_" + DataType[j];
         //Similar logic as Hist_highEtaJet_percentage_pT. But this time we have total and others, besides quark and gluon.
         Hist_highEtaJet_pT[i][j].SetName(HistName.c_str());
         Hist_highEtaJet_pT[i][j].SetTitle(HistName.c_str());
         Hist_highEtaJet_pT[i][j].SetBins( nbin, bins );

         HistName = JetNumber[2*k] + "_or_" + JetNumber[2*k+1] + "_lowEta_" + JetType[type] + "Jet_pT_" + DataType[j];
         Hist_lowEtaJet_pT[i][j].SetName(HistName.c_str());
         Hist_lowEtaJet_pT[i][j].SetTitle(HistName.c_str());
         Hist_lowEtaJet_pT[i][j].SetBins( nbin, bins );
      }
   }


   TH1F* Hist_leading_jet_pT =  new TH1F [3]; //2 sets of jets, (j1, j2) and (j3, j4). 4 kind of considerations: Total, quark, gluon, and others.
   for( int i=0; i<3; i++)
   {
      HistName = "Jet1_pT_" + JetType[i+1];
      Hist_leading_jet_pT[i].SetName(HistName.c_str());
      Hist_leading_jet_pT[i].SetTitle(HistName.c_str());
      Hist_leading_jet_pT[i].SetBins( nbin, bins );
   }

	//  ws: 100 different pdf weights for pythia
   TH1F** Hist_highEta_QGJet_Ntrk_pdf =  new TH1F *[101]; 
   TH1F** Hist_lowEta_QGJet_Ntrk_pdf =  new TH1F *[101];
   TH1F** Hist_highEta_QGJet_BDT_pdf =  new TH1F *[101]; 
   TH1F** Hist_lowEta_QGJet_BDT_pdf =  new TH1F *[101];
   for( int i=0; i<101; i++ )
   {
      Hist_highEta_QGJet_Ntrk_pdf[i] = new TH1F[2]; //2 set of input, 0 for Quark, 1 for Gluon
      Hist_lowEta_QGJet_Ntrk_pdf[i] = new TH1F[2]; 
      Hist_highEta_QGJet_BDT_pdf[i] = new TH1F[2]; //2 set of input, 0 for Quark, 1 for Gluon
      Hist_lowEta_QGJet_BDT_pdf[i] = new TH1F[2]; 
   }
   for( int i=0;i<101;i++)
   {
	   for( int j=0;j<2;j++)
	   {
         HistName = "HighEta_" + JetType[j+1] + "Jet_Ntrk_500_to_600_pdf"+std::to_string(i);
         Hist_highEta_QGJet_Ntrk_pdf[i][j].SetName(HistName.c_str());
         Hist_highEta_QGJet_Ntrk_pdf[i][j].SetTitle(HistName.c_str());
         Hist_highEta_QGJet_Ntrk_pdf[i][j].SetBins(60, 0., 60.);

         HistName = "LowEta_" + JetType[j+1] + "Jet_Ntrk_500_to_600_pdf"+std::to_string(i);
         Hist_lowEta_QGJet_Ntrk_pdf[i][j].SetName(HistName.c_str());
         Hist_lowEta_QGJet_Ntrk_pdf[i][j].SetTitle(HistName.c_str());
	    Hist_lowEta_QGJet_Ntrk_pdf[i][j].SetBins(60, 0., 60.);


         HistName = "HighEta_" + JetType[j+1] + "Jet_BDT_500_to_600_pdf"+std::to_string(i);
         Hist_highEta_QGJet_BDT_pdf[i][j].SetName(HistName.c_str());
         Hist_highEta_QGJet_BDT_pdf[i][j].SetTitle(HistName.c_str());
         Hist_highEta_QGJet_BDT_pdf[i][j].SetBins(60, -0.8, 0.7);

         HistName = "LowEta_" + JetType[j+1] + "Jet_BDT_500_to_600_pdf"+std::to_string(i);
         Hist_lowEta_QGJet_BDT_pdf[i][j].SetName(HistName.c_str());
         Hist_lowEta_QGJet_BDT_pdf[i][j].SetTitle(HistName.c_str());
	    Hist_lowEta_QGJet_BDT_pdf[i][j].SetBins(60, -0.8, 0.7);
	   }
   }


   TH1F** Hist_highEta_QGJet_Ntrk =  new TH1F *[13]; //Number of Track for Quark and Gluon with 13 pT ranges
   TH1F** Hist_lowEta_QGJet_Ntrk =  new TH1F *[13];
   for( int i=0; i<13; i++ )
   {
      Hist_highEta_QGJet_Ntrk[i] = new TH1F[2]; //2 set of input, 0 for Quark, 1 for Gluon
      Hist_lowEta_QGJet_Ntrk[i] = new TH1F[2]; 
   }
   for( int i=0; i<13; i++)
   {
      for( int j=0; j<2; j++)
      {
         HistName = "HighEta_" + JetType[j+1] + "Jet_Ntrk_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]);
         //Similar logic as Hist_highEtaJet_percentage_pT. But this time we have total and others, besides quark and gluon.
         Hist_highEta_QGJet_Ntrk[i][j].SetName(HistName.c_str());
         Hist_highEta_QGJet_Ntrk[i][j].SetTitle(HistName.c_str());
         Hist_highEta_QGJet_Ntrk[i][j].SetBins(60, 0., 60.);

         HistName = "LowEta_" + JetType[j+1] + "Jet_Ntrk_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]);
         Hist_lowEta_QGJet_Ntrk[i][j].SetName(HistName.c_str());
         Hist_lowEta_QGJet_Ntrk[i][j].SetTitle(HistName.c_str());
         Hist_lowEta_QGJet_Ntrk[i][j].SetBins(60, 0., 60.);
      }
   }


   TH1F** Hist_highEtaJet_Ntrk =  new TH1F *[13]; //Number of Track for MC and data with 13 pT ranges
   TH1F** Hist_lowEtaJet_Ntrk =  new TH1F *[13];
   for( int i=0; i<13; i++ )
   {
      Hist_highEtaJet_Ntrk[i] = new TH1F[2]; //2 set of input, 0 for MC, 1 for Data
      Hist_lowEtaJet_Ntrk[i] = new TH1F[2]; 
   }
   for( int i=0; i<13; i++)
   {
      for( int j=0; j<2; j++)
      {
         HistName = "HighEtaJet_Ntrk_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]) + "_"  + DataType[j];
         //Similar logic as Hist_highEtaJet_percentage_pT. But this time we have total and others, besides quark and gluon.
         Hist_highEtaJet_Ntrk[i][j].SetName(HistName.c_str());
         Hist_highEtaJet_Ntrk[i][j].SetTitle(HistName.c_str());
         Hist_highEtaJet_Ntrk[i][j].SetBins(60, 0., 60.);

         HistName = "LowEtaJet_Ntrk_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]) + "_"  + DataType[j];
         Hist_lowEtaJet_Ntrk[i][j].SetName(HistName.c_str());
         Hist_lowEtaJet_Ntrk[i][j].SetTitle(HistName.c_str());
         Hist_lowEtaJet_Ntrk[i][j].SetBins(60, 0., 60.);
      }
   }
   // for BDT
   TH1F** Hist_highEta_QGJet_BDT =  new TH1F *[13]; //Number of Track for Quark and Gluon with 13 pT ranges
   TH1F** Hist_lowEta_QGJet_BDT =  new TH1F *[13];
   for( int i=0; i<13; i++ )
   {
      Hist_highEta_QGJet_BDT[i] = new TH1F[2]; //2 set of input, 0 for Quark, 1 for Gluon
      Hist_lowEta_QGJet_BDT[i] = new TH1F[2]; 
   }
   for( int i=0; i<13; i++)
   {
      for( int j=0; j<2; j++)
      {
         HistName = "HighEta_" + JetType[j+1] + "Jet_BDT_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]);
         //Similar logic as Hist_highEtaJet_percentage_pT. But this time we have total and others, besides quark and gluon.
         Hist_highEta_QGJet_BDT[i][j].SetName(HistName.c_str());
         Hist_highEta_QGJet_BDT[i][j].SetTitle(HistName.c_str());
         Hist_highEta_QGJet_BDT[i][j].SetBins(60, -0.8,0.7);

         HistName = "LowEta_" + JetType[j+1] + "Jet_BDT_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]);
         Hist_lowEta_QGJet_BDT[i][j].SetName(HistName.c_str());
         Hist_lowEta_QGJet_BDT[i][j].SetTitle(HistName.c_str());
         Hist_lowEta_QGJet_BDT[i][j].SetBins(60, -0.8,0.7);
      }
   }


   TH1F** Hist_highEtaJet_BDT =  new TH1F *[13]; //Number of Track for MC and data with 13 pT ranges
   TH1F** Hist_lowEtaJet_BDT =  new TH1F *[13];
   for( int i=0; i<13; i++ )
   {
      Hist_highEtaJet_BDT[i] = new TH1F[2]; //2 set of input, 0 for MC, 1 for Data
      Hist_lowEtaJet_BDT[i] = new TH1F[2]; 
   }
   for( int i=0; i<13; i++)
   {
      for( int j=0; j<2; j++)
      {
         HistName = "HighEtaJet_BDT_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]) + "_"  + DataType[j];
         //Similar logic as Hist_highEtaJet_percentage_pT. But this time we have total and others, besides quark and gluon.
         Hist_highEtaJet_BDT[i][j].SetName(HistName.c_str());
         Hist_highEtaJet_BDT[i][j].SetTitle(HistName.c_str());
         Hist_highEtaJet_BDT[i][j].SetBins(60, -0.8,0.7);

         HistName = "LowEtaJet_BDT_" + std::to_string(BINS_Name[i]) + "_to_" + std::to_string(BINS_Name[i+1]) + "_"  + DataType[j];
         Hist_lowEtaJet_BDT[i][j].SetName(HistName.c_str());
         Hist_lowEtaJet_BDT[i][j].SetTitle(HistName.c_str());
         Hist_lowEtaJet_BDT[i][j].SetBins(60, -0.8,0.7);
      }
   }
   /*
   For the above loop, i = 0, 2, 4, 6 are the histograms for Total, quark, gluon, and other jets respectively, within the set (j1, j2)
   i = 1, 3, 5, 7 are for the histograms for Total, quark, gluon, and other jets respectively, within the set (j3, j4)
   */

   std::cout << "Checking Point." << "\n";
  
   //define Branch Address
   theTree1->SetBranchAddress("mcChannelNumber",&mcChannelNumber);
   theTree1->SetBranchAddress("j1_is_truth_jet",&j1_is_truth_jet);
   theTree1->SetBranchAddress("j2_is_truth_jet",&j2_is_truth_jet);
   theTree1->SetBranchAddress("j1_pT",&j1_pT);
   theTree1->SetBranchAddress("j2_pT",&j2_pT);
   theTree1->SetBranchAddress("j1_pT_truth",&j1_pT_truth);
   theTree1->SetBranchAddress("j2_pT_truth",&j2_pT_truth);
   theTree1->SetBranchAddress("j1_eta",&j1_eta);
   theTree1->SetBranchAddress("j2_eta",&j2_eta);
   theTree1->SetBranchAddress("j1_eta_truth",&j1_eta_truth);
   theTree1->SetBranchAddress("j2_eta_truth",&j2_eta_truth);
   theTree1->SetBranchAddress("j1_bdt_resp",&j1_bdt_resp);
   theTree1->SetBranchAddress("j2_bdt_resp",&j2_bdt_resp);
   theTree1->SetBranchAddress("j1_NumTrkPt500",&j1_NumTrkPt500);
   theTree1->SetBranchAddress("j2_NumTrkPt500",&j2_NumTrkPt500);
   theTree1->SetBranchAddress("j1_partonLabel",&j1_partonLabel);
   theTree1->SetBranchAddress("j2_partonLabel",&j2_partonLabel);
   theTree1->SetBranchAddress("weight_ptslice",&weight_ptslice);
   theTree1->SetBranchAddress("weight",&weight);
   theTree1->SetBranchAddress("weight_pileup",&weight_pileup);
   theTree1->SetBranchAddress("pdfWeights",&pdfWeights);
  
  
   //start filling histogram
   for (Int_t i=0; i<event1; i++)
   //for (Int_t i=0; i<10000; i++)
   //for (Int_t i=108100000; i<event1; i++)
   //for (Int_t i=0; i<27600000; i++)
   {
       theTree1->GetEntry(i);

       //Preparation 
       jet_pT[0] = j1_pT;
       jet_pT[1] = j2_pT;
       jet_pT[2] = j3_pT;
       jet_pT[3] = j4_pT;
       jet_pT_truth[0] = j1_pT_truth;
       jet_pT_truth[1] = j2_pT_truth;
        
       jet_eta[0] = j1_eta;
       jet_eta[1] = j2_eta;
       jet_eta[2] = j3_eta;
       jet_eta[3] = j4_eta;
       jet_eta_truth[0] = j1_eta_truth;
       jet_eta_truth[1] = j2_eta_truth;
 
       jet_Ntrk[0] = j1_NumTrkPt500;
       jet_Ntrk[1] = j2_NumTrkPt500;
       jet_Ntrk[2] = j3_NumTrkPt500;
       jet_Ntrk[3] = j4_NumTrkPt500;
       jet_BDT[0] = j1_bdt_resp;
       jet_BDT[1] = j2_bdt_resp;
       jet_BDT[2] = j3_bdt_resp;
       jet_BDT[3] = j4_bdt_resp;
  
       jet_partonLabel[0] = j1_partonLabel;
       jet_partonLabel[1] = j2_partonLabel;
       jet_partonLabel[2] = j3_partonLabel;
       jet_partonLabel[3] = j4_partonLabel;
  
       if(mcChannelNumber != 0)
       {
			if (weight_pileup == -1)  	weight_pileup = 1;
			if (weight_ptslice == -1) 	weight_ptslice = 1;
			// for mc16d
//			if(mcChannelNumber == 364700) {weight_ptslice=3810.915499;}
//
//			else if(mcChannelNumber == 364702){weight_ptslice=8514.01402;}
//			else if(mcChannelNumber == 364703){weight_ptslice=2558.978958;}
//			else if(mcChannelNumber == 364704){weight_ptslice=797.6770695;}
//			else if(mcChannelNumber == 364705){weight_ptslice=478.5718455;}
//			else if(mcChannelNumber == 364706){weight_ptslice=111.9971453;}
//			else if(mcChannelNumber == 364707){weight_ptslice=47.06716309;}
//			else if(mcChannelNumber == 364708){weight_ptslice=1.02E+02;}
//			else if(mcChannelNumber == 364709){weight_ptslice=1.101859868;}
//			else if(mcChannelNumber == 364710){weight_ptslice=6.27E-01;}
//
//			else if(mcChannelNumber == 364712){weight_ptslice=5.46E-05;}
			// for mc16e
//			if(mcChannelNumber == 364700) {weight_ptslice=4.65E+03;}
//			else if(mcChannelNumber == 364701){weight_ptslice=7.51E+01;}
//			else if(mcChannelNumber == 364702){weight_ptslice=2.76E+03;}
//			else if(mcChannelNumber == 364703){weight_ptslice=1.63E+03;}
//			else if(mcChannelNumber == 364704){weight_ptslice=5.96E+02;}
//			else if(mcChannelNumber == 364705){weight_ptslice=2.83E+02;}
//			else if(mcChannelNumber == 364706){weight_ptslice=6.27E+01;}
//			else if(mcChannelNumber == 364707){weight_ptslice=3.36E+01;}
//			else if(mcChannelNumber == 364708){weight_ptslice=4.10E+00;}
//			else if(mcChannelNumber == 364709){weight_ptslice=8.61E-01;}
//			else if(mcChannelNumber == 364710){weight_ptslice=2.20E-01;}
//
//			else if(mcChannelNumber == 364712){weight_ptslice=4.51E-05;}

			// for mc16a
			if(mcChannelNumber == 364700) {weight_ptslice=4.91E+03;}
			else if(mcChannelNumber == 364701){weight_ptslice=1.19E+02;}
			else if(mcChannelNumber == 364702){weight_ptslice=4.06E+03;}
			else if(mcChannelNumber == 364703){weight_ptslice=2.94E+03;}
			else if(mcChannelNumber == 364704){weight_ptslice=9.57E+02;}
			else if(mcChannelNumber == 364705){weight_ptslice=4.95E+02;}
			else if(mcChannelNumber == 364706){weight_ptslice=1.10E+02;}
			else if(mcChannelNumber == 364707){weight_ptslice=5.88E+01;}
			else if(mcChannelNumber == 364708){weight_ptslice=7.18E+00;}
			else if(mcChannelNumber == 364709){weight_ptslice=1.38E+00;}
			else if(mcChannelNumber == 364710){weight_ptslice=3.52E-01;}
			else if(mcChannelNumber == 364711){weight_ptslice=1.07E-02;}
			else if(mcChannelNumber == 364712){weight_ptslice=7.22E-05;}
          DataType_Number = 0; //MC
          total_weight = weight*weight_ptslice*pdfWeights[0];
          //total_weight = 1.0;
       }
       else if(mcChannelNumber == 0)
       {
          DataType_Number = 1; //Data
          total_weight = 1.0;
          //std::cout << "It's data." << "\n";
       }
       
       for(int j=0; j<2; j++) //This loop compare the abs(eta) of j1 and j2, then j3 and j4. 
       {
          if( fabs(jet_eta[2*j]) > fabs(jet_eta[2*j+1]) )
          {
             highEtaJet[j] = 0; //Jet1 or Jet3 is high eta jet
          }
          else
          {
             highEtaJet[j] = 1; //Jet2 or Jet4 is high eta jet
          }
       }

       for(int j=0; j<4; j++)
       {
          if(jet_partonLabel[j] == 21)
          {
             JetType_Number[j] = 2; //Gluon
          }
          else if(jet_partonLabel[j]>0 && jet_partonLabel[j]<5)
          {
             JetType_Number[j] = 1; //Light quark u,d,s,c
          }
          else
          {
             JetType_Number[j] = 3; //Others, mainly pileup 
          }
       }

       //Start Filling the diagram
       if( (j1_pT > 500.) && (fabs(j1_eta)<2.1) && (fabs(j2_eta)<2.1) && ((j1_pT/j2_pT)<1.5) && (j1_pT < 2000.) )
       {
          for( int k=0; k<4; k++)//Filling the 1st 2 jets' kinematic variales
          {
             //if( jet_pT[k] > 0.) //mainly for jet3 and jet4
                Hist_pT[k][DataType_Number].Fill(jet_pT[k], total_weight); 
                Hist_eta[k][DataType_Number].Fill(jet_eta[k], total_weight); 
                if( jet_Ntrk[k] > 0.)
                {
                   Hist_Ntrk[k][DataType_Number].Fill(jet_Ntrk[k], total_weight);
                } 
                if( jet_BDT[k] > -99.)
                {
                   Hist_BDT[k][DataType_Number].Fill(jet_BDT[k], total_weight);
                } 
          }

          int b = 0;
          pT_bin[0] = Find_bin_position(j1_pT, b, bins);
          int c = 0;
          pT_bin[1] = Find_bin_position(j2_pT, c, bins);

          if( DataType_Number == 0)//MC only
          {
             Hist_leading_jet_pT[JetType_Number[0]-1].Fill(j1_pT, total_weight);

             if( (jet_Ntrk[ highEtaJet[0] ] > 0) && (JetType_Number[ highEtaJet[0] ] != 3) )
             {  // highEtaJet[0] for j1 j2 compare
                Hist_highEta_QGJet_Ntrk[pT_bin[highEtaJet[0]]][JetType_Number[highEtaJet[0]]-1].Fill(jet_Ntrk[highEtaJet[0]], total_weight);
				for( int k=0;k<101;k++){
					if(( highEtaJet[0] <1 )&& j1_pT<600){
					Hist_highEta_QGJet_Ntrk_pdf[k][JetType_Number[highEtaJet[0]]-1].Fill(jet_Ntrk[highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
					else if ((highEtaJet[0] >0) && j2_pT<600){
					Hist_highEta_QGJet_Ntrk_pdf[k][JetType_Number[highEtaJet[0]]-1].Fill(jet_Ntrk[highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
				}
             }
             if( (jet_Ntrk[ 1-highEtaJet[0] ] > 0) && (JetType_Number[ 1-highEtaJet[0] ] != 3) )
             {
                Hist_lowEta_QGJet_Ntrk[pT_bin[1-highEtaJet[0]]][JetType_Number[1-highEtaJet[0]] - 1].Fill(jet_Ntrk[1-highEtaJet[0]], total_weight);
				for( int k=0;k<101;k++){
					if(( highEtaJet[0] <1 )&& j2_pT<600){
					Hist_lowEta_QGJet_Ntrk_pdf[k][JetType_Number[1-highEtaJet[0]]-1].Fill(jet_Ntrk[1-highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
					else if ((highEtaJet[0] >0) && j1_pT<600){
					Hist_lowEta_QGJet_Ntrk_pdf[k][JetType_Number[1-highEtaJet[0]]-1].Fill(jet_Ntrk[1-highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
				}
          	}

             if( (jet_BDT[ highEtaJet[0] ] > -99) && (JetType_Number[ highEtaJet[0] ] != 3) )
             {  // highEtaJet[0] for j1 j2 compare
                Hist_highEta_QGJet_BDT[pT_bin[highEtaJet[0]]][JetType_Number[highEtaJet[0]]-1].Fill(jet_BDT[highEtaJet[0]], total_weight);
				for( int k=0;k<101;k++){
					if(( highEtaJet[0] <1 )&& j1_pT<600){
					Hist_highEta_QGJet_BDT_pdf[k][JetType_Number[highEtaJet[0]]-1].Fill(jet_BDT[highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
					else if ((highEtaJet[0] >0) && j2_pT<600){
					Hist_highEta_QGJet_BDT_pdf[k][JetType_Number[highEtaJet[0]]-1].Fill(jet_BDT[highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
				}
             }
             if( (jet_BDT[ 1-highEtaJet[0] ] > -99) && (JetType_Number[ 1-highEtaJet[0] ] != 3) )
             {
                Hist_lowEta_QGJet_BDT[pT_bin[1-highEtaJet[0]]][JetType_Number[1-highEtaJet[0]] - 1].Fill(jet_BDT[1-highEtaJet[0]], total_weight);
				for( int k=0;k<101;k++){
					if(( highEtaJet[0] <1 )&& j2_pT<600){
					Hist_lowEta_QGJet_BDT_pdf[k][JetType_Number[1-highEtaJet[0]]-1].Fill(jet_BDT[1-highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
					else if ((highEtaJet[0] >0) && j1_pT<600){
					Hist_lowEta_QGJet_BDT_pdf[k][JetType_Number[1-highEtaJet[0]]-1].Fill(jet_BDT[1-highEtaJet[0]], total_weight*pdfWeights[k]);
				 	}	
				}
          	}
		  }
		  

          if(jet_Ntrk[ highEtaJet[0] ] > 0)
          {
             Hist_highEtaJet_Ntrk[pT_bin[highEtaJet[0]]][DataType_Number].Fill(jet_Ntrk[highEtaJet[0]], total_weight);
          }
          if(jet_Ntrk[ 1-highEtaJet[0] ] > 0)
          {
             Hist_lowEtaJet_Ntrk[pT_bin[1-highEtaJet[0]]][DataType_Number].Fill(jet_Ntrk[1-highEtaJet[0]], total_weight);
          }
          if(jet_BDT[ highEtaJet[0] ] > -99)
          {
             Hist_highEtaJet_BDT[pT_bin[highEtaJet[0]]][DataType_Number].Fill(jet_BDT[highEtaJet[0]], total_weight);
          }
          if(jet_BDT[ 1-highEtaJet[0] ] > -99)
          {
             Hist_lowEtaJet_BDT[pT_bin[1-highEtaJet[0]]][DataType_Number].Fill(jet_BDT[1-highEtaJet[0]], total_weight);
          }

          for ( int k=0; k<2; k++)//First loop filling the set (j1, j2), then second loop filling the set (j3, j4)
          {
             if( (jet_pT[2*k] > 0.) && (jet_pT[2*k+1] > 0.) ) //mainly for jet3 and jet4, i.e. loop k=1
             {
                //All type of jets are required to fill in
                Hist_highEtaJet_pT[k][DataType_Number].Fill(jet_pT[2*k + highEtaJet[k]], total_weight);// (2*k + highEtaJet[k]) help us to find out the correct jet to fill
                Hist_lowEtaJet_pT[k][DataType_Number].Fill(jet_pT[2*k + 1 - highEtaJet[k]], total_weight);

                //Different type of jets are filled into different type of histogram
                Hist_highEtaJet_pT[2*JetType_Number[2*k + highEtaJet[k]] + k][DataType_Number].Fill(jet_pT[2*k + highEtaJet[k]], total_weight);
                Hist_lowEtaJet_pT[2*JetType_Number[2*k + 1 - highEtaJet[k]] + k][DataType_Number].Fill(jet_pT[2*k + 1 - highEtaJet[k]], total_weight);
             }
          }
       }  
   }
   /*
   for (Int_t i=27700000; i<28600000; i++)

   for (Int_t i=28700000; i<32500000; i++)

   for (Int_t i=32600000; i<108000000; i++)

   for (Int_t i=108100000; i<event1; i++)
   */

 
   for( int i=0; i<2; i++)//1st loop for (j1, j2), 2nd loop for (j3, j4)
   {
      Hist_highEtaJet_percentage_pT[i+2] = (Hist_highEtaJet_pT[i+4][0])/ (Hist_highEtaJet_pT[i][0]);
      Hist_lowEtaJet_percentage_pT[i+2] = (Hist_lowEtaJet_pT[i+4][0])/ (Hist_lowEtaJet_pT[i][0]);
      Hist_highEtaJet_percentage_pT[i] = ((Hist_highEtaJet_pT[i][0])-(Hist_highEtaJet_pT[i+4][0]))/ (Hist_highEtaJet_pT[i][0]);
      Hist_lowEtaJet_percentage_pT[i] = ((Hist_lowEtaJet_pT[i][0])-(Hist_lowEtaJet_pT[i+4][0]))/ (Hist_lowEtaJet_pT[i][0]);
   }
   
   
   for( int i=0; i<4; i++)//1st loop for (j1, j2), 2nd loop for (j3, j4)
   {
      int type = i/2;// The result can be only 0, or 1. They are the 2 kind of considerations, Quark and Gluon, i.e. (JetType[1] and JetType[2])
      int k = i%2;// The result can be only 0 or 1. The are the 2 sets of jets, (j1, j2) and (j3, j4).

      HistName = JetNumber[2*k] + "_or_" + JetNumber[2*k+1] + "_highEta_" + JetType[type+1] + "Jet_percentage_pT";
      //First 2 loops are quark, the later 2 loops are gluon.
      //First and third loop are for (j1, j2). Second and forth loop are for (j3, j4)
      Hist_highEtaJet_percentage_pT[i].SetName(HistName.c_str());
      Hist_highEtaJet_percentage_pT[i].SetTitle(HistName.c_str());

      HistName = JetNumber[2*k] + "_or_" + JetNumber[2*k+1] + "_lowEta_" + JetType[type+1] + "Jet_percentage_pT";
      Hist_lowEtaJet_percentage_pT[i].SetName(HistName.c_str());
      Hist_lowEtaJet_percentage_pT[i].SetTitle(HistName.c_str());
   }
 
   std::string filename;
   filename = "dijet_" + PDFtype + "_j1234.root";
  
   TFile* output = new TFile( filename.c_str(), "RECREATE" );

   for( int i=0; i<4; i++)
   {
      for( int j=0; j<2; j++)
      {
         Hist_pT[i][j].Write(); 
         Hist_eta[i][j].Write(); 
         Hist_Ntrk[i][j].Write(); 
         Hist_BDT[i][j].Write(); 
      }
      Hist_highEtaJet_percentage_pT[i].Write(); 
      Hist_lowEtaJet_percentage_pT[i].Write(); 
   }

   for( int i=0; i<8; i++)
   {
      for( int j=0; j<2; j++)
      {
         Hist_highEtaJet_pT[i][j].Write(); 
         Hist_lowEtaJet_pT[i][j].Write(); 
      }
   }

   for( int i=0; i<3; i++)
   {
      Hist_leading_jet_pT[i].Write(); 
   }

   for( int i=0; i<13; i++)
   {
      for( int j=0; j<2; j++)
      {
         Hist_highEta_QGJet_Ntrk[i][j].Write();
         Hist_lowEta_QGJet_Ntrk[i][j].Write();
         Hist_highEta_QGJet_BDT[i][j].Write();
         Hist_lowEta_QGJet_BDT[i][j].Write();
      }
   }

   for( int i=0; i<13; i++)
   {
      for( int j=0; j<2; j++)
      {
         Hist_highEtaJet_Ntrk[i][j].Write();
         Hist_lowEtaJet_Ntrk[i][j].Write();
         Hist_highEtaJet_BDT[i][j].Write();
         Hist_lowEtaJet_BDT[i][j].Write();
      }
   }
   
   for( int i=0; i<101; i++)
   {
      for( int j=0; j<2; j++)
      {
         Hist_highEta_QGJet_Ntrk_pdf[i][j].Write();
         Hist_lowEta_QGJet_Ntrk_pdf[i][j].Write();
         Hist_highEta_QGJet_BDT_pdf[i][j].Write();
         Hist_lowEta_QGJet_BDT_pdf[i][j].Write();
      }
   }


   output->Close();
   
   std::cout << "Histogram root-file is created. \n";
   fname1->Close();
}


int Find_bin_position(double jet_pT, int b, double* bins)
{
   int position;
   if ( jet_pT > bins[b])
   {
   	b++;
   	position = Find_bin_position( jet_pT, b, bins );
   	
   }
   else
   {
   position = b-1;
   
   }
   
   return position;
}

