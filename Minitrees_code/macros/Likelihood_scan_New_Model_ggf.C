// AUTHOR: VALERIO BORTOLOTTO
// date : 3 October 2016
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


void Likelihood_scan_New_Model_ggf(TString wsInput = "output_combined_datastat_model.root" , TString workspace="combined" , TString model="ModelConfig" , TString dataName="obsData" /*"obsData" or "asimovData"*/, float coupling_1 = 1, float coupling_2 = 2 , TString formula_1 ="@1 + @0", TString file_readout = "function.txt", int number_of_base = 3, TString coupling_name_file = "coupling_name.txt", bool total_width_fix = true, bool ggf_BR_modification = true, const float inizio = -8, const float larghezza = 0.1, const int passi = 160 , bool total_mu_ggf = false, TString syst_names_file = "macros/systematics.txt" , bool fix_sm = true ){

    TString total_width_function = "";

//    TString syst_names_file = "macros/systematics.txt";
    std::ifstream filesyst;
    filesyst.open (syst_names_file, std::ifstream::in);
    if(!filesyst.is_open()) std::cout<<"Did not manage to open file:"<< filesyst <<std::endl;
    std::vector< TString > systematics;
    TString syst_nam;
    while( filesyst >> syst_nam ) systematics.push_back( syst_nam );
//pushing also the theo syst:
/*
    systematics.push_back("theo_syst_EFT_VBF");
    systematics.push_back("theo_syst_categor_powheg");
    systematics.push_back("ioannis_error_elec");
    systematics.push_back("ioannis_error_muons");
    systematics.push_back("ggf_qcd_scale");
    systematics.push_back("ggf_pdf_alph_s");
    systematics.push_back("VBF_qcd_scale");
    systematics.push_back("VBF_pdf_alph_s");
    systematics.push_back("ZZqq_uncert_from_Nan_QCD");
    systematics.push_back("ZZqq_uncert_from_Nan_pdf");
    systematics.push_back("ZZgg_uncert_from_Nan_qcd");
    systematics.push_back("luminosity_uncert");
    systematics.push_back("ggf_ST");
*/

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
    float syst_ATLAS_QCDscale_ggH_ptH_m01 = -999.;
    float syst_ATLAS_QCDscale_ggH_ptH_m12 = -999.;


    TH2F *h2f = new TH2F("h2f","Observed Distribution",12,-3,3,11,-3,3);
    std::cout<<"please keep an eye on the coupling order same + prod + dec" << std::endl;

    TTree* tree_scan = new TTree("Likelihood","test statistic");
    tree_scan->Branch("qmu",&m2DLL);
    tree_scan->Branch("fit_status",&fit_status_ALTHypo);
    tree_scan->Branch("SM_coupling",&SM_value);
    tree_scan->Branch("BSM_coupling",&BSM_value);



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
	
        if(nuisance_string_nll.Contains("ggH_B")){
	    cout << " nuisance string  = " << nuisance_string_nll << endl;
	    nuis_SM_nll->setVal(1);
	    nuis_SM_nll->setConstant(true);
            mu_names.push_back(nuis_SM_nll->GetName());
	}
    }
   
  int num_np = 0;
  string varName;
  string varName_postfit;
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

//    RooRealVar* gvbf_vh = new RooRealVar ("total_mu_vbf_vh","total_mu_vbf_vh",1,-50,50);
    RooRealVar* gggf = new RooRealVar ("total_mu_ggf","total_mu_ggf",1,-50,50);
//    gvbf_vh->setConstant(true);
    if(!total_mu_ggf)  gggf->setConstant(true);
    if(total_mu_ggf) gggf->setConstant(false);

    ga->setConstant(true);
    gb->setConstant(true);

    RooArgList normList(*ga,*gb,*gggf);

/* ADDING HERE INFORMATION FOR THE TOTAL WIDTH */
    //if(!total_width_fix) total_width_function = "1.";
    // total_width_function = "1.";
//    total_width_function = "(1/1/( 0.9143 + 0.0857*(1*@0*@0+ 2.250249501*@1*@1 + 0.000748503*@0*@1)))" ;
/* I realized there were too many 1/... */
// OLD
//total_width_function = "(1/( 0.9143 + 0.0857*(1*@0*@0+ 2.250249501*@1*@1 + 0.000748503*@0*@1)))" ;
//New

	total_width_function = "(1/( 0.9143 + 0.0857*(1*@0*@0+ 2.24989*@1*@1)))" ;
//        total_width_function = "(1/( 0.76011 + 0.02619*(@0*@0 + 0.001281*@1*@1 + 0.060305*@0*@1) + 0.2137*(@0*@0 + 0.002955625*@1*@1 + 0.089193*@0*@1)))";
//
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
	if(total_width_fix){
		formula_1 +=")*@2*"+total_width_function;	
	}else{
		formula_1 +=")*@2";
	}
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
    cout << "********************************"<< endl;
    
    for(unsigned int editing = 0 ; editing<names_of_the_formulae.size() ; editing++ ){
        if(editing == 0) editStr << "EDIT::" << pdf->GetName() << "_edit(" << pdf->GetName() <<","<< mu_names[editing] << "=" << names_of_the_formulae[editing]<<",";

	if(editing != 0 && editing != (names_of_the_formulae.size() -1 )) editStr << mu_names[editing] << "=" << names_of_the_formulae[editing]<<",";	
	if(editing == (names_of_the_formulae.size() -1 ) )  editStr << mu_names[editing] << "="<< names_of_the_formulae[editing] << ")";
    } 
    
    w->factory(editStr.str().c_str());
    
    RooAbsPdf* yourNewPDF = w->pdf((string(pdf->GetName())+"_edit").c_str());
    
    
    RooRealVar* test_prova = NULL;
    test_prova = w->var(coupling_name[1]);
    
    RooRealVar* test_prova_sm = NULL;
    test_prova_sm = w->var(coupling_name[0]);

//likelihood uncond
    int status_uncond = 0;
    float Likelihood_uncond = -999.;
    //test_prova->setVal(-22.1);
    test_prova->setVal(0);
    test_prova->setConstant(false);
    test_prova_sm->setVal(1);
    test_prova_sm->setConstant(true);

    RooAbsReal* nll_uncond = yourNewPDF->createNLL(*data/* *data */, RooFit::Constrain( *mc->GetNuisanceParameters() ), RooFit::GlobalObservables(*mc->GetGlobalObservables()), RooFit::Offset(false) );
    
    RooMinimizer uncond (*nll_uncond);
    uncond.setPrintLevel(0);
    uncond.setStrategy(0);
    status_uncond = uncond.minimize("Minuit2");
    Likelihood_uncond = nll_uncond->getVal();
    
    test_prova_sm->setVal(1);
    test_prova_sm->setConstant(true);


    RooRealVar* sistematica = NULL;
    
    float Likelihood_scan[passi];
    float coupling_value[passi];
    float scan_point = passi;
    float width = larghezza;
    float starting_point = inizio;
    float coupl_val = 0.;
    float Likelihood = -999.;
    for(int i = 0 ; i< scan_point ; i++){
            for(int in_post_pre  = 0; in_post_pre < systN ; in_post_pre++ ){
                sistematica = w->var("alpha_"+systematics[in_post_pre]);
                sistematica->setVal(0.);
            }

	Likelihood = -999.;
	coupl_val = starting_point + i*width;
	test_prova->setVal(coupl_val);
	test_prova->setConstant(true);

        test_prova_sm->setVal(1);

	if(fix_sm) test_prova_sm->setConstant(true);
        if(!fix_sm) test_prova_sm->setConstant(false);


	RooAbsReal* nll_cond = yourNewPDF->createNLL(*data, RooFit::Constrain( *mc->GetNuisanceParameters() ), RooFit::GlobalObservables(*mc->GetGlobalObservables()), RooFit::Offset(false) );
	RooMinimizer SM_HYPO (*nll_cond);
	SM_HYPO.setPrintLevel(0);
	SM_HYPO.setStrategy(0);
	fit_status_ALTHypo = SM_HYPO.minimize("Minuit2");
/*
*/
	Likelihood = nll_cond->getVal();	
	m2DLL=2*(Likelihood - Likelihood_uncond);
	coupling_value[i]=coupl_val;
	Likelihood_scan[i]=m2DLL;
	std::cout<< "2DLL = " << m2DLL <<"   at a coupl_val " << test_prova->getVal() << std::endl;

        SM_value = test_prova_sm->getVal();
        BSM_value = test_prova->getVal();
/*
*/
	tree_scan->Fill();
    }//close the for over coupling

// bool total_width_fix = true, bool ggf_BR_modification = true

    TString file_name_string = "";    
    if(ggf_BR_modification && total_width_fix) file_name_string = "ggf_BR_modified_scan_" + coupling_name[1] + "_total_width_modified_";
    if(!ggf_BR_modification && total_width_fix) file_name_string = "ggf_BR_NOT_modified_scan_" + coupling_name[1] + "_total_width_modified_";
    if(ggf_BR_modification && !total_width_fix) file_name_string = "ggf_BR_modified_scan_" + coupling_name[1] + "_total_width_NOT_modified_";
    if(!ggf_BR_modification && !total_width_fix) file_name_string = "ggf_BR_NOT_modified_scan_" + coupling_name[1] + "_total_width_NOT_modified_";
    file_name_string = "ggf_Khgg_" + coupling_name[1] + "_only_";

        if(fix_sm) file_name_string = "ggf_Khgg_fix_" + coupling_name[1] + "_only_";
        if(!fix_sm) file_name_string = "ggf_Khgg_float_" + coupling_name[1] + "_only_";

    
    TGraph * g_sign = new TGraph(scan_point,coupling_value,Likelihood_scan);
    g_sign->SetTitle("Likelihood Scan");
    g_sign->GetXaxis()->SetTitle("KAgg");
    //g_sign->GetXaxis()->SetTitleSize(0.1);
    g_sign->GetYaxis()->SetTitle("-2DLL");
    //g_sign->GetYaxis()->SetTitleSize(0.1);
    g_sign->SaveAs("output/"+file_name_string+".eps");
    
    TFile *fout = new TFile("output/"+file_name_string+".root", "RECREATE");
    g_sign->Write();
    tree_scan->Write();
    fout->Close();

    cout << "***************************  print the ws INFO  ****************************" << endl;
    w->Print();
    cout << "*************************** end print the ws INFO  ****************************" << endl; 

    cout << "***************************  print the PDF INFO  ****************************" << endl;
    yourNewPDF->Print();
    cout << "***************************  print the PDF INFO  ****************************" << endl;

    cout << " editStr.str().c_str()" <<  editStr.str().c_str() << endl;
}
