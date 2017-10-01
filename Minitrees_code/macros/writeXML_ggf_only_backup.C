#include "TH1D.h"
#include "TFile.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TMath.h"
#include "TStyle.h"
#include "THStack.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include "TSystem.h"
#include <cstdlib>
#include "TString.h"
#include "TLatex.h"
#include "TROOT.h"
#include <fstream>
#include <utility>
#include <set>


void writeXML_ggf_only(bool add_syst = true, bool add_stat_err = false, bool higgs_theo_syst = false,TString sample_names_file = "macros/samples_SM.txt", TString jetptcut = "35" , bool theo_syst_EFT = false , TString syst_names_file = "macros/systematics.txt"){
    

    if (add_syst) std::cout<<"ADDING EXPERIMENTAL SYSTEMATIC ERRORS"<<std::endl;
    if (add_stat_err) std::cout<<"ADDING STATISTICAL ERRORS"<<std::endl;
    if (higgs_theo_syst) std::cout<<"ADDING THEORETICAL SYSTEMATIC ERRORS"<<std::endl;


    TString norm_factor_file = "macros/norm_factor_EFT_kagg.txt";
    std::ifstream filenorm_factor;
    filenorm_factor.open (norm_factor_file, std::ifstream::in);
    if(!filenorm_factor.is_open()) std::cout<<"Did not manage to open file:"<< filenorm_factor <<std::endl;
    std::vector< TString > norm_factor_vector;
    TString norm_factor;
    while( filenorm_factor >> norm_factor ) norm_factor_vector.push_back( norm_factor );
    
    //EFT DEVE VALERE SOLO PER DUE JET -> Fabio mi ha chiesto di non farlo valere solo per due jet.
    TString coupling_type = "khdz";
    float var_up = 1.1;
    float var_down = 0.9;
    float value_UP = 1.1;
    float value_DOWN = 0.9;
    float value = 1.;
   
    bool skip_syst = false; 
    bool syst_TTVA = false;

    //Samples
    std::ifstream filesample;
    filesample.open (sample_names_file, std::ifstream::in);
    if(!filesample.is_open()) std::cout<<"Did not manage to open file:"<< filesample <<std::endl;
    std::vector< TString > sample;
    TString sample_nam;
    while( filesample >> sample_nam ) sample.push_back( sample_nam );
    
    
    //Regions
    TString region_names_file = "macros/regions.txt";
    std::ifstream fileregion;
    fileregion.open (region_names_file, std::ifstream::in);
    if(!fileregion.is_open()) std::cout<<"Did not manage to open file:"<< fileregion <<std::endl;
    std::vector< TString > region;
    TString region_nam;
    while( fileregion >> region_nam ) region.push_back( region_nam );


    //Systematics
//    TString syst_names_file = "macros/systematics.txt";
    std::ifstream filesyst;
    filesyst.open (syst_names_file, std::ifstream::in);
    if(!filesyst.is_open()) std::cout<<"Did not manage to open file:"<< filesyst <<std::endl;
    std::vector< TString > systematics;
    TString syst_nam;
    while( filesyst >> syst_nam ) systematics.push_back( syst_nam );    
    const int systN = systematics.size();
    
    
    TString directoryrm = ".! rm -rf workspaces/*ZZ.xml";
    gROOT->ProcessLine(directoryrm);
    TString directory = ".! mkdir -p workspaces/";
    gROOT->ProcessLine(directory);
    TFile *f_input;
    TFile *f_input_data;
    

    int basis_BSM = 0;    
    
    for(unsigned int j = 0; j<region.size() ; j++){
	
	basis_BSM = 0;      
	ofstream * optfile = new ofstream;
	
	TString xmlName = "workspaces/category_"+ region[j] +"_ZZ.xml";
	std::cout<<""<<std::endl;
	std::cout << "file name = " << xmlName << std::endl;
	optfile->open(xmlName);
	TString fileName = "outputs/output_category_"+ region[j] +"_data_run_2_nominal.root";
	f_input_data = TFile::Open(fileName);
	if(!f_input_data) continue;
	
	*optfile<<"<!DOCTYPE Channel SYSTEM 'HistFactorySchema.dtd'>" << std::endl;
	*optfile<<std::endl;
	*optfile<<"<Channel Name=\"c_"<<region[j]<<"" <<"\" HistoName=\"\">"<<std::endl;
	*optfile<<"  <Data " <<" InputFile= \"" << "../"+fileName << "\" HistoName=\"data_run_2_BDT\" HistoPath=\"\" />" << std::endl;
	*optfile<<" <StatErrorConfig RelErrorThreshold= \"0 \" ConstraintType=\"Poisson\" />"<<std::endl;
	*optfile<<std::endl;
      

	for(unsigned int k = 0; k<sample.size() ; k++){
	
	    std::cout << "------------------------------------- sample = " << sample[k] << std::endl;

	    TString histoName = sample[k]+"_BDT";
	    TString fileName_sample = "outputs/output_category_"+ region[j] +"_"+sample[k]+"_nominal.root";
	    f_input = TFile::Open(fileName_sample);
	    if(!f_input) continue;
	    //TH1F *h_signal = (TH1F*)f_input->Get(histoName);


            TH1F *h_signal = (TH1F*)f_input->Get(histoName);
            value = h_signal->GetBinContent(1);
            //cout << "value = " << value << endl;

	    
	    *optfile<<"  <Sample Name=\""<<sample[k]<<"\" "<<" InputFile= \"" << "../"+fileName_sample <<"\""  <<" HistoPath=\"\" HistoName=\""<<histoName <<"\" NormalizeByTheory=\"True\">"<<std::endl;

	    if(add_stat_err) *optfile <<"    <StatError Activate=\"True\"/> " <<std::endl;
	    if(!add_stat_err) *optfile <<"    <StatError Activate=\"False\"/> " <<std::endl;
	    
	    std::cout << "DONE nominal "<< std::endl;	    
/*	    
	    if ( (region.at(j)!="0jet_ptbin0")&&(region.at(j)!="0jet_ptbin1")&&(region.at(j)!="VHlep_ptbin0")&&(region.at(j)!="VHlep_ptbin1") ){
		if(add_syst){
		    for(int m = 0 ; m< systN; m++){
			if ( systematics.at(m)!="JET_JER_SINGLE_NP" ){
			    TString fileName_UP = "../outputs/output_category_"+region[j]+"_"+sample[k]+"_"+systematics[m]+"__1up.root";
			    TString fileName_DOWN = "../outputs/output_category_"+region[j]+"_"+sample[k]+"_"+systematics[m]+"__1down.root";
                            // ORDER: StatError | HistoSys | OverallSys | ShapeSys | NormFactor | ShapeFactor )	
			    *optfile<<"    <HistoSys Name=\""<<systematics.at(m)<<"\" HistoFileHigh=\""<<fileName_UP<<"\" HistoNameHigh=\""<<histoName<<"\" HistoFileLow=\""<<fileName_DOWN<<"\" HistoNameLow=\""<<histoName<<"\" />" << std::endl;
			    std::cout << "DONE "<<systematics.at(m)<< std::endl;	    
			} 
			//else if ( systematics.at(m)=="JET_JER_SINGLE_NP" ){
			//    TString fileName_UP = "../outputs/output_category_"+region[j]+"_"+sample[k]+"_"+systematics[m]+"__1up.root";
			//    TString fileName_DOWN = "../outputs/output_category_"+region[j]+"_"+sample[k]+"_nominal.root";
			//    *optfile<<"    <HistoSys Name=\""<<systematics.at(m)<<"\" HistoFileHigh=\""<<fileName_UP<<"\" HistoNameHigh=\""<<histoName<<"\" HistoFileLow=\""<<fileName_DOWN<<"\" HistoNameLow=\""<<histoName<<"\" />" << std::endl;
			//    std::cout << "DONE "<<systematics.at(m)<< std::endl;	    
			//}

		    }
		}//close syst
	    }//close loop on categories without jets
*/	    
//Redu_Muons
//Redu_Elec
//
         if(add_syst){
	  skip_syst = false;
          syst_TTVA = false;
          if(sample[k]=="Redu_Muons" ||  sample[k]=="Redu_Elec"|| sample[k]=="Redu_Bkg" ) skip_syst = true;
          if(sample[k]=="ggH125" ||  sample[k]=="ZZllll_mll4_m4l_100_150" || sample[k]=="ZZllll_mll4" ) syst_TTVA = true;

	  if(!skip_syst){
           for(int m = 0 ; m<systN; m++){
            TString fileName_UP ="";
            TString fileName_DOWN ="";


            if(systematics[m]!="JET_JER_SINGLE_NP"  && systematics[m]!="weight_var_JvtEfficiency" && systematics[m]!="MUONS_TTVA__byHand"){// && systematics[m]!="weight_JvtEfficiencyUp"){
              	fileName_UP = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"__1up.root";
//"outputs/output_category_"
              	TFile *f_input_UP = TFile::Open(fileName_UP);
              	if(!f_input_UP) continue;
              	TH1F *h_signal_UP = (TH1F*)f_input_UP->Get(histoName);
              	value_UP = h_signal_UP->GetBinContent(1);

              	fileName_DOWN = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"__1down.root";

		std::cout << "file = " << fileName_DOWN << std::endl;

              	TFile *f_input_DOWN = TFile::Open(fileName_DOWN);
              	if(!f_input_DOWN) continue;
              	TH1F *h_signal_DOWN = (TH1F*)f_input_DOWN->Get(histoName);
              	value_DOWN = h_signal_DOWN->GetBinContent(1);
              	f_input_DOWN->Close();
              	f_input_UP->Close();

	      	var_up = value_UP / value;
              	var_down = value_DOWN / value;
//pruning by hand

                if( !((var_up)<1.005 && (var_up)>0.995) || !((var_down)<1.005 && (var_down)>0.995) ) {
                if(value > 0.01 && ((var_up - 1. )*(var_down - 1.))<0 ) 	*optfile<<"    <OverallSys Name=\"" << systematics[m] << "\" High= \""<< var_up <<"\" Low=\"" << var_down << "\"/>" << std::endl;
		}
            }
	    if(systematics[m]=="JET_JER_SINGLE_NP" /*|| systematics[m]=="NormSystematic"*/){
		
		if (systematics[m]=="JET_JER_SINGLE_NP") fileName_UP = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"__1up.root";
                if (systematics[m]=="NormSystematic") fileName_UP = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+".root";
                TFile *f_input_UP = TFile::Open(fileName_UP);
                if(!f_input_UP) continue;
                TH1F *h_signal_UP = (TH1F*)f_input_UP->Get(histoName);
                value_UP = h_signal_UP->GetBinContent(1);

                if (systematics[m]=="JET_JER_SINGLE_NP") fileName_DOWN = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"__1up.root";
                if (systematics[m]=="NormSystematic") fileName_DOWN = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+".root";
                std::cout << "file = " << fileName_DOWN << std::endl;

                TFile *f_input_DOWN = TFile::Open(fileName_DOWN);
                if(!f_input_DOWN) continue;
                TH1F *h_signal_DOWN = (TH1F*)f_input_DOWN->Get(histoName);
                value_DOWN = h_signal_DOWN->GetBinContent(1);
                f_input_DOWN->Close();
                f_input_UP->Close();

                var_up = value_UP / value;
                //var_down = value_DOWN / value;
                var_down = value / value_DOWN;		// SYMMETRIZING THE SYSTEMATICS
//pruning by hand
                if( !((var_up)<1.005 && (var_up)>0.995) || !((var_down)<1.005 && (var_down)>0.995) ){
                 if(value > 0.01 && ((var_up - 1. )*(var_down - 1.))<0 ) *optfile<<"    <OverallSys Name=\"" << systematics[m] << "\" High= \""<< var_up <<"\" Low=\"" << var_down << "\"/>" << std::endl;
		}
	    }
//ADDING IT TO SOLVE ISSUE WITH PRODV05 
//weight_JvtEfficiencyDown
//weight_JvtEfficiencyUP
	    if(systematics[m]=="weight_var_JvtEfficiency"){  
		fileName_UP = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"Up.root";
                TFile *f_input_UP = TFile::Open(fileName_UP);
                if(!f_input_UP) continue;
                TH1F *h_signal_UP = (TH1F*)f_input_UP->Get(histoName);
                value_UP = h_signal_UP->GetBinContent(1);
                fileName_DOWN = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"Down.root";
                std::cout << "file = " << fileName_DOWN << std::endl;
                TFile *f_input_DOWN = TFile::Open(fileName_DOWN);
                if(!f_input_DOWN)continue;
                TH1F *h_signal_DOWN = (TH1F*)f_input_DOWN->Get(histoName);
                value_DOWN = h_signal_DOWN->GetBinContent(1);
                f_input_DOWN->Close();
                f_input_UP->Close();


                var_up = value_UP / value;
                var_down = value_DOWN / value;

                *optfile<<"    <OverallSys Name=\"" << systematics[m] << "\" High= \""<< var_up <<"\" Low=\"" << var_down << "\"/>" << std::endl;


	    }

//Haonan add this
//For Prodv06
//Flat 1% for TTVA
/*
           if(systematics[m]=="MUONS_TTVA__byHand" && syst_TTVA == true){
                fileName_UP = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"_up.root";
                TFile *f_input_UP = TFile::Open(fileName_UP);
                if(!f_input_UP) continue;
                TH1F *h_signal_UP = (TH1F*)f_input_UP->Get(histoName);
                value_UP = h_signal_UP->GetBinContent(1);
                fileName_DOWN = "outputs/output_category_"+ region[j]+ "_"+ sample[k] + "_" + systematics[m]+"_down.root";
                std::cout << "file = " << fileName_DOWN << std::endl;
                TFile *f_input_DOWN = TFile::Open(fileName_DOWN);
                if(!f_input_DOWN)continue;
                TH1F *h_signal_DOWN = (TH1F*)f_input_DOWN->Get(histoName);
                value_DOWN = h_signal_DOWN->GetBinContent(1);
                f_input_DOWN->Close();
                f_input_UP->Close();


                var_up = value_UP / value;
                var_down = value_DOWN / value;

                *optfile<<"    <OverallSys Name=\"" << systematics[m] << "\" High= \""<< var_up <<"\" Low=\"" << var_down << "\"/>" << std::endl;


            }
*/


	  }
	 }
        }//close syst

	    
	    if(sample[k].Contains("bfMG5")){
	//	*optfile <<"    <NormFactor Name=\"mu_vbf\" Val=\"1.\" Low=\"0.\" High=\"20.\" Const=\"True\" />" << std::endl;
		if(region[j]=="0jet") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_qqH\" High=\"0.9824\" Low=\"1.0155\" />" << std::endl;
                if(region[j]=="1jet_highpt4l" || region[j]=="1jet_mediumpt4l" || region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_qqH\" High=\"0.9937\" Low=\"1.0096\" />" << std::endl;
                if(region[j]=="2jet_VH_highpt4l" || region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_qqH\" High=\"1.0134\" Low=\"0.9668\" />" << std::endl;
                if(region[j]=="2jet_VBF_highpt4l" || region[j]=="2jet_VBF_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_qqH\" High=\"1.0071\" Low=\"0.9932\" />" << std::endl;
                if(region[j]=="2jet_VBF_highptjet" || region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_qqH\" High=\"1.0071\" Low=\"0.9932\" />" << std::endl;

                if(region[j]=="0jet") *optfile <<"    <OverallSys Name=\"QCDscale_qqH_H4l\" High=\"0.9848\" Low=\"1.0140\" />" << std::endl;
                if(region[j]=="1jet_highpt4l" || region[j]=="1jet_mediumpt4l" || region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"QCDscale_qqH_H4l\" High=\"0.9967\" Low=\"1.0048\" />" << std::endl;
                if(region[j]=="2jet_VH_highpt4l" || region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"QCDscale_qqH_H4l\" High=\"1.0063\" Low=\"0.9906\" />" << std::endl;
                if(region[j]=="2jet_VBF_highpt4l" || region[j]=="2jet_VBF_lowpt4l") *optfile <<"    <OverallSys Name=\"QCDscale_qqH_H4l\" High=\"1.0080\" Low=\"0.9937\" />" << std::endl;
                if(region[j]=="2jet_VBF_highptjet" || region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"QCDscale_qqH_H4l\" High=\"1.0080\" Low=\"0.9937\" />" << std::endl;


                if(region[j]=="0jet") *optfile <<"    <OverallSys Name=\"pdf_Higgs_qqH_H4l\" High=\"1.0029\" Low=\"0.9887\" />" << std::endl;
                if(region[j]=="1jet_highpt4l" || region[j]=="1jet_mediumpt4l" || region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"pdf_Higgs_qqH_H4l\" High=\"1.0029\" Low=\"0.994\" />" << std::endl;
                if(region[j]=="2jet_VH_highpt4l" || region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"pdf_Higgs_qqH_H4l\" High=\"1.0033\" Low=\"0.9942\" />" << std::endl;
                if(region[j]=="2jet_VBF_highpt4l" || region[j]=="2jet_VBF_lowpt4l") *optfile <<"    <OverallSys Name=\"pdf_Higgs_qqH_H4l\" High=\"1.0062\" Low=\"0.9942\" />" << std::endl;
                if(region[j]=="2jet_VBF_highptjet" || region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"pdf_Higgs_qqH_H4l\" High=\"1.0062\" Low=\"0.9942\" />" << std::endl;


                *optfile <<"    <OverallSys Name=\"BR_VV\" High=\"1.027\" Low=\"0.973\" />" << std::endl;
                *optfile <<"    <OverallSys Name=\"QCDscale_qqH_WH_ZH\" High=\"1.009\" Low=\"0.992\" />" << std::endl;
                *optfile <<"    <OverallSys Name=\"pdf_Higgs_VBF_WH_ZH\" High=\"1.02\" Low=\"0.980\" />" << std::endl;


		

	    } else if(sample[k].Contains("ggH125")){
//0.974 1.0264
//		*optfile <<"    <OverallSys Name=\"ATLAS_BR_VV\" High=\"1.0264\" Low=\"0.974\" />" << std::endl;
		*optfile <<"    <NormFactor Name=\"mu_ggh\" Val=\"1.\" Low=\"0.\" High=\"90.\" Const=\"True\" />" << std::endl;

	    } else if( (sample[k]=="WH125")||(sample[k]=="ZH125") ){
		*optfile <<"    <NormFactor Name=\"mu_vh\" Val=\"1.\" Low=\"0.\" High=\"90.\" Const=\"True\" />" << std::endl;
	    } else if( sample[k].Contains("gH") ){

		if(higgs_theo_syst){
/* ST vertical */
                   if(region[j]=="0jet") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_m01\" High=\"0.918\" Low=\"1.089\" />" << std::endl;
                   if(region[j]=="1jet_highpt4l" || region[j]=="1jet_mediumpt4l" || region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_m01\" High=\"1.27\" Low=\"0.7874\" />" << std::endl;
                   if(region[j]=="1jet_highpt4l" || region[j]=="1jet_mediumpt4l" || region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_m12\" High=\"0.72\" Low=\"1.389\" />" << std::endl;

                   if(region[j]=="2jet_VH_highpt4l" || region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_m12\" High=\"1.68\" Low=\"0.5952\" />" << std::endl;
                   if(region[j]=="2jet_VBF_highpt4l" || region[j]=="2jet_VBF_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_m12\" High=\"1.68\" Low=\"0.5952\" />" << std::endl;
                   if(region[j]=="2jet_VBF_highptjet" || region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_m12\" High=\"1.68\" Low=\"0.5952\" />" << std::endl;


/* horizontal FxFx case */
//My Implementation
/*
                   if(region[j]=="1jet_lowpt4l")   *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_1jet_low_med\" High=\"0.824\" Low=\"1.15\" />" << std::endl;
                   if(region[j]=="1jet_mediumpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_1jet_low_med\" High=\"1.22\" Low=\"0.822\" />" << std::endl; ;
                   if(region[j]=="1jet_highpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_1jet_high\" High=\"1.227\" Low=\"0.812\" />" << std::endl; ;
//vbf for pt4l
//vbf for ptj  /
                  if(region[j]=="2jet_VBF_highptjet") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VBF\" High=\"1.293\" Low=\"0.765\" />" << std::endl;
                   if(region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VBF\" High=\"0.73\" Low=\"1.225\" />" << std::endl ;

                   if(region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VH\" High=\"0.771\" Low=\"1.20\" />" << std::endl;
                   if(region[j]=="2jet_VH_highpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VH\" High=\"1.409\" Low=\"0.734\" />" << std::endl;
*/
//Katharina' implementation
                   if(region[j]=="1jet_lowpt4l")   *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_1jet_low_med\" High=\"1.176\" Low=\"0.85\" />" << std::endl;
                   if(region[j]=="1jet_mediumpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_1jet_low_med\" High=\"0.78\" Low=\"1.178\" />" << std::endl; ;
                   if(region[j]=="1jet_highpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_1jet_high\" High=\"0.773\" Low=\"1.188\" />" << std::endl; ;
//vbf for pt4l
////vbf for ptj
                  if(region[j]=="2jet_VBF_highptjet") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VBF\" High=\"0.707\" Low=\"1.235\" />" << std::endl;
                   if(region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VBF\" High=\"1.27\" Low=\"0.775\" />" << std::endl ;

                   if(region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VH\" High=\"1.229\" Low=\"0.80\" />" << std::endl;
                   if(region[j]=="2jet_VH_highpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_QCDscale_ggH_ptH_FxFx_2jet_VH\" High=\"0.591\" Low=\"1.266\" />" << std::endl;

//theo syst from katharina 20 - 1 - 2017
                if(region[j]=="0jet") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_ggH\" High=\"0.9882\" Low=\"1.0121\" />" << std::endl;
                if(region[j]=="1jet_highpt4l" || region[j]=="1jet_mediumpt4l" || region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_ggH\" High=\"1.0099\" Low=\"0.9859\" />" << std::endl;
                if(region[j]=="2jet_VH_highpt4l" || region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_ggH\" High=\"1.0406\" Low=\"0.9999\" />" << std::endl;
                if(region[j]=="2jet_VBF_highpt4l" || region[j]=="2jet_VBF_lowpt4l") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_ggH\" High=\"1.0334\" Low=\"0.9578\" />" << std::endl;
                if(region[j]=="2jet_VBF_highptjet" || region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"ATLAS_UEPS_ggH\" High=\"1.0334\" Low=\"0.9578\" />" << std::endl;
		}

		*optfile <<"    <OverallSys Name=\"pdf_Higgs_gg_ttH\" High=\"1.032\" Low=\"0.968\" />" << std::endl;
                *optfile <<"    <OverallSys Name=\"BR_VV\" High=\"1.027\" Low=\"0.973\" />" << std::endl;
                *optfile <<"    <OverallSys Name=\"QCDscale_bbH_ggH_ttH\" High=\"1.048\" Low=\"0.931\" />" << std::endl;

		*optfile <<"    <NormFactor Name=\"mu_"<< norm_factor_vector[basis_BSM]  <<"\" Val=\"1.\" Low=\"0.\" High=\"90.\" Const=\"True\" />" << std::endl;
//flat 20%
		basis_BSM++;
	    }
	    
	    
//	    if(add_syst) *optfile <<"    <OverallSys Name=\"ATLAS_LUMI\" High=\"1.03\" Low=\"0.97\" />" << std::endl;
// aggiungere sistematiche su reducilbe
	    if(add_syst){
		if(sample[k]=="Redu_Muons"){  
			//errore per categoria
/*
                    if(region[j]=="2jet_VH")*optfile <<"    <OverallSys Name=\"ATLAS_norm_SF_H4l_redbkg\" High=\"1.145\" Low=\"0.855\" />" << std::endl;
*/
		}
                if(sample[k]=="Redu_Elec" ){
			//errore per categoria
/*
                    if(region[j]=="2jet_VH")*optfile <<"    <OverallSys Name=\"ATLAS_norm_SF_H4l_redbkg\" High=\"1.333\" Low=\"0.667\" />" << std::endl;
*/
		} 

                if(sample[k]=="Redu_Bkg" ){


                   if(region[j]=="0jet") *optfile <<"    <OverallSys Name=\"red_bkg_syst_0jet\" High=\"1.0897\" Low=\"0.9103\" />" << std::endl;
                   if(region[j]=="1jet_highpt4l") *optfile <<"    <OverallSys Name=\"red_bkg_syst_1jet\" High=\"1.12\" Low=\"0.88\" />" << std::endl;
                   if(region[j]=="1jet_mediumpt4l") *optfile <<"    <OverallSys Name=\"red_bkg_syst_1jet\" High=\"1.12\" Low=\"0.88\" />" << std::endl;
                   if(region[j]=="1jet_lowpt4l") *optfile <<"    <OverallSys Name=\"red_bkg_syst_1jet\" High=\"1.192\" Low=\"0.808\" />" << std::endl;
                   if(region[j]=="2jet_VBF_highptjet") *optfile << "    <OverallSys Name=\"red_bkg_syst_2jet_vbf\" High=\"1.79\" Low=\"0.21\" />" << std::endl;
                   if(region[j]=="2jet_VH_highpt4l") *optfile << "    <OverallSys Name=\"red_bkg_syst_2jet_vh\" High=\"1.17\" Low=\"0.83\" />" << std::endl;
                   if(region[j]=="2jet_VBF_lowptjet") *optfile <<"    <OverallSys Name=\"red_bkg_syst_2jet_vbf\" High=\"1.162\" Low=\"0.838\" />" << std::endl;
                   if(region[j]=="2jet_VH_lowpt4l") *optfile <<"    <OverallSys Name=\"red_bkg_syst_2jet_vh\" High=\"1.214\" Low=\"0.786\" />" << std::endl;
                   if(region[j]=="VHlep") *optfile <<"    <OverallSys Name=\"red_bkg_syst_vhlep\" High=\"1.17\" Low=\"0.83\" />" << std::endl;
                   if(region[j]=="ttH") *optfile <<"    <OverallSys Name=\"red_bkg_syst_tth\" High=\"1.17\" Low=\"0.83\" />" << std::endl;


                }


//from Yellow report 4
// https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt13TeV
//
                if(sample[k]=="ZZllll"){
		   *optfile <<"    <OverallSys Name=\"QCDscale_VV\" High=\"1.05\" Low=\"0.93\" />" << std::endl;
                   *optfile <<"    <OverallSys Name=\"pdf_qq\" High=\"1.05\" Low=\"0.95\" />" << std::endl;
		}
//ggZZ_4l','ggZZ_2mu2e'
		if(sample[k]=="ggZZ_4l"){

		   *optfile <<"    <OverallSys Name=\"QCDscale_ggVV\" High=\"1.60\" Low=\"0.40\" />" << std::endl;
                   *optfile <<"    <OverallSys Name=\"pdf_gg\" High=\"1.08\" Low=\"0.92\" />" << std::endl;

		}
                if(sample[k]=="ggZZ_2mu2e"){

                   *optfile <<"    <OverallSys Name=\"QCDscale_ggVV\" High=\"1.60\" Low=\"0.40\" />" << std::endl;
                   *optfile <<"    <OverallSys Name=\"pdf_gg\" High=\"1.08\" Low=\"0.92\" />" << std::endl;

                }

	    }
//            *optfile<<"    <OverallSys Name=\"ATLAS_MUON_TTVA_SYS\" High= \"1.01\" Low=\"0.99\"/>" << std::endl;
	    *optfile<<"  <OverallSys Name=\"ATLAS_luminoisty\" High= \"1.037\" Low=\"0.963\"/>"<<std::endl;
	    *optfile<<"  </Sample>"<<std::endl;
	    *optfile<<std::endl;
	    
	}//close sample
	
	*optfile<<"</Channel>"<< std::endl;
	optfile->close();	
	f_input->Close();

    
    }//close region
    
    f_input_data->Close();
	

}
