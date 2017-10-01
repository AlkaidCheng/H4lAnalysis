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



void create_inputs_normsyst_correct_lumi(TString Input_file = "/afs/cern.ch/atlas/groups/HSG2.1/Minitrees/Prod_v02/mc/Systematics/NormSystematic/mc15_13TeV.343247.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0p.root",  TString TTree_input = "tree_incl_all", TString Output_file = "outputs/output_category_2jet_VH_ggH125_nominal.root",TString Histo_Output_Name="ggH125_BDT", TString var_1 ="BDT_VH_discriminant", TString var_2 ="BDT_VH_discriminant", int const number_of_bin = 10 ,float lower_cut = -1., float upper_cut = 1., float Luminosity_fb = 10., TString cat = "2jet_VH", TString sample = "ggH125", TString norm_syst_type = "weight_HOEW_QCD_syst__1up"){


  std::cout << "running on " << Input_file << std::endl;
    

  TH1F *histo_output = new TH1F (Histo_Output_Name,Histo_Output_Name,number_of_bin, lower_cut, upper_cut);

  TFile *f = new TFile(Input_file);
  TTree *tree_input = (TTree*)f->Get(TTree_input);
    
  Float_t variable_1;
  Float_t variable_2;
  Double_t weight;
  Double_t weight_jet;
  Double_t w_lumi;
  Int_t n_jets;
  Float_t m4l_constrained;
  Float_t pt4l_fsr;
  Float_t dijet_invmass;
  Int_t prod_type;
  Float_t ggF_ME;
  Float_t discriminant;
  Int_t pass_vtx4lCut;
  Double_t w_syst;
  Float_t leading_jet_pt;
  Float_t jet_pt[4];
  Double_t weight_couplings;
  Float_t w_br;

  tree_input->SetBranchAddress(norm_syst_type,&w_syst);
  tree_input->SetBranchAddress("pass_vtx4lCut",&pass_vtx4lCut);
  tree_input->SetBranchAddress("weight",&weight);
//  tree_input->SetBranchAddress("weight_jet",&weight_jet);
//  tree_input->SetBranchAddress("w_lumi",&w_lumi);
  tree_input->SetBranchAddress("prod_type",&prod_type);
//  tree_input->SetBranchAddress(var_1,&variable_1);
  tree_input->SetBranchAddress("m4l_constrained",&m4l_constrained);
  tree_input->SetBranchAddress("leading_jet_pt",&leading_jet_pt);
  tree_input->SetBranchAddress("pt4l_fsr",&pt4l_fsr);
  tree_input->SetBranchAddress("jet_pt",&jet_pt);
  tree_input->SetBranchAddress("weight_couplings",&weight_couplings);
//  tree_input->SetBranchAddress("w_br",&w_br);


  float tree_input_nentries = (float)tree_input->GetEntries();
  std::cout << "# of entries total = " << tree_input_nentries << std::endl;

  //forcing luminosity to 1
//  Luminosity_fb = 1.;

  for (Int_t i=0; i<tree_input_nentries; i++) {
      tree_input->GetEntry(i);
      leading_jet_pt = jet_pt[0];

      discriminant = variable_1;
      weight_jet = weight_couplings;

      if(discriminant>=lower_cut && discriminant<=upper_cut ){

          if ( (m4l_constrained<118.)||(m4l_constrained>129.) ) continue;
	    if ( pass_vtx4lCut != 1) continue;
//		if(w_syst == 0) std::cout << "w_syst = " << w_syst << std::endl;

            if ( (cat=="0jet")&&(prod_type==0) ) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
            if ( (cat=="1jet")&&(prod_type==1) ) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
            if ( (cat=="2jet_VH")&&(prod_type==2) ) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1. ));
            if ( (cat=="2jet_VBF")&&(prod_type==3) ) histo_output->Fill(discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
            if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1. ));
	    if ( (cat=="ttH")&&(prod_type==5) ) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1. ));

	    if ((cat=="2jet_merged")&&(prod_type==3 || prod_type==2)) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1. ));

/*
            if ( (cat=="0jet")&&(prod_type==0) ) histo_output->Fill( discriminant, w_syst*Luminosity_fb);
            if ( (cat=="1jet")&&(prod_type==1) ) histo_output->Fill( discriminant, w_syst*Luminosity_fb);
            if ( (cat=="2jet_VH")&&(prod_type==2) ) histo_output->Fill( discriminant, w_syst*Luminosity_fb);
            if ( (cat=="2jet_VBF")&&(prod_type==3) ) histo_output->Fill(discriminant, w_syst*Luminosity_fb);
            if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill( discriminant, w_syst*Luminosity_fb);
*/	  
            if ( prod_type==1 ){
                if(cat=="1jet_highpt4l" && pt4l_fsr > 120.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
                if(cat=="1jet_lowpt4l" && pt4l_fsr <= 60.) histo_output->Fill( discriminant,weight_jet*w_syst*Luminosity_fb*(1.));
                if(cat=="1jet_mediumpt4l" && pt4l_fsr >60 && pt4l_fsr <=120.) histo_output->Fill( discriminant,weight_jet*w_syst*Luminosity_fb*(1.));

            }
            if ( prod_type==3 ){
                if(cat=="2jet_VBF_highpt4l" && pt4l_fsr > 120.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
                if(cat=="2jet_VBF_lowpt4l" && pt4l_fsr <= 120.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
            }
            if ( prod_type==2 ){
                if(cat=="2jet_VH_highpt4l" && pt4l_fsr > 120.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
                if(cat=="2jet_VH_lowpt4l" && pt4l_fsr <= 120.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
            }
            if ( prod_type==3 ){
                if(cat=="2jet_VBF_highptjet" && leading_jet_pt > 200.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
                if(cat=="2jet_VBF_lowptjet" && leading_jet_pt <= 200.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
            }
            if ( prod_type==2 ){
                if(cat=="2jet_VH_highptjet" && leading_jet_pt > 150.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
                if(cat=="2jet_VH_lowptjet" && leading_jet_pt <= 150.) histo_output->Fill( discriminant, weight_jet*w_syst*Luminosity_fb*(1.));
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

    
