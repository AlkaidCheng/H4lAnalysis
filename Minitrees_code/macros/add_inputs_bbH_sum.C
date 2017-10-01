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


void add_inputs_bbH_sum(int const number_of_bin = 1, float lower_cut = -1, float upper_cut = 1, TString cat = "0jet", TString systematic = "nominal"){

  std::cout << "adding inputs to data" << std::endl;
  
  TH1F *histo_output = new TH1F ("bbH_SUM_BDT","bbH_SUM_BDT",number_of_bin, lower_cut, upper_cut);

  TString redu_bbH_1 = "outputs"+cat+"/output_category_"+cat+"_redu_bbH_1_"+systematic+".root";
  TString redu_bbH_2 = "outputs"+cat+"/output_category_"+cat+"_redu_bbH_2_"+systematic+".root";
  
  TFile *f_redu_bbH_1 = new TFile(redu_bbH_1);
  TFile *f_redu_bbH_2 = new TFile(redu_bbH_2);

  TH1F *h_redu_bbH_1 = (TH1F*)f_redu_bbH_1->Get("redu_bbH_1_BDT");
  TH1F *h_redu_bbH_2 = (TH1F*)f_redu_bbH_2->Get("redu_bbH_2_BDT");

  histo_output->Add(h_redu_bbH_1);
  histo_output->Add(h_redu_bbH_2);

  std::cout<<"histo_output->Integral() = "<<histo_output->Integral()<<std::endl;  

  TString s_data = "outputs"+cat+"/output_category_"+cat+"_bbH_SUM_"+systematic+".root";

  TFile *fout = new TFile(s_data, "RECREATE");
  histo_output->Write();


  delete histo_output;
  delete h_redu_bbH_1;
  delete h_redu_bbH_2;


  fout->Close();
  f_redu_bbH_1->Close();
  f_redu_bbH_2->Close();
}

    
