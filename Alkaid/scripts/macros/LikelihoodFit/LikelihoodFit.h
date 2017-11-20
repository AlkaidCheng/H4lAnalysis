#ifndef _LikelihoodFit_
#define _LikelihoodFit_
#pragma once
#include <vector>
#include "TString.h"
#include <iostream>
#include <fstream>
//#include <array>  //Problem with ROOT 5; Fixed in ROOT 6
#include "../General/Utilities.h"
#include "../General/Path.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TROOT.h"
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
#include "RooNLLVar.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooMinimizer.h"
#include "RooProfileLL.h"
#include "RooStats/RooStatsUtils.h"
using namespace RooFit;
using namespace RooStats;

const TString PATH_TO_TXT = APATH::BASE_PATH + "/All_Input/input_LLFit/";
const TString PATH_TO_WORKSPACE = APATH::BASE_PATH +"/All_Input/workspace/"; //"~/Valerio/workspaces/";//APATH::BASE_PATH +"/All_Input/workspace/";
const TString COUPLING_INPUT_1D = "coupling_name_khvv.txt";
const TString COUPLING_INPUT_2D = "coupling_name_khvv_kavv.txt";
const TString SYS_INPUT = "systematics_fake.txt";
const TString WORKSPACE_INPUT = "output_combined_datastat_model.root";
const TString READOUT_INPUT_1D = "function_khvv.txt";
const TString READOUT_INPUT_2D = "function_vbf_2D_KHVV_KAVV.txt";
const TString GGF_READOUT_INPUT_1D ="function_ggf_SM_1_p6_1_m6.txt";
const TString GGF_READOUT_INPUT_2D = "function_ggf_2D_KHVV_KAVV.txt";
const TString OUTPUT_PREFIX = APATH::BASE_PATH + "/output/";

namespace FitInfo {
TString TotalWidthFunction_khvv = "(1/( 0.76011 + 0.02619*(@0*@0 + 0.0012583*@1*@1 + 0.060434*@0*@1) + 0.2137*(@0*@0 + 0.002958494*@1*@1 + 0.091485142*@0*@1)))";
TString TotalWidthFunction_kavv = "(1/( 0.76011 + 0.02619*(@0*@0 + 0.0005457336*@1*@1 - 0.00002061966*@0*@1) + 0.2137*(@0*@0 + 0.001219426*@1*@1 - 0.000001918664*@0*@1)))";
TString TotalWidthFunction_khvv_kavv = "(1/( 0.76011 + 0.02619*(@0*@0 + 0.001287352*@1*@1 + 0.060486166*@0*@1 + 0.000525296*@2*@2 + 0.000043478*@0*@2 + 0.000489723*@1*@2) + 0.2137*(@0*@0 + 0.002932275*@1*@1 + 0.091639792*@0*@1 + 0.001219052*@2*@2 -0.000034731*@2*@0 -0.000002481*@1*@2)))";
TString Formula_Exctracted_For_ttH_khvv = "@0*@0 + 0.001281*@1*@1 + 0.060305*@0*@1";
TString Formula_Exctracted_For_ttH_kavv = "@0*@0 + 0.00052654431*@1*@1 - 0.00035524*@0*@1";
TString Formula_Exctracted_For_ttH_khvv_kavv = "(@0*@0 + 0.001287352*@1*@1 + 0.060486166*@0*@1 + 0.000525296*@2*@2 + 0.000043478*@0*@2 + 0.000489723*@1*@2)";
}

class LikelihoodFitting {
protected:
	bool SM_fix;
	bool total_width_fix;
	bool ggf_BR_modification;
  bool Asimov;
	int NOB;				//Number Of Base
	int NOB_ggf;			//Number Of Base ggf
	TString TotalWidthFunction;
	TString Formula_Exctracted_For_ttH;
	double SM_coupling;
	double BSM_coupling1;
	double m2DLL;
	double fit_status_ALTHypo;
	double SM_weight;
	double BSM1_weight;
	double BSM2_weight;
	double BSM3_weight;
	double BSM4_weight;
	double BR_SCALING;

	double SM_weight_ggf;
	double BSM1_weight_ggf;
	double BSM2_weight_ggf;
	std::vector<double> Systematics_Value;
	std::vector<TString> Systematics_Name;
	std::vector<TString> Couplings_Name;
	std::vector<TString> mu_names;
	std::vector<TString> mu_names_ggf;

	TFile* f;
	RooWorkspace* w;
	ModelConfig* mc;
	RooAbsPdf* pdf;
	RooArgSet* NP;
	RooArgSet* pois;
	RooDataSet* data;

	RooRealVar* Lumi_to_fix;
	RooRealVar* nuis_SM_nll;
	RooRealVar* nuis_SM_nll_ggf;
	RooRealVar* ga;
	RooRealVar* gb;
	RooArgList* normList;

	RooAbsPdf* NewPDF;
	RooRealVar* test_prova_bsm1;
	RooRealVar* test_prova_sm;
	RooRealVar* test_sm_weight;
	RooRealVar* test_bsm1_weight;
	RooRealVar* test_bsm2_weight;
	RooRealVar* test_bsm3_weight;
	RooRealVar* test_bsm4_weight;
	double Likelihood_uncond;

	TTree* Tree_Scan;

	void ParamIni();
	void SetBranch();
	virtual void SetCouplBranch();
	void SetSystBranch(const TString &Input = PATH_TO_TXT + "systematics_fake.txt");
	void SetCouplingName(const TString &Input = PATH_TO_TXT + COUPLING_INPUT_1D);
	void SetWorkSpace(const TString &Input = PATH_TO_WORKSPACE + "output_combined_datastat_model.root", const TString &workspace = "combined", const TString &model = "ModelConfig", const TString &dataname = "obsData");
	virtual void SetRealVar();
	void SetNuisance(const TString &NuisSpec, std::vector < TString > *v, const bool &SetConst = false, const double &SetVal = -999.);
	void SetMuNuisance();
	virtual void SetTotalWidthFunction();
	virtual void SetFormula_Exctracted_For_ttH();
	std::vector<TString> GetFormulaName(const TString prefix, const int &len);
	std::vector<TString> GetFormulaList(const std::vector<TString> &values, unsigned const int &NumberOfSection, const TString &Multiplier = "1");
	void SetFormula(const TString &Input, const int & N, const TString &Multiplier, const TString &prefix = "Valerio");
	//TString GetFactoryString(const TString &name, const  std::initializer_list< std::array<std::vector<TString>, 2>> &input); //Problem with ROOT 5; Fixed in ROOT 6
	TString GetFactoryString(const TString &name, const  std::vector< std::vector<std::vector<TString> > > &input);
	void SetFactory();
	void PrintCouplingName();
	void LLVarIni();
	virtual void TestCouplingIni();
	virtual void TestCouplingReset(double SM = 1., double BSM_1 = 0., bool Fix_BSM_1 = false, double BSM_2 = 0., bool Fix_BSM_2 = false);
	void DerivedClassVariablesIni(const TString &ReadoutInput = PATH_TO_TXT + "function_khvv.txt", const TString &ggfReadoutInput = PATH_TO_TXT + "function_ggf_SM_1_p6_1_m6.txt");
	void DoScan_Uncon();
public:
	LikelihoodFitting(const TString &SystErrInput = PATH_TO_TXT + SYS_INPUT, const TString &CouplingInput = PATH_TO_TXT + COUPLING_INPUT_1D, const TString &ReadoutInput = PATH_TO_TXT + READOUT_INPUT_1D, const TString &ggfReadoutInput = PATH_TO_TXT + GGF_READOUT_INPUT_1D, const TString &wsInput = PATH_TO_TXT + WORKSPACE_INPUT, const TString &workspace = "combined", const TString &model = "ModelConfig", const TString &dataname = "obsData", const bool &_SM_fix_ = true, const bool & _total_width_fix_ = true, const int &_NOB_ = 5, const int &_NOB_ggf_ = 3, const bool &_ggf_BR_modification_ = true, const bool &_Asimov_ = true);
	~LikelihoodFitting();
	virtual void DoScan(const int &NumOfScanX = 120, const double &widthX = 0.1, const double &Xmin = -8., const int &NumOfScanY = 120, const double &widthY = 0.1, const double &Ymin = -8., const TString &OutputFilePrefix = OUTPUT_PREFIX);
};


class LikelihoodFitting_1D : public LikelihoodFitting {
private:
	void SetCouplBranch() ;
	void SetRealVar() ;
	void SetTotalWidthFunction() ;
	void SetFormula_Exctracted_For_ttH() ;
	void TestCouplingIni() ;
	void TestCouplingReset(double SM = 1., double BSM_1 = 0., bool Fix_BSM_1 = false, double BSM_2 = 0., bool Fix_BSM_2 = false) ;
public:
	LikelihoodFitting_1D(const TString &SystErrInput = PATH_TO_TXT + SYS_INPUT, const TString &CouplingInput = PATH_TO_TXT + COUPLING_INPUT_1D, const TString &ReadoutInput = PATH_TO_TXT + READOUT_INPUT_1D, const TString &ggfReadoutInput = PATH_TO_TXT + GGF_READOUT_INPUT_1D, const TString &wsInput = PATH_TO_WORKSPACE + WORKSPACE_INPUT, const TString &workspace = "combined", const TString &model = "ModelConfig", const TString &dataname = "obsData", const bool &_SM_fix_ = true, const bool & _total_width_fix_ = true, const int &_NOB_ = 5, const int &_NOB_ggf_ = 3, const bool &_ggf_BR_modification_ = true, const bool &_Asimov_ = true):
		LikelihoodFitting(SystErrInput, CouplingInput, ReadoutInput, ggfReadoutInput, wsInput, workspace, model, dataname, _SM_fix_, _total_width_fix_, _NOB_, _NOB_ggf_, _ggf_BR_modification_, _Asimov_)
	{
		DerivedClassVariablesIni(ReadoutInput, ggfReadoutInput);
		DoScan_Uncon();
	}
	void DoScan(const int &NumOfScanX = 120, const double &widthX = 0.1, const double &Xmin = -8, const int &NumOfScanY = 0, const double &widthY = 0., const double &Ymin = 0., const TString &OutputFilePrefix = OUTPUT_PREFIX) ;
};

class LikelihoodFitting_2D : public LikelihoodFitting {
private:
	
	double BSM_coupling2;
	RooRealVar* gc;
	RooRealVar* test_prova_bsm2;
	void SetCouplBranch() ;
	void SetRealVar() ;
	void SetTotalWidthFunction() ;
	void SetFormula_Exctracted_For_ttH() ;
	void TestCouplingIni() ;
	void TestCouplingReset(double SM = 1., double BSM_1 = 0., bool Fix_BSM_1 = false, double BSM_2 = 0., bool Fix_BSM_2 = false) ;

public:
	LikelihoodFitting_2D(const TString &SystErrInput = PATH_TO_TXT + SYS_INPUT, const TString &CouplingInput = PATH_TO_TXT + COUPLING_INPUT_2D, const TString &ReadoutInput = PATH_TO_TXT + READOUT_INPUT_2D, const TString &ggfReadoutInput = PATH_TO_TXT + GGF_READOUT_INPUT_1D, const TString &wsInput = PATH_TO_WORKSPACE + WORKSPACE_INPUT, const TString &workspace = "combined", const TString &model = "ModelConfig", const TString &dataname = "obsData", const bool &_SM_fix_ = true, const bool & _total_width_fix_ = true, const int &_NOB_ = 15, const int &_NOB_ggf_ = 6, const bool &_ggf_BR_modification_ = true, const bool &_Asimov_ = true):
		LikelihoodFitting(SystErrInput, CouplingInput, ReadoutInput, ggfReadoutInput, wsInput, workspace, model, dataname, _SM_fix_, _total_width_fix_, _NOB_, _NOB_ggf_, _ggf_BR_modification_, _Asimov_) 
	{
		DerivedClassVariablesIni(ReadoutInput, ggfReadoutInput);
		DoScan_Uncon();
	}
	~LikelihoodFitting_2D();
	void DoScan(const int &NumOfScanX = 41, const double &widthX = 0.25, const double &Xmin = -5., const int &NumOfScanY = 41, const double &widthY = 0.25, const double &Ymin = -5., const TString &OutputFilePrefix = OUTPUT_PREFIX) ;
};

#endif
