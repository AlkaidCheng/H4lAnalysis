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


void create_table(bool add_syst = true, bool add_stat_err = false, bool higgs_theo_syst = false,TString sample_names_file = "macros/samples_BSM_khvv.txt", TString jetptcut = "30" , bool theo_syst_EFT = false, bool percent = false){
    

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
    TString syst_names_file = "macros/systematics.txt";
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
    const int regioni = region.size();
    const int campioni = sample.size() + 1 ;    
    float matrix_of_values[regioni][campioni];
    float matrix_of_errors[regioni][campioni];
    float error = 0.;

    ofstream * optfile = new ofstream;
    TString xmlName = "table_ZZ.txt";
//    std::cout << "file name = " << xmlName << std::endl;
    optfile->open(xmlName);

    
    for(unsigned int j = 0; j<region.size() ; j++){
	
	basis_BSM = 0;      
	
	TString fileName = "outputs"+region[j]+"/output_category_"+ region[j] +"_data_run_2_nominal.root";
	f_input_data = TFile::Open(fileName);
	if(!f_input_data) continue;

	TH1F *h_data = (TH1F*)f_input_data->Get("data_run_2_BDT");
        matrix_of_values[j][0] = h_data->Integral();


	for(unsigned int k = 0; k<sample.size() ; k++){
	
//	    std::cout << "------------------------------------- sample = " << sample[k] << std::endl;

	    TString histoName = sample[k]+"_BDT";
	    TString fileName_sample = "outputs"+region[j]+"/output_category_"+ region[j] +"_"+sample[k]+"_nominal.root";
	    f_input = TFile::Open(fileName_sample);
	    if(!f_input) continue;
	    //TH1F *h_signal = (TH1F*)f_input->Get(histoName);


            TH1F *h_signal = (TH1F*)f_input->Get(histoName);
            value = h_signal->GetBinContent(1);
	    error = h_signal->GetBinError(1);
            //cout << "value = " << value << endl;

            matrix_of_values[j][k+1] = value;
	    matrix_of_errors[j][k+1] = error;
	}//close sample
        f_input->Close();    
    }//close region
    
    f_input_data->Close();

   *optfile << "Sample & data &";
   for(unsigned int kkk = 0; kkk<sample.size() ; kkk++){
	*optfile <<  sample[kkk] <<  "  & ";
   }
//	*optfile << "data " << std::endl;
	*optfile << " "  << std::endl;
//calculate total per region
   float vector_of_sum[campioni];
   for (unsigned int kk = 1; kk<sample.size()+1 ; kk++){
	 vector_of_sum[kk] = 0; //initialize values to 0
     for(unsigned int jj = 0; jj<region.size() ; jj++){
         vector_of_sum[kk] += matrix_of_values[jj][kk] ;
     }
       cout << "for the sample " << sample[kk-1] << "the total is " << vector_of_sum[kk] << endl; 
   }
   for(unsigned int jj = 0; jj<region.size() ; jj++){	
/*
   if(jj==0 )*optfile << "0 jet     && ";
   if(jj==1 )*optfile << "1 jet     && ";
   if(jj==2 )*optfile << "2 jet VBF && ";
   if(jj==3 )*optfile << "2 jet VH  && ";
   if(jj==4 )*optfile << "VH lep    && ";
*/
      *optfile <<  region[jj] <<  "  & ";
      for(unsigned int kk = 0; kk<sample.size()+1 ; kk++){//before there was +1

	if(!percent || kk == 0) *optfile << matrix_of_values[jj][kk] <<  /* "+-"<< matrix_of_errors[jj][kk] <<*/" & " ;
        if(percent && kk != 0) *optfile << (matrix_of_values[jj][kk] / vector_of_sum[kk]) <<  /* "+-"<< matrix_of_errors[jj][kk] <<*/" & " ;

      }
     *optfile << " " << std::endl;
   }
    optfile->close();

}
