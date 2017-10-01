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


void add_inputs_BSM_tth_ggf(TString Histo_Output_Name="data_run_2_BDT", int const number_of_bin = 1, float lower_cut = -1, float upper_cut = 1, TString cat = "0jet", TString bsm = "ggH_SM"){


  std::cout << "adding inputs to data" << std::endl;
  
  TH1F *histo_output = new TH1F (Histo_Output_Name,Histo_Output_Name,number_of_bin, lower_cut, upper_cut);

  TString s_ggh = "outputs"+cat+"/output_category_"+cat+"_"+bsm+"_nominal.root";
/*mc15_13TeV.341505.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_ggH125_ZZ4lep_noTau_nominal.root";*/
  TString s_vbf_bsm = "outputs"+cat+"/output_category_"+cat+"_vbfMG5_0p_nominal.root";
  TString s_ZZ_mll4 = "outputs"+cat+"/output_category_"+cat+"_ZZllll_mll4_nominal.root";
//  TString s_ttbar = "outputs/output_category_"+cat+"_ttbar_nominal.root";
  TString s_Redu_Bkg = "outputs"+cat+"/output_category_"+cat+"_Redu_Bkg_nominal.root";
  TString s_gg_ZZ_4l = "outputs"+cat+"/output_category_"+cat+"_gg_ZZ_bkg_4l_noTau_nominal.root";
  TString s_gg_ZZ_2mu2e = "outputs"+cat+"/output_category_"+cat+"_gg_ZZ_bkg_2e2mu_nominal.root";
  TString s_ttH = "outputs"+cat+"/output_category_"+cat+"_ttH_nominal.root";
  TString s_OTHER_REDU = "outputs"+cat+"/output_category_"+cat+"_MC_REDU_BKG_nominal.root";
  TString s_bbH_SUM = "outputs"+cat+"/output_category_"+cat+"_bbH_SUM_nominal.root";

//  TString s_data = "outputs/output_category_"+cat+"_MC_REDU_BKG_nominal.root";
//
//ttH
/*mc15_13TeV.342556.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZllll_mll4_m4l_100_150_nominal.root";*/
  
  TFile *f_ggh = new TFile(s_ggh);
  TFile *f_vbf_bsm = new TFile(s_vbf_bsm);
  TFile *f_ZZ_mll4 = new TFile(s_ZZ_mll4);
  TFile *f_Redu_Bkg = new TFile(s_Redu_Bkg);
  TFile *f_gg_ZZ_4l = new TFile(s_gg_ZZ_4l);
  TFile *f_gg_ZZ_2mu2e = new TFile(s_gg_ZZ_2mu2e);
  TFile *f_ttH = new TFile(s_ttH);
  TFile *f_OTHER_REDU = new TFile(s_OTHER_REDU);
  TFile *f_bbH_SUM = new TFile(s_bbH_SUM);


  TString var = bsm+"_BDT";
  TH1F *h_ggh = (TH1F*)f_ggh->Get(var);
  TH1F *h_vbf_bsm = (TH1F*)f_vbf_bsm->Get("vbfMG5_0p_BDT");
  TH1F *h_ZZ_mll4 = (TH1F*)f_ZZ_mll4->Get("ZZllll_mll4_BDT");
  TH1F *h_Redu_Bkg = (TH1F*)f_Redu_Bkg->Get("Redu_Bkg_BDT");
  TH1F *h_gg_ZZ_4l = (TH1F*)f_gg_ZZ_4l->Get("gg_ZZ_bkg_4l_noTau_BDT");
  TH1F *h_gg_ZZ_2mu2e = (TH1F*)f_gg_ZZ_2mu2e->Get("gg_ZZ_bkg_2e2mu_BDT");
  TH1F *h_ttH = (TH1F*)f_ttH->Get("ttH_BDT");
  TH1F *h_OTHER_REDU = (TH1F*)f_OTHER_REDU->Get("MC_REDU_BKG_BDT");
  TH1F *h_bbH_SUM = (TH1F*)f_bbH_SUM->Get("bbH_SUM_BDT");


  std::cout<<"************************ var =  " << var <<std::endl;

  histo_output->Add(h_ggh);
  histo_output->Add(h_vbf_bsm);
  histo_output->Add(h_ZZ_mll4);
  histo_output->Add(h_Redu_Bkg);  
  histo_output->Add(h_gg_ZZ_4l);
  histo_output->Add(h_gg_ZZ_2mu2e);
  histo_output->Add(h_ttH);
  histo_output->Add(h_OTHER_REDU);
  histo_output->Add(h_bbH_SUM);

  std::cout<<"histo_output->Integral() = "<<histo_output->Integral()<<std::endl;  

  TString s_data = "outputs"+cat+"/output_category_"+cat+"_data_run_2_nominal.root";

  TFile *fout = new TFile(s_data, "RECREATE");
  histo_output->Write();

  delete histo_output;
  delete h_ggh;
  delete h_vbf_bsm;
  delete h_ZZ_mll4;
  delete h_Redu_Bkg;
  delete h_gg_ZZ_4l;
  delete h_gg_ZZ_2mu2e;
  delete h_ttH;
  delete h_OTHER_REDU;
  delete h_bbH_SUM;


  fout->Close();
  f_ggh->Close();
  f_vbf_bsm->Close();
  f_ZZ_mll4->Close();
//  f_ttbar->Close();
  f_Redu_Bkg->Close();
  f_gg_ZZ_4l->Close();
  f_gg_ZZ_2mu2e->Close();
  f_ttH->Close();
  f_OTHER_REDU->Close();
  f_bbH_SUM->Close();

}

   
 
