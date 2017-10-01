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


void sumup_ZZ_contributions(TString Histo_Output_Name="data_run_2_BDT", int const number_of_bin = 1, float lower_cut = -1, float upper_cut = 1, TString cat = "0jet", TString bsm = "ggH_SM"){


  std::cout << "adding inputs to data" << std::endl;
  
  TH1F *histo_output = new TH1F ("ZZllll_mll4_BDT","ZZllll_mll4_BDT",number_of_bin, lower_cut, upper_cut);

  TString s_ZZ_mll4_1 = "outputs"+cat+"/output_category_"+cat+"_ZZllll_mll4_1_nominal.root";
  TString s_ZZ_mll4_2 = "outputs"+cat+"/output_category_"+cat+"_ZZllll_mll4_2_nominal.root";
  TString s_ZZ_mll4_3 = "outputs"+cat+"/output_category_"+cat+"_ZZllll_mll4_3_nominal.root";

  TFile *f_ZZ_mll4_1 = new TFile(s_ZZ_mll4_1);
  TFile *f_ZZ_mll4_2 = new TFile(s_ZZ_mll4_2);
  TFile *f_ZZ_mll4_3 = new TFile(s_ZZ_mll4_3);

  TH1F *h_ZZ_mll4_1 = (TH1F*)f_ZZ_mll4_1->Get("ZZllll_mll4_1_BDT");
  TH1F *h_ZZ_mll4_2 = (TH1F*)f_ZZ_mll4_2->Get("ZZllll_mll4_2_BDT");
  TH1F *h_ZZ_mll4_3 = (TH1F*)f_ZZ_mll4_3->Get("ZZllll_mll4_3_BDT");

  histo_output->Add(h_ZZ_mll4_1);
  histo_output->Add(h_ZZ_mll4_2);
  histo_output->Add(h_ZZ_mll4_3);

  TString s_data = "outputs"+cat+"/output_category_"+cat+"_ZZllll_mll4_nominal.root";

  TFile *fout = new TFile(s_data, "RECREATE");
  histo_output->Write();

  delete histo_output;
  delete h_ZZ_mll4_1;
  delete h_ZZ_mll4_2;
  delete h_ZZ_mll4_3;

  fout->Close();
  f_ZZ_mll4_1->Close();
  f_ZZ_mll4_2->Close();

}

   
 
