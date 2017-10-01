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


void add_inputs_mg5_all(TString Histo_Output_Name_pre="data_run_2", int const number_of_bin = 20, float lower_cut = -0.9, float upper_cut = 0.9, TString cat = "0jet_ptbin0", TString systematics ="nominal"){
//'vbfMG5_0p_lep','vbfMG5_0mkAv15_lep','vbfMG5_0pphmkAv5_lep','vbfMG5_0pphmkAvm5_lep','vbfMG5_0pphmkAv2p5_lep','vbfMG5_0pphmkAvm2p5_lep'
  TString Histo_Output_Name = "";
  if(Histo_Output_Name_pre=="vbfMG5_0p_lep") Histo_Output_Name = "vbfMG5_0p";
  if(Histo_Output_Name_pre=="vbfMG5_0mkAv15_lep") Histo_Output_Name = "vbfMG5_0mkAv15";
  if(Histo_Output_Name_pre=="vbfMG5_0pphmkAv5_lep") Histo_Output_Name = "vbfMG5_0pphmkAv5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphmkAvm5_lep") Histo_Output_Name = "vbfMG5_0pphmkAvm5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphmkAv2p5_lep") Histo_Output_Name = "vbfMG5_0pphmkAv2p5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphmkAvm2p5_lep") Histo_Output_Name = "vbfMG5_0pphmkAvm2p5";

  if(Histo_Output_Name_pre=="vbfMG5_0phkHv10_lep") Histo_Output_Name = "vbfMG5_0phkHv10";
  if(Histo_Output_Name_pre=="vbfMG5_0pphkHv5_lep") Histo_Output_Name = "vbfMG5_0pphkHv5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphkHvm5_lep") Histo_Output_Name = "vbfMG5_0pphkHvm5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphkHv2p5_lep") Histo_Output_Name = "vbfMG5_0pphkHv2p5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphkHvm2p5_lep") Histo_Output_Name = "vbfMG5_0pphkHvm2p5";

  if(Histo_Output_Name_pre=="vbfMG5_0pphm_khv2p5_kAvm5_lep") Histo_Output_Name = "vbfMG5_0pphm_khv2p5_kAvm5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphm_khv5_kAv5_lep") Histo_Output_Name = "vbfMG5_0pphm_khv5_kAv5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphm_khv5_kAv6_lep") Histo_Output_Name = "vbfMG5_0pphm_khv5_kAv6";
  if(Histo_Output_Name_pre=="vbfMG5_0pphm_khvm2p5_kAvm5_lep") Histo_Output_Name = "vbfMG5_0pphm_khvm2p5_kAvm5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphm_khvm5_kAv5_lep") Histo_Output_Name = "vbfMG5_0pphm_khvm5_kAv5";
  if(Histo_Output_Name_pre=="vbfMG5_0pphm_khvm5_kAvm6_lep") Histo_Output_Name = "vbfMG5_0pphm_khvm5_kAvm6";

  std::cout << "adding inputs to data" << std::endl;
  std::cout << "Histo_Output_Name_pre= " << Histo_Output_Name_pre << "      while Histo_Output_Name= "<< Histo_Output_Name << std::endl;
  


  TH1F *histo_output = new TH1F (Histo_Output_Name+"_BDT",Histo_Output_Name+"_BDT",number_of_bin, lower_cut, upper_cut);

  TString s_vbf_had = "outputs"+cat+"/output_category_"+cat+"_"+Histo_Output_Name+"_had_"+systematics+".root";
  TString s_vbf_lep = "outputs"+cat+"/output_category_"+cat+"_"+Histo_Output_Name+"_lep_"+systematics+".root";


  TFile *f_vbf_had = new TFile(s_vbf_had);
  TFile *f_vbf_lep = new TFile(s_vbf_lep);


  TH1F *h_vbf_had = (TH1F*)f_vbf_had->Get(Histo_Output_Name+"_had_BDT");
  TH1F *h_vbf_lep = (TH1F*)f_vbf_lep->Get(Histo_Output_Name+"_lep_BDT");

  float total_mg5 = 0.;
  total_mg5 = h_vbf_had->Integral() + h_vbf_lep->Integral();

  
  histo_output->Add(h_vbf_had);
  histo_output->Add(h_vbf_lep);


  std::cout<<"h_vbf_had->Integral() = "<<h_vbf_had->Integral()<<std::endl;
  std::cout<<"h_vbf_lep->Integral() = "<<h_vbf_lep->Integral()<<std::endl; 
  std::cout<<"histo_output->Integral() = "<<histo_output->Integral()<<std::endl;  


  TString s_data = "outputs"+cat+"/output_category_"+cat+"_"+Histo_Output_Name+"_"+systematics+".root";

  TFile *fout = new TFile(s_data, "RECREATE");
  histo_output->Write();
  fout->Close();
  delete h_vbf_had;
  delete h_vbf_lep;

  f_vbf_had->Close();
  f_vbf_lep->Close();
  delete histo_output;
//  delete h_vbf_had;
//  delete h_vbf_lep;
}

    
