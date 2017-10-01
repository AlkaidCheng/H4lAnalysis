// AUTHOR: VALERIO BORTOLOTTO
// date : 23 Feb 2017
#include "TFile.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStopwatch.h"
#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooRandom.h"
#include "RooProduct.h"
#include "RooRealSumPdf.h"
#include "RooFormulaVar.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/ToyMCImportanceSampler.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/RatioOfProfiledLikelihoodsTestStat.h"
#include "RooStats/AsymptoticCalculator.h"
#include "RooStats/FrequentistCalculator.h"
#include <vector>
#include "RooNLLVar.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooMinimizer.h"
#include "RooProfileLL.h"
#include "RooStats/RooStatsUtils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <TMath.h>
#include <set>
#include <sstream>
#include <fstream>
#include <utility>
#include <map>
#include <string>

using namespace RooFit;
using namespace RooStats;

void Likelihood_Test_Value_TTH_FUNCTION(TString wsInput = "workspaces/output_combined_datastat_model.root" , TString workspace="combined" , TString model="ModelConfig" , TString dataName="obsData" /*"obsData" or "asimovData"*/, TString formula_1 ="@1 + @0", TString file_readout = "macros/function_khvv.txt", int number_of_base = 5, TString coupling_name_file = "macros/coupling_name_khvv.txt", bool total_width_fix = true, bool ggf_BR_modification = true, TString ggf_file_readout = "macros/function_ggf.txt", int ggf_number_of_base = 3 , TString syst_names_file = "macros/systematics_fake.txt", bool sm_fix = true , TString region_names_file = "macros/regions.txt" , TString sample_names_file = "macros/samples.txt" ,float coupling_value_input = 1. ){
TString formula_1_ggf = "@1 + @0";
    TString total_width_function = "";

RooProduct* andrea = NULL;
float total_value = 0.;
TString interesting = "";


//    TString region_names_file = region_file;
    std::ifstream fileregion;
    fileregion.open (region_names_file, std::ifstream::in);
    if(!fileregion.is_open()) std::cout<<"Did not manage to open file:"<< fileregion <<std::endl;
    std::vector< TString > region;
    TString region_nam;
    while( fileregion >> region_nam ) region.push_back( region_nam );


    std::ifstream filesample;
    filesample.open (sample_names_file, std::ifstream::in);
    if(!filesample.is_open()) std::cout<<"Did not manage to open file:"<< filesample <<std::endl;
    std::vector< TString > sample;
    TString sample_nam;
    while( filesample >> sample_nam ) sample.push_back( sample_nam );




//    TString syst_names_file = "macros/systematics_fake.txt";
    std::ifstream filesyst;
    filesyst.open (syst_names_file, std::ifstream::in);
    if(!filesyst.is_open()) std::cout<<"Did not manage to open file:"<< filesyst <<std::endl;
    std::vector< TString > systematics;
    TString syst_nam;
    while( filesyst >> syst_nam ) systematics.push_back( syst_nam );

    const int systN = systematics.size();



    float SM_value = -999;
    float BSM_value = -999;
    float m2DLL = -999.;
    float fit_status_ALTHypo = -999;
    float SM_weight = -999.;
    float BSM1_weight = -999.;
    float BSM2_weight = -999.;
    float BSM3_weight = -999.;
    float BSM4_weight = -999.;
    float BR_SCALING = -999.;

    float SM_weight_ggf = -999.;
    float BSM1_weight_ggf = -999.;
    float BSM2_weight_ggf = -999.;


    TH2F *h2f = new TH2F("h2f","Observed Distribution",12,-3,3,11,-3,3);
    std::cout<<"please keep an eye on the coupling order same + prod + dec" << std::endl;

    TTree* tree_scan = new TTree("Likelihood","test statistic");
    tree_scan->Branch("qmu",&m2DLL);
    tree_scan->Branch("fit_status",&fit_status_ALTHypo);
    tree_scan->Branch("SM_coupling",&SM_value);
    tree_scan->Branch("BSM_coupling",&BSM_value);
    tree_scan->Branch("SM_weight",&SM_weight);
    tree_scan->Branch("BSM1_weight",&BSM1_weight);
    tree_scan->Branch("BSM2_weight",&BSM2_weight);
    tree_scan->Branch("BSM3_weight",&BSM3_weight);
    tree_scan->Branch("BSM4_weight",&BSM4_weight);
    tree_scan->Branch("SM_weight_ggf",&SM_weight_ggf);
    tree_scan->Branch("BSM1_weight_ggf",&BSM1_weight_ggf);
    tree_scan->Branch("BSM2_weight_ggf",&BSM2_weight_ggf);
    tree_scan->Branch("BR_SCALING",&BR_SCALING);

    float Systematics_value[systN];
    for(int in_pre = 0 ; in_pre < systN ; in_pre++ ){
	tree_scan->Branch("post_fit_"+systematics[in_pre], &Systematics_value[in_pre]);

    }
  



    std::ifstream fileoutput_coupling;
    fileoutput_coupling.open (coupling_name_file, std::ifstream::in);
    if(!fileoutput_coupling.is_open()) std::cout<<"Did not manage to open file:"<< coupling_name_file <<std::endl;
    
    std::vector< TString > coupling_name;
    TString coup_nam;
    while( fileoutput_coupling >> coup_nam ) coupling_name.push_back( coup_nam );
    
    
    std::cout<<"Your vector of coupling contains = " << coupling_name.size() << "  check if it is ok" << std::endl; 
    for(unsigned int p = 0 ; p < coupling_name.size(); p++) std::cout<<"Your vector of coupling contains ["<< p <<"] = " << coupling_name[p] << std::endl;
    
    
    std::ifstream fileoutput;
    fileoutput.open (file_readout, std::ifstream::in);
    if(!fileoutput.is_open()) std::cout<<"Did not manage to open file:"<< file_readout <<std::endl;

    std::ifstream fileoutput_ggf;
    fileoutput_ggf.open (ggf_file_readout, std::ifstream::in);
    if(!fileoutput_ggf.is_open()) std::cout<<"Did not manage to open file:"<< ggf_file_readout <<std::endl;
    
    
    float status_nll_SM_hypo = -999.;
    TFile* f = new TFile(wsInput);
    
    RooWorkspace* w = (RooWorkspace*) f->Get(workspace);
    ModelConfig* mc = (ModelConfig*) w->obj(model);
    RooAbsPdf* pdf = mc->GetPdf();
    RooArgSet* NP =(RooArgSet*)  mc->GetGlobalObservables();
    RooArgSet* pois = (RooArgSet*) mc->GetParametersOfInterest();
    RooDataSet* data = (RooDataSet*) w->data(dataName);
    

    RooRealVar* Lumi_to_fix;
    Lumi_to_fix = w->var("Lumi");
    Lumi_to_fix->setConstant(true);

    
    std::vector < std::string > mu_names;
    TString nuisance_string_nll = "prova";
    RooRealVar* nuis_SM_nll;
    TIterator *nuis_itr_SM_nll = mc->GetNuisanceParameters()->createIterator();
    while ((nuis_SM_nll=(RooRealVar*)nuis_itr_SM_nll->Next())){
	nuisance_string_nll = nuis_SM_nll->GetName();
	
        if(nuisance_string_nll.Contains("bf_")){
	    cout << " nuisance string  = " << nuisance_string_nll << endl;
	    nuis_SM_nll->setVal(1);
	    nuis_SM_nll->setConstant(true);
            mu_names.push_back(nuis_SM_nll->GetName());
	}
    }

    std::vector < std::string > mu_names_ggf;
    TString nuisance_string_nll_ggf = "prova";
    RooRealVar* nuis_SM_nll_ggf;
    TIterator *nuis_itr_SM_nll_ggf = mc->GetNuisanceParameters()->createIterator();
    while ((nuis_SM_nll_ggf=(RooRealVar*)nuis_itr_SM_nll_ggf->Next())){
        nuisance_string_nll_ggf = nuis_SM_nll_ggf->GetName();

        if(nuisance_string_nll_ggf.Contains("gf_")){
            cout << " nuisance string  = " << nuisance_string_nll_ggf << endl;
            nuis_SM_nll_ggf->setVal(1);
            nuis_SM_nll_ggf->setConstant(true);
	    mu_names_ggf.push_back(nuis_SM_nll_ggf->GetName());
        }
    }
cout << "THIS IS A SIMPLE TEST mu_names_ggf [0] = " << mu_names_ggf[0] << endl;

//adding a branch per syst

   
  int num_np = 0;
  string varName;
  string varName_postfit;

// VALERIO adding syst
/*
    RooRealVar* var_test;
    TIterator *vitr_test = mc->GetNuisanceParameters()->createIterator();
    while ((var_test=(RooRealVar*)vitr_test->Next())){
        num_np++;
    }
  const int number_of_np = num_np;

  float Prefit_nllALTHYPO[number_of_np];

  TIterator *vitr_branch = mc->GetNuisanceParameters()->createIterator();
  RooRealVar* var_branch;
  int internal = 0;
  while ((var_branch=(RooRealVar*)vitr_branch->Next())){
    varName = var_branch->GetName();
    string varName_to_use_prefit_nllALTHYPO = "prefit_nllALTHYPO_"+ varName;
    tree_scan->Branch(varName_to_use_prefit_nllALTHYPO.c_str(), &Prefit_nllALTHYPO[internal]);
    internal++;
  }
*/


    RooRealVar* ga = new RooRealVar (coupling_name[0],coupling_name[0],1);
    RooRealVar* gb = new RooRealVar (coupling_name[1],coupling_name[1],0.,-50.,50.);
    RooRealVar* gc = new RooRealVar ("gc","gc",1);
    RooRealVar* gcc = new RooRealVar ("gcc","gcc",0);
    ga->setConstant(true);
    gb->setConstant(true);
    RooArgList normList(*ga,*gb);

    if(coupling_name[1]=="khvv"){
	total_width_function = "(1/( 0.76011 + 0.02619*(@0*@0 + 0.001281*@1*@1 + 0.060305*@0*@1) + 0.2137*(@0*@0 + 0.002955625*@1*@1 + 0.089193*@0*@1)))";
    }
    if(coupling_name[1]=="kavv"){
        total_width_function = "(1/( 0.76011 + 0.02619*(@0*@0 + 0.00052654431*@1*@1 - 0.00035524*@0*@1) + 0.2137*(@0*@0 + 0.00121828543*@1*@1 - 0.000091675*@0*@1)))";
        }
    if(!total_width_fix) total_width_function = "1.";
    
    std::vector< std::string > values;
    string val;
    int value_I_WANT = 0;
    while( fileoutput >> val ) values.push_back( val );
    
    TString formula_name = "";
    TString stringhetta;
    std::vector < TString > names_of_the_formulae;
    
    std::cout << " ********************* values size = " << values.size() << std::endl;
    
    for(int firstloop = 0; firstloop<number_of_base /**/; firstloop++){
	formula_1 = "(";
	formula_name = "";
	stringhetta = Form("%1i",firstloop);
	
	formula_name = "Valerio_" + stringhetta;
	
	names_of_the_formulae.push_back(formula_name);
	
	for(unsigned int secondloop = 0;secondloop< (( values.size()/number_of_base)) /**/; secondloop++){
	    if(secondloop==0) formula_1 += values[secondloop + (number_of_base)*firstloop];
	    if(secondloop!=0) formula_1 += " + " + values[secondloop + (number_of_base)*firstloop];		
	    
	}
		formula_1 +=")*"+total_width_function;	
	RooFormulaVar multi(names_of_the_formulae[firstloop],names_of_the_formulae[firstloop],formula_1,normList);
	w->import(multi);    
	formula_1 = "(";
	stringhetta = "";

    }


    stringstream editStr;
    
    cout << "********************************"<< endl;
    cout <<"mu_names[0] = "<< mu_names[0] << endl;
    cout <<"mu_names[1] = "<< mu_names[1] << endl;
    cout <<"mu_names[2] = "<< mu_names[2] << endl;
    cout <<"mu_names[3] = "<< mu_names[3] << endl;
    cout <<"mu_names[4] = "<< mu_names[4] << endl;
    cout << "********************************"<< endl;


    std::vector< std::string > values_ggf;
    string val_ggf;
    int value_I_WANT_ggf = 0;
    while( fileoutput_ggf >> val_ggf ) values_ggf.push_back( val_ggf );
    TString formula_name_ggf = "";
    TString stringhetta_ggf;
    std::vector < TString > names_of_the_formulae_ggf;
    std::cout << " ********************* values size = " << values_ggf.size() << std::endl;
    for(int firstloop_ggf = 0; firstloop_ggf<ggf_number_of_base /**/; firstloop_ggf++){
        formula_1_ggf = "(";
        formula_name_ggf = "";
        stringhetta_ggf = Form("%1i",firstloop_ggf);
        formula_name_ggf = "Valerio_ggf_" + stringhetta_ggf;
        names_of_the_formulae_ggf.push_back(formula_name_ggf);
        for(unsigned int secondloop_ggf = 0;secondloop_ggf< (( values_ggf.size()/ggf_number_of_base)) /**/; secondloop_ggf++){
            if(secondloop_ggf==0) formula_1_ggf += values_ggf[secondloop_ggf + (ggf_number_of_base)*firstloop_ggf];
            if(secondloop_ggf!=0) formula_1_ggf += " + " + values_ggf[secondloop_ggf + (ggf_number_of_base)*firstloop_ggf];
        }
	formula_1_ggf +=")*"+total_width_function;
        RooFormulaVar multi_ggf(names_of_the_formulae_ggf[firstloop_ggf],names_of_the_formulae_ggf[firstloop_ggf],formula_1_ggf,normList);
        w->import(multi_ggf);
        formula_1_ggf = "(";
        stringhetta_ggf = "";
    }
    cout << "********************************"<< endl;
    cout <<"mu_names_ggf[0] = "<< mu_names_ggf[0] << endl;
    cout <<"mu_names_ggf[1] = "<< mu_names_ggf[1] << endl;
    cout <<"mu_names_ggf[2] = "<< mu_names_ggf[2] << endl;
    cout << "********************************"<< endl;

//adding the ttH BR function
TString formula_exctracted_for_ttH = "9*9=81" ;

    if(coupling_name[1]=="khvv"){
        formula_exctracted_for_ttH = "@0*@0 + 0.001281*@1*@1 + 0.060305*@0*@1";
    }
    if(coupling_name[1]=="kavv"){
        formula_exctracted_for_ttH = "@0*@0 + 0.00052654431*@1*@1 - 0.00035524*@0*@1";
        }

RooFormulaVar t_t_H_function("t_t_H_function_Valerio", "t_t_H_function_Valerio", formula_exctracted_for_ttH ,normList);
w->import(t_t_H_function);

    for(unsigned int editing = 0 ; editing<names_of_the_formulae.size() ; editing++ ){
        if(editing == 0){
                editStr << "EDIT::" << pdf->GetName() << "_edit(" << pdf->GetName() << "," << mu_names[editing] << "=" << names_of_the_formulae[editing]<<",";
                for(unsigned int editing_ggf = 0 ; editing_ggf<names_of_the_formulae_ggf.size() ; editing_ggf++ ){
                        editStr << mu_names_ggf[editing_ggf] << "=" << names_of_the_formulae_ggf[editing_ggf]<<",";
                }
        }
        if(editing != 0 && editing != (names_of_the_formulae.size() -1 )) editStr << mu_names[editing] << "=" << names_of_the_formulae[editing]<<",";


        if(editing == (names_of_the_formulae.size() -1 ) )  editStr << mu_names[editing] << "="<< names_of_the_formulae[editing] << "," << "mu_tth_norm = t_t_H_function_Valerio" << ")";
    }

    w->factory(editStr.str().c_str());

   
 
    RooAbsPdf* yourNewPDF = w->pdf((string(pdf->GetName())+"_edit").c_str());
    
    RooRealVar* test_prova = NULL;
    test_prova = w->var(coupling_name[1]);
    
    RooRealVar* test_prova_sm = NULL;
    test_prova_sm = w->var(coupling_name[0]);

    RooRealVar* test_sm_weight = NULL;
	test_sm_weight = w->var("mu_vbf_BSM0");

    RooRealVar* test_bsm1_weight = NULL;
        test_bsm1_weight = w->var("mu_vbf_BSM1");

    RooRealVar* test_bsm2_weight = NULL;
        test_bsm2_weight = w->var("mu_vbf_BSM2");

    RooRealVar* test_bsm3_weight = NULL;
        test_bsm3_weight = w->var("mu_vbf_BSM3");

    RooRealVar* test_bsm4_weight = NULL;
        test_bsm4_weight = w->var("mu_vbf_BSM4");

//likelihood uncond
    int status_uncond = 0;
    float Likelihood_uncond = -999.;
    test_prova->setVal(0);
    test_prova->setConstant(false);
    test_prova_sm->setVal(1);
    test_prova_sm->setConstant(true);

    if(sm_fix)      test_prova_sm->setConstant(true);

    if(!sm_fix)     test_prova_sm->setConstant(false);

    std::cout << "***************************************************** UNCOND  *************************************************" << std::endl;
    RooAbsReal* nll_uncond = yourNewPDF->createNLL(*data/* *data */, RooFit::Constrain( *mc->GetNuisanceParameters() ), RooFit::GlobalObservables(*mc->GetGlobalObservables()), RooFit::Offset(false) );
    
    RooMinimizer uncond (*nll_uncond);
    uncond.setPrintLevel(0);
    uncond.setStrategy(0);
    status_uncond = uncond.minimize("Minuit2");
    Likelihood_uncond = nll_uncond->getVal();

    for(unsigned int j = 0; j<region.size() ; j++){
        total_value = 0.;
        for(unsigned int k = 0; k<sample.size() ; k++){
                if(sample[k].Contains("MG5") || sample[k].Contains("MG5") || sample[k] == "ttH"){
                        interesting = sample[k]+"_c_"+region[j]+"_overallSyst_x_Exp_simPdf_edit";
                }else{
                        interesting = sample[k]+"_c_"+region[j]+"_overallSyst_x_Exp";
                }

                andrea = (RooProduct*) w->obj(interesting);
                total_value = total_value + andrea->getVal();
                std::cout<< " In " << region[j] << " Sample " << sample[k] << " " << andrea->getVal() << std::endl;

        }
        std::cout<< "In region " << region[j] <<" the number of events postfit = " << total_value << std::endl;
     }

//        std::cout << yourNewPDF->expectedEvents(*mc->GetObservables()) << std::endl;
        std::cout << "VALERIO POSTFIT UNCOND WITH MIN AT " << test_prova->getVal()  << " OBSERVED = " << yourNewPDF->expectedEvents(*mc->GetObservables()) << std::endl;

    std::cout << "************************************************** END OF UNCOND  *************************************************" << std::endl;

   
    test_prova_sm->setVal(1);
    test_prova_sm->setConstant(true);



    RooRealVar* sistematica = NULL;

    float coupl_val = 0.;
    float Likelihood = -999.;

        Likelihood = -999.;

	test_prova->setVal(coupling_value_input);
        test_prova->setConstant(true);

            test_prova_sm->setVal(1);
            test_prova_sm->setConstant(true);

        if(sm_fix) test_prova_sm->setConstant(true);
        if(!sm_fix) test_prova_sm->setConstant(false);

        RooAbsReal* nll_cond = yourNewPDF->createNLL(*data, RooFit::Constrain( *mc->GetNuisanceParameters() ), RooFit::GlobalObservables(*mc->GetGlobalObservables()), RooFit::Offset(false) );
        RooMinimizer SM_HYPO (*nll_cond);
        SM_HYPO.setPrintLevel(0);
        SM_HYPO.setStrategy(0);
        fit_status_ALTHypo = SM_HYPO.minimize("Minuit2");
        Likelihood = nll_cond->getVal();
        m2DLL=2*(Likelihood - Likelihood_uncond);
//        std::cout<< "2DLL = " << m2DLL <<"   at a coupl_val " << test_prova->getVal() << std::endl;
        std::cout<< "*****************************************************************************************" << std::endl;
        std::cout<< "**************************** COND TO THE VALUE "<< test_prova->getVal() << " *************************" << std::endl;
        std::cout<< "2DLL = " << m2DLL <<"   at a coupl_val " << test_prova->getVal() << std::endl;

        for(unsigned int j = 0; j<region.size() ; j++){
                total_value = 0.;
                for(unsigned int k = 0; k<sample.size() ; k++){
                        if(sample[k].Contains("MG5") || sample[k].Contains("MG5") || sample[k]=="ttH"){
                                interesting = sample[k]+"_c_"+region[j]+"_overallSyst_x_Exp_simPdf_edit";
                        }else{
                                interesting = sample[k]+"_c_"+region[j]+"_overallSyst_x_Exp";
                        }

                        andrea = (RooProduct*) w->obj(interesting);
                        total_value = total_value + andrea->getVal();
                        std::cout<< " In " << region[j] << " Sample " << sample[k] << " " << andrea->getVal() << std::endl;
                }
        std::cout<< "In region " << region[j] <<" the number of events cond postfit = " << total_value << std::endl;
        }


//        std::cout << yourNewPDF->expectedEvents(*mc->GetObservables()) << std::endl;
        std::cout << "VALERIO POSTFIT COND TO " << test_prova->getVal() <<   " OBSERVED = "<< yourNewPDF->expectedEvents(*mc->GetObservables())   << std::endl;


        tree_scan->Fill();


}
