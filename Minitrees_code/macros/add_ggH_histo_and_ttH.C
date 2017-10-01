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


void add_ggH_histo_and_ttH(TString Histo_Output_Name="ggH_SM", int const number_of_bin = 1, float lower_cut = -1, float upper_cut = 1, TString cat = "0jet", TString systematics ="nominal"){
//'vbfMG5_0p_lep','vbfMG5_0mkAv15_lep','vbfMG5_0pphmkAv5_lep','vbfMG5_0pphmkAvm5_lep','vbfMG5_0pphmkAv2p5_lep','vbfMG5_0pphmkAvm2p5_lep'
  TString s_ggH = "outputs"+cat+"/output_category_"+cat+"_"+Histo_Output_Name+"_"+systematics+"_solo.root";
  TString s_ttH = "outputs"+cat+"/output_category_"+cat+"_ttH_"+systematics+".root";


  TFile *f_ggH = new TFile(s_ggH);
  TFile *f_ttH = new TFile(s_ttH);


  TH1F *h_ggH = (TH1F*)f_ggH->Get(Histo_Output_Name+"_BDT");
  TH1F *h_ttH = (TH1F*)f_ttH->Get("ttH_BDT");


  TH1F *histo_output = new TH1F (Histo_Output_Name+"_BDT",Histo_Output_Name+"_BDT",number_of_bin, lower_cut, upper_cut);
  histo_output->Add(h_ggH);
  histo_output->Add(h_ttH);

  
  
  TString s_data = "outputs"+cat+"/output_category_"+cat+"_"+Histo_Output_Name+"_"+systematics+".root";

  TFile *fout = new TFile(s_data, "RECREATE");
    histo_output->Write();
  delete   histo_output;
  fout->Close();
  delete h_ggH;
  delete h_ttH;

  f_ggH->Close();
  f_ttH->Close();

  
}

    
