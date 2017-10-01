#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStopwatch.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <TMath.h>
#include <algorithm>


void add_inputs_some_redu_bkg_sum(int const number_of_bin = 1, float lower_cut = -1, float upper_cut = 1, TString cat = "0jet", TString systematic = "nominal"){

  std::cout << "adding inputs to data" << std::endl;
  
  TH1F *histo_output = new TH1F ("MC_REDU_BKG_BDT","MC_REDU_BKG_BDT",number_of_bin, lower_cut, upper_cut);

  TString redu_bbH_1 = "outputs"+cat+"/output_category_"+cat+"_redu_bbH_1_"+systematic+".root";
  TString redu_bbH_2 = "outputs"+cat+"/output_category_"+cat+"_redu_bbH_2_"+systematic+".root";
  TString redu_ttZ_1 = "outputs"+cat+"/output_category_"+cat+"_redu_ttZ_1_"+systematic+".root";
  TString redu_ttZ_2 = "outputs"+cat+"/output_category_"+cat+"_redu_ttZ_2_"+systematic+".root";
  TString redu_WWZ = "outputs"+cat+"/output_category_"+cat+"_redu_WWZ_"+systematic+".root";
  TString redu_WZZ = "outputs"+cat+"/output_category_"+cat+"_redu_WZZ_"+systematic+".root";
  TString redu_ZZZ_1 = "outputs"+cat+"/output_category_"+cat+"_redu_ZZZ_1_"+systematic+".root";
  TString redu_ZZZ_2 = "outputs"+cat+"/output_category_"+cat+"_redu_ZZZ_2_"+systematic+".root";

  
  TFile *f_redu_bbH_1 = new TFile(redu_bbH_1);
  TFile *f_redu_bbH_2 = new TFile(redu_bbH_2);
  TFile *f_redu_ttZ_1 = new TFile(redu_ttZ_1);
  TFile *f_redu_ttZ_2 = new TFile(redu_ttZ_2);
  TFile *f_redu_WWZ = new TFile(redu_WWZ);
  TFile *f_redu_WZZ = new TFile(redu_WZZ);
  TFile *f_redu_ZZZ_1 = new TFile(redu_ZZZ_1);
  TFile *f_redu_ZZZ_2 = new TFile(redu_ZZZ_2);

  TH1F *h_redu_bbH_1 = (TH1F*)f_redu_bbH_1->Get("redu_bbH_1_BDT");
  TH1F *h_redu_bbH_2 = (TH1F*)f_redu_bbH_2->Get("redu_bbH_2_BDT");
  TH1F *h_redu_ttZ_1 = (TH1F*)f_redu_ttZ_1->Get("redu_ttZ_1_BDT");
  TH1F *h_redu_ttZ_2 = (TH1F*)f_redu_ttZ_2->Get("redu_ttZ_2_BDT");
  TH1F *h_redu_WWZ = (TH1F*)f_redu_WWZ->Get("redu_WWZ_BDT");
  TH1F *h_redu_WZZ = (TH1F*)f_redu_WZZ->Get("redu_WZZ_BDT");
  TH1F *h_redu_ZZZ_1 = (TH1F*)f_redu_ZZZ_1->Get("redu_ZZZ_1_BDT");
  TH1F *h_redu_ZZZ_2 = (TH1F*)f_redu_ZZZ_2->Get("redu_ZZZ_2_BDT");

  histo_output->Add(h_redu_bbH_1);
  histo_output->Add(h_redu_bbH_2);
  histo_output->Add(h_redu_ttZ_1);
  histo_output->Add(h_redu_ttZ_2);
  histo_output->Add(h_redu_WWZ);  
  histo_output->Add(h_redu_WZZ);
  histo_output->Add(h_redu_ZZZ_1);
  histo_output->Add(h_redu_ZZZ_2);


  std::cout<<"histo_output->Integral() = "<<histo_output->Integral()<<std::endl;  

  TString s_data = "outputs"+cat+"/output_category_"+cat+"_MC_REDU_BKG_"+systematic+".root";

  TFile *fout = new TFile(s_data, "RECREATE");
  histo_output->Write();


  delete histo_output;
  delete h_redu_bbH_1;
  delete h_redu_bbH_2;
  delete h_redu_ttZ_1;
  delete h_redu_ttZ_2;
  delete h_redu_WWZ;
  delete h_redu_WZZ;
  delete h_redu_ZZZ_1;
  delete h_redu_ZZZ_2;


  fout->Close();
  f_redu_bbH_1->Close();
  f_redu_bbH_2->Close();
  f_redu_ttZ_1->Close();
  f_redu_ttZ_2->Close();
  f_redu_WWZ->Close();
  f_redu_WZZ->Close();
  f_redu_ZZZ_1->Close();
  f_redu_ZZZ_2->Close();
}

    
