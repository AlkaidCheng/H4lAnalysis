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



void create_ICHEP_inputs(TString Input_file = "../inputs/mc15_ggH125_30.root",  TString TTree_input = "newtree", TString Output_file = "outputs/output_category_2jet_VH_ggH125_nominal.root",TString Histo_Output_Name="ggH125_BDT", TString var_1 ="BDT_VH_discriminant", TString var_2 ="BDT_VH_discriminant", int const number_of_bin = 10 ,float lower_cut = -1., float upper_cut = 1., float Luminosity_fb = 10., TString cat = "2jet_VH", TString sample = "ggH125"){


  std::cout << "running on " << Input_file << std::endl;
    

  TH1F *histo_output = new TH1F (Histo_Output_Name,Histo_Output_Name,number_of_bin, lower_cut, upper_cut);

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
/*
  tree_input->SetBranchAddress(var_1,&variable_1);
  tree_input->SetBranchAddress("pt4l_constrained",&pt4l_constrained);
  tree_input->SetBranchAddress("weight",&weight);
  tree_input->SetBranchAddress("w_lumi",&w_lumi);
  if ( (cat=="2jet_VBF")||(cat=="2jet_VH") ){
      tree_input->SetBranchAddress("dijet_invmass",&dijet_invmass);
      //tree_input->SetBranchAddress("ggF_ME",&ggF_ME);
  }
  tree_input->SetBranchAddress("n_jets",&n_jets);  
  if ( cat=="VHlep" ) tree_input->SetBranchAddress("prod_type",&prod_type);
  tree_input->SetBranchAddress(var_1,&variable_1);
  tree_input->SetBranchAddress("m4l_constrained",&m4l_constrained);
  //tree_input->SetBranchAddress("dijet_invmass",&dijet_invmass);
*/
  tree_input->SetBranchAddress("pass_vtx4lCut",&pass_vtx4lCut);
  tree_input->SetBranchAddress("weight",&weight);
  tree_input->SetBranchAddress("weight_jet",&weight_jet);
  tree_input->SetBranchAddress("w_lumi",&w_lumi);
  tree_input->SetBranchAddress("prod_type",&prod_type);
  tree_input->SetBranchAddress(var_1,&variable_1);
  tree_input->SetBranchAddress("m4l_constrained",&m4l_constrained);


  float tree_input_nentries = (float)tree_input->GetEntries();
  std::cout << "# of entries total = " << tree_input_nentries << std::endl;

/*    
  for (Int_t i=0; i<tree_input_nentries; i++) {
    tree_input->GetEntry(i);

    discriminant = variable_1;
  
    if(discriminant>lower_cut && discriminant<upper_cut ){

	if( m4l_constrained<118. || m4l_constrained>129. ) continue;
	//std::cout<<"discriminant = "<<discriminant<<std::endl;
	//std::cout<<"n_jets = "<<n_jets<<std::endl;
	//std::cout<<"njets = "<<njets<<std::endl;
	//std::cout<<"m4l_constrained = "<<m4l_constrained<<std::endl;
	//std::cout<<"pt4l_constrained = "<<pt4l_constrained<<std::endl;
	//std::cout<<"dijet_invmass = "<<dijet_invmass<<std::endl;

	if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill(discriminant,Luminosity_fb*(weight/w_lumi));
	if (prod_type!=4){	
	    if ( (cat=="0jet")&&(n_jets==0) ) histo_output->Fill(discriminant,Luminosity_fb*(weight/w_lumi));
	    if ( (cat=="1jet")&&(n_jets==1) ) histo_output->Fill(discriminant,Luminosity_fb*(weight/w_lumi));
	    if ( (cat=="2jet_VBF")&&(n_jets>=2)&&(dijet_invmass>120.) ) histo_output->Fill(discriminant,Luminosity_fb*(weight/w_lumi));
	    if ( (cat=="2jet_VH")&&(n_jets>=2)&&(dijet_invmass<=120.)&&(dijet_invmass>=40.) ){
		histo_output->Fill(discriminant,Luminosity_fb*(weight/w_lumi));
	    }	
	    
	}
	
    } 
    
  }
  */

  for (Int_t i=0; i<tree_input_nentries; i++) {
      tree_input->GetEntry(i);
      discriminant = 1;
//      if ( cat!="VHlep" ) 
      discriminant = variable_1;
//      if ( cat=="VHlep" ) discriminant = 119.;

      if(discriminant>=lower_cut && discriminant<=upper_cut ){

          if ( (m4l_constrained<118.)||(m4l_constrained>129.) ) continue;
//	  std::cout << "pass_vtx4lCut = " << pass_vtx4lCut << std::endl;
	    if ( pass_vtx4lCut != 1) continue;
/*
            if ( (cat=="0jet")&&(prod_type==0) ) histo_output->Fill( discriminant,1);
            if ( (cat=="1jet")&&(prod_type==1) ) histo_output->Fill( discriminant,1);
            if ( (cat=="2jet_VH")&&(prod_type==2) ) histo_output->Fill( discriminant,1);
            if ( (cat=="2jet_VBF")&&(prod_type==3) ) histo_output->Fill(discriminant,1);
            if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill( discriminant,1);
*/


/*
            if ( (cat=="0jet")&&(prod_type==0) ) histo_output->Fill( discriminant,Luminosity_fb*(weight/w_lumi));
            if ( (cat=="1jet")&&(prod_type==1) ) histo_output->Fill( discriminant,Luminosity_fb*(weight/w_lumi));
            if ( (cat=="2jet_VH")&&(prod_type==2) ) histo_output->Fill( discriminant,Luminosity_fb*(weight/w_lumi));
            if ( (cat=="2jet_VBF")&&(prod_type==3) ) histo_output->Fill(discriminant,Luminosity_fb*(weight/w_lumi));
            if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill( discriminant,Luminosity_fb*(weight/w_lumi));
*/

            if ( (cat=="0jet")&&(prod_type==0) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet /* /w_lumi*/));
            if ( (cat=="1jet")&&(prod_type==1) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/* /w_lumi*/));
            if ( (cat=="2jet_VH")&&(prod_type==2) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/*/w_lumi*/));
            if ( (cat=="2jet_VBF")&&(prod_type==3) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/*/w_lumi*/));
            if ( (cat=="VHlep")&&(prod_type==4) ) histo_output->Fill( discriminant,Luminosity_fb*(weight_jet/*/w_lumi*/));
	  
      }

  }
  
  std::cout<<"histo_output->Integral() = "<<histo_output->Integral()<<std::endl;  


  TFile *fout = new TFile(Output_file, "RECREATE");
  histo_output->Write();
  fout->Close();
  f->Close();
  delete histo_output;  
  
}

    
