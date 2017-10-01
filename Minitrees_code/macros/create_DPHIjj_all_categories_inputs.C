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



void create_DPHIjj_all_categories_inputs(TString Input_file = "../inputs/mc15_ggH125_30.root",  TString TTree_input = "newtree", TString Output_file = "outputs/output_category_2jet_VH_ggH125_nominal.root",TString Histo_Output_Name="ggH125_BDT", TString var_1 ="BDT_VH_discriminant", TString var_2 ="BDT_VH_discriminant", int const number_of_bin = 10 ,float lower_cut = -1., float upper_cut = 1., float Luminosity_fb = 10., TString cat = "2jet_VH", TString sample = "ggH125"){
bool Raw = false;
  float lower_cut_histo = lower_cut;
  float upper_cut_histo = upper_cut;
   /* 
  if(cat=="2jet_VH" || cat=="2jet_VBF"){
	lower_cut = -1.;
	upper_cut = 1.;
  }
*/
  TH1F *histo_output = new TH1F (Histo_Output_Name,Histo_Output_Name,number_of_bin, lower_cut_histo, upper_cut_histo);

  TFile *f = new TFile(Input_file);
  TTree *tree_input = (TTree*)f->Get(TTree_input);
    
  Float_t variable_1;
  Float_t variable_2;
  Float_t weight;
  Float_t weight_jet;
  Float_t w_lumi;
  Int_t n_jets;
  Float_t m4l_constrained;
  Float_t pt4l_constrained;
  Float_t dijet_invmass;
  Int_t prod_type;
  Float_t ggF_ME;
  Float_t discriminant;
  Int_t pass_vtx4lCut;
  Float_t jet_pt[4];
  Float_t jet_eta[4];
  Float_t jet_phi[4];
  Float_t leading_jet_pt;

  Float_t phi_jet_1;
  Float_t phi_jet_2;

  Float_t dijet_phi = 0.;

  tree_input->SetBranchAddress("pass_vtx4lCut",&pass_vtx4lCut);
  tree_input->SetBranchAddress("weight",&weight);
  tree_input->SetBranchAddress("weight_jet",&weight_jet);
  tree_input->SetBranchAddress("w_lumi",&w_lumi);
  tree_input->SetBranchAddress("prod_type",&prod_type);
  tree_input->SetBranchAddress(var_1,&variable_1);
  tree_input->SetBranchAddress("m4l_constrained",&m4l_constrained);
  tree_input->SetBranchAddress("pt4l_constrained",&pt4l_constrained);
  tree_input->SetBranchAddress("jet_pt",&jet_pt);
  tree_input->SetBranchAddress("jet_phi",&jet_phi);
  tree_input->SetBranchAddress("jet_eta",&jet_eta);


  float tree_input_nentries = (float)tree_input->GetEntries();
  std::cout << "# of entries total = " << tree_input_nentries << std::endl;

  for (Int_t i=0; i<tree_input_nentries; i++) {
      tree_input->GetEntry(i);
      leading_jet_pt = jet_pt[0];

//calculating the DPHIjj according to Andrea
/*
      if(jet_eta[0] > jet_eta[1]){
	phi_jet_1 = jet_phi[0];
        phi_jet_2 = jet_phi[1];
      }else{
        phi_jet_1 = jet_phi[1];
        phi_jet_2 = jet_phi[0];
      }

      dijet_phi= TMath::Abs(phi_jet_1-phi_jet_2);
      if ( dijet_phi > TMath::Pi()) dijet_phi = dijet_phi - 2*TMath::Pi();
*/
      if (jet_eta[0]>jet_eta[1]) dijet_phi= jet_phi[0]-jet_phi[1];

      if (jet_eta[0]<jet_eta[1]) dijet_phi= jet_phi[1]-jet_phi[0];

      

      if ( dijet_phi <0 ) dijet_phi = dijet_phi + 2*3.141592;


      discriminant = 1;
      discriminant = variable_1;
      if(cat=="2jet_VH") discriminant = dijet_phi;
      if(cat=="2jet_VBF") discriminant = dijet_phi;



//      if(discriminant>=lower_cut && discriminant<=upper_cut ){
	if(true){
          if ( (m4l_constrained<118.)||(m4l_constrained>129.) ) continue;
	    if ( pass_vtx4lCut != 1) continue;

	    if(Raw){
		 weight = 1.;
		 w_lumi = 1.;
		 Luminosity_fb = 1.;
	    }
            if ( (cat=="0jet")&&(prod_type==0) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/w_lumi));
            if ( (cat=="1jet")&&(prod_type==1) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/w_lumi));
            if ( (cat=="2jet_VH")&&(prod_type==2) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/w_lumi ));
            if ( (cat=="2jet_VBF")&&(prod_type==3) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/w_lumi));
            if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/w_lumi));

// 3 pt4l categories for 1 jet
            if ( prod_type==1 ){
                if(cat=="1jet_highpt4l" && pt4l_constrained > 120.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
                if(cat=="1jet_lowpt4l" && pt4l_constrained <= 60.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
                if(cat=="1jet_mediumpt4l" && pt4l_constrained >60 && pt4l_constrained <=120.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
		
	    }
// 2 pt4l categories for VBF
            if ( prod_type==3 ){
                if(cat=="2jet_VBF_highpt4l" && pt4l_constrained > 120.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
                if(cat=="2jet_VBF_lowpt4l" && pt4l_constrained <= 120.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
            }
// 2 pt4l categories for VH
            if ( prod_type==2 ){
                if(cat=="2jet_VH_highpt4l" && pt4l_constrained > 120.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
                if(cat=="2jet_VH_lowpt4l" && pt4l_constrained <= 120.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
            }
// 2 pt jet categories for VBF
            if ( prod_type==3 ){
                if(cat=="2jet_VBF_highptjet" && leading_jet_pt > 200.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
                if(cat=="2jet_VBF_lowptjet" && leading_jet_pt <= 200.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
            }
// 2 pt jet categories for VH
            if ( prod_type==2 ){
                if(cat=="2jet_VH_highptjet" && leading_jet_pt > 150.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
                if(cat=="2jet_VH_lowptjet" && leading_jet_pt <= 150.) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /w_lumi ));
            }
 

      }

  }
  
  std::cout<<"histo_output->Integral() = "<<histo_output->Integral()<<std::endl;  


  TFile *fout = new TFile(Output_file, "RECREATE");
  histo_output->Write();
  fout->Close();
  f->Close();
  delete histo_output;  
  
}

    
