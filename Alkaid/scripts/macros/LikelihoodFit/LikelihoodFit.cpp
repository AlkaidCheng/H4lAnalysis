#include "LikelihoodFit.h"

//Defining virtual functions
void LikelihoodFitting::SetCouplBranch() {}
void LikelihoodFitting::SetRealVar() {}
void LikelihoodFitting::SetTotalWidthFunction() {}
void LikelihoodFitting::SetFormula_Exctracted_For_ttH() {}
void LikelihoodFitting::TestCouplingIni() {}
void LikelihoodFitting::TestCouplingReset(double SM, double BSM_1, bool Fix_BSM_1, double BSM_2, bool Fix_BSM_2) {}
void LikelihoodFitting::DoScan(const int &NumOfScanX, const double &widthX, const double &Xmin, const int &NumOfScanY, const double &widthY, const double &Ymin, const TString &OutputFilePrefix) {}


//Constructor for Initializing all variables for the NLL scan
LikelihoodFitting::LikelihoodFitting(const TString &SystErrInput, const TString &CouplingInput, const TString &ReadoutInput, const TString &ggfReadoutInput, const TString &wsInput, const TString &workspace, const TString &model, const TString &dataname, const bool &_SM_fix_, const bool & _total_width_fix_, const int &_NOB_, const int &_NOB_ggf_, const bool &_ggf_BR_modification_, const bool &_Asimov_, const bool &_total_mu_ggf_, const TString& _mode_) {
	this->Tree_Scan = new TTree("Likelihood", "test statistic");
	this->SM_fix = _SM_fix_;
	this->total_width_fix = _total_width_fix_;
	this->NOB = _NOB_;
	this->NOB_ggf = _NOB_ggf_;
	this->ggf_BR_modification = _ggf_BR_modification_;
	this->Asimov = _Asimov_;
	this->total_mu_ggf = _total_mu_ggf_;
	this->mode = _mode_;
	ParamIni();
	SetBranch();
	SetSystBranch(SystErrInput);
	SetCouplingName(CouplingInput);
	SetWorkSpace(wsInput, workspace, model, dataname);
	SetMuNuisance();
}

//Destructor to prevent memory leakage
LikelihoodFitting::~LikelihoodFitting() {
	delete this->Tree_Scan;
	delete this->f;
	delete this->ga;
	delete this->gb;
	if (this->mode == "ggf")
		delete this->gggf;
	delete this->normList;
}


LikelihoodFitting_2D::~LikelihoodFitting_2D() {
	delete this->gc;
}

//Initialization specific to the derived class: 1D / 2D 
void LikelihoodFitting::DerivedClassVariablesIni(const TString &ReadoutInput, const TString &ggfReadoutInput) {
	SetCouplBranch();
	SetRealVar();
	SetTotalWidthFunction();
	if (this->mode == "ggf")
		SetFormula(ReadoutInput, this->NOB, TString("").Format("@2*%s",this->TotalWidthFunction.Data()), "Valerio");
	else if (this->mode == "vv") {
		SetFormula_Exctracted_For_ttH();
		RooFormulaVar t_t_H_function("t_t_H_function_Valerio", "t_t_H_function_Valerio", this->Formula_Exctracted_For_ttH, *this->normList);
		w->import(t_t_H_function);
		SetFormula(ReadoutInput, this->NOB, this->TotalWidthFunction, "Valerio");
		SetFormula(ggfReadoutInput, this->NOB_ggf, this->TotalWidthFunction, "Valerio_ggf");
	}
	SetFactory();
	LLVarIni();
	TestCouplingIni();
	DoScan_Uncon();
}

void LikelihoodFitting::ParamIni() {
	this->SM_coupling = -999.;
	this->BSM_coupling1 = -999.;
	this->m2DLL = -999.;
	this->fit_status_ALTHypo = -999.;
	this->SM_weight = -999.;
	this->BSM1_weight = -999.;
	this->BSM2_weight = -999.;
	this->BSM3_weight = -999.;
	this->BSM4_weight = -999.;
	this->BR_SCALING = -999.;

	this->SM_weight_ggf = -999.;
	this->BSM1_weight_ggf = -999.;
	this->BSM2_weight_ggf = -999.;
	this->Likelihood_uncond = -999.;
	this->Systematics_Name.clear();
	this->Systematics_Value.clear();
	this->Couplings_Name.clear();
}

void LikelihoodFitting_1D::SetCouplBranch() {
	this->Tree_Scan->Branch("SM_coupling", &this->SM_coupling);
	this->Tree_Scan->Branch("BSM_coupling", &this->BSM_coupling1);
}

void LikelihoodFitting_2D::SetCouplBranch() {
	this->Tree_Scan->Branch("SM_coupling", &this->SM_coupling);
	this->Tree_Scan->Branch("BSM1_coupling", &this->BSM_coupling1);
	this->Tree_Scan->Branch("BSM2_coupling", &this->BSM_coupling2);
}

void LikelihoodFitting::SetBranch() {
	this->Tree_Scan->Branch("qmu", &this->m2DLL);
	this->Tree_Scan->Branch("fit_status", &this->fit_status_ALTHypo);
	this->Tree_Scan->Branch("SM_weight", &this->SM_weight);
	this->Tree_Scan->Branch("BSM1_weight", &this->BSM1_weight);
	this->Tree_Scan->Branch("BSM2_weight", &this->BSM2_weight);
	this->Tree_Scan->Branch("BSM3_weight", &this->BSM3_weight);
	this->Tree_Scan->Branch("BSM4_weight", &this->BSM4_weight);
	this->Tree_Scan->Branch("SM_weight_ggf", &this->SM_weight_ggf);
	this->Tree_Scan->Branch("BSM1_weight_ggf", &this->BSM1_weight_ggf);
	this->Tree_Scan->Branch("BSM2_weight_ggf", &this->BSM2_weight_ggf);
	this->Tree_Scan->Branch("BR_SCALING", &this->BR_SCALING);
}


void LikelihoodFitting::SetSystBranch(const TString &SystErrInput) {
	std::vector<TString> SystNames = ParseInputTextToVector(SystErrInput);
	if (SystNames.size() != 0) {
		this->Systematics_Value.clear();
		for (unsigned int i = 0; i < SystNames.size(); i++) {
			this->Systematics_Value.push_back(-999.);
			this->Tree_Scan->Branch("post_fit_" + SystNames[i], &this->Systematics_Value.back());
		}
	}
	else
		std::cout << "ERROR: Invalid Systematic Errors Input. No Systemmatic Errors branch can be set.\n";
}

void LikelihoodFitting::PrintCouplingName() {
	std::cout << "Total number of couplings: " << this->Couplings_Name.size() << std::endl;
	std::cout << "List of couplings: ";
	for (unsigned int i = 0; i < this->Couplings_Name.size(); i++)
		std::cout << this->Couplings_Name[i].Data() << "\t";
	std::cout << std::endl;
}

void LikelihoodFitting::SetCouplingName(const TString &Input) {
	this->Couplings_Name = ParseInputTextToVector(Input);
	if (this->Couplings_Name.size() == 0)
		std::cout << "ERROR: No coupling names received!Please check your couplings input\n";
	else
		PrintCouplingName();
}

void LikelihoodFitting::SetWorkSpace(const TString &Input, const TString &workspace, const TString &model, const TString &dataname) {

	this->f = new TFile(Input);
	this->w = (RooWorkspace*)f->Get(workspace);
	if (this->w) {
		this->mc = (ModelConfig*)w->obj(model);
		this->pdf = (RooAbsPdf*)mc->GetPdf();
		this->NP = (RooArgSet*)mc->GetGlobalObservables();
		this->pois = (RooArgSet*)mc->GetParametersOfInterest();
		this->data = (RooDataSet*)w->data(dataname);
		this->Lumi_to_fix = this->w->var("Lumi");
		this->Lumi_to_fix->setConstant(true);
	}
	else
		std::cout << "ERROR: workspace CANNOT be initialized.";

}

void LikelihoodFitting_1D::SetRealVar() {

	if (this->Couplings_Name.size() >= 2) {
		this->ga = new RooRealVar(this->Couplings_Name[0], this->Couplings_Name[0], 1);
		this->gb = new RooRealVar(this->Couplings_Name[1], this->Couplings_Name[1], 0., -50., 50.);
		this->ga->setConstant(true);
		this->gb->setConstant(true);
   
		if (this->mode == "ggf") {
			this->gggf = new RooRealVar("total_mu_ggf", "total_mu_ggf", 1, -50, 50);
			this->gggf->setConstant(!this->total_mu_ggf);
		}
		if (this->mode == "vv")
			this->normList = new RooArgList(*ga, *gb);
		else if (this->mode == "ggf")
			this->normList = new RooArgList(*ga, *gb,*gggf);
	}
	else
		std::cout << "ERROR: coupling names are not properly initialized. Cannot create RooRealVar ga and gb";
}

void LikelihoodFitting_2D::SetRealVar() {

	if (this->Couplings_Name.size() >= 3) {
		this->ga = new RooRealVar(this->Couplings_Name[0], this->Couplings_Name[0], 1);
		this->gb = new RooRealVar(this->Couplings_Name[1], this->Couplings_Name[1], 0., -50., 50.);
		this->gc = new RooRealVar(this->Couplings_Name[2], this->Couplings_Name[2], 0., -50., 50.);
		this->ga->setConstant(true);
		this->gb->setConstant(true);
		this->gc->setConstant(true);
		this->normList = new RooArgList(*ga, *gb, *gc);
	}
	else
		std::cout << "ERROR: coupling names are not properly initialized. Cannot create RooRealVar for the couplings!";
}

void LikelihoodFitting::SetNuisance(const TString &NuisSpec, std::vector < TString > *v, const bool &SetConst, const  double &SetVal) {
	TString str = "";
	RooRealVar* var;		//Pointer to the Nuisance Parameter
	TIterator *iter = this->mc->GetNuisanceParameters()->createIterator();
	while ((var = (RooRealVar*)iter->Next())) {
		str = var->GetName();
		if (IsMatch(NuisSpec, str)) {
			std::cout << " Nuisance Parameter: " << str << std::endl;
			if (!DoubleIsEqual(SetVal, -999.))		//If SetVal is equal to the defaulg value of -999. the Value fo the Nuisance Paramter will not be set
				var->setVal(SetVal);				//Set the value of the Nuisance Parameter to the input SetVal
			var->setConstant(SetConst);				//Fixing or Not Fixing the Nuisance Parameter 
			v->push_back(str);						//Store the name of the Nuisance Paramter
		}
	}
}

void LikelihoodFitting::SetMuNuisance() {
	if (this->mode == "vv"){
		SetNuisance("*bf_", &this->mu_names, true, 1.);
	  SetNuisance("*gf_", &this->mu_names_ggf, true, 1.);
  }
  else if (this->mode == "ggf")
    SetNuisance("*gf_", &this->mu_names, true, 1.);
}


void LikelihoodFitting_1D::SetTotalWidthFunction() {
	if (!this->total_width_fix)
		this->TotalWidthFunction = "1.";
	else if (!this->Couplings_Name[1].CompareTo("khvv"))
		this->TotalWidthFunction = FitInfo::TotalWidthFunction_khvv;
	else if (!this->Couplings_Name[1].CompareTo("kavv"))
		this->TotalWidthFunction = FitInfo::TotalWidthFunction_kavv;
	else if ((!this->Couplings_Name[1].CompareTo("kagg"))|| (!this->Couplings_Name[1].CompareTo("khgg")))
		this->TotalWidthFunction = FitInfo::TotalWidthFunction_khgg_kagg;
}

void LikelihoodFitting_2D::SetTotalWidthFunction() {
	if (!this->total_width_fix)
		this->TotalWidthFunction = "1.";
	else
		this->TotalWidthFunction = FitInfo::TotalWidthFunction_khvv_kavv;
}


void LikelihoodFitting_1D::SetFormula_Exctracted_For_ttH() {
	if (!this->Couplings_Name[1].CompareTo("khvv"))
		this->Formula_Exctracted_For_ttH = FitInfo::Formula_Exctracted_For_ttH_khvv;
	else if (!this->Couplings_Name[1].CompareTo("kavv"))
		this->Formula_Exctracted_For_ttH = FitInfo::Formula_Exctracted_For_ttH_kavv;
	//	this->Formula_Exctracted_For_ttH = FitInfo::Formula_Exctracted_For_ttH_List[std::distance(FitInfo::ParamForTotalWidthFunction.begin(), std::find(ParamForTotalWidthFunction.begin(), ParamForTotalWidthFunction.end(), this->Couplings_Name[1]))];
}

void LikelihoodFitting_2D::SetFormula_Exctracted_For_ttH() {
	this->Formula_Exctracted_For_ttH = FitInfo::Formula_Exctracted_For_ttH_khvv_kavv;
}

std::vector<TString> LikelihoodFitting::GetFormulaName(const TString prefix, const int &len) {
	std::vector<TString> result;
	for (int i = 0; i < len; i++)
		result.push_back(Form("%s_%1i", prefix.Data(), i));
	return result;
}


std::vector<TString> LikelihoodFitting::GetFormulaList(const std::vector<TString> &values, unsigned const int &NumberOfSection, const TString &Multiplier) {
	std::vector<TString> result;
	if (values.size() % NumberOfSection != 0)
		std::cout << "WARNING: Number of Values in the Formula NOT Divisible by the Number of Section.";
	unsigned int ElementsPerSection = (values.size() / NumberOfSection);
	for (unsigned int i = 0; i < NumberOfSection; i++) {
		TString buffer = "(";
		for (unsigned int j = 0; j < ElementsPerSection; j++) {
			TString value = values[i*NumberOfSection + j];
			buffer = (j == 0) ? (buffer + value) : (buffer + "+" + value);
		}
		buffer = buffer + ")*" + Multiplier;
		result.push_back(buffer);
	}
	return result;
}

void LikelihoodFitting::SetFormula(const TString &Input, const int & N, const TString &Multiplier, const TString &prefix) {
	std::vector<TString> Formula = GetFormulaList(ParseInputTextToVector(Input), N, Multiplier);
	std::vector<TString> FormulaName = GetFormulaName(prefix, Formula.size());
	for (unsigned int i = 0; i < Formula.size(); i++) {
		RooFormulaVar buffer = RooFormulaVar(FormulaName[i], FormulaName[i], Formula[i], *this->normList);
		std::cout << "Included Formula: " << FormulaName[i] << " " << Formula[i] << std::endl;
		w->import(buffer);
	}
}

TString LikelihoodFitting::GetFactoryString(const TString &name, const  std::vector< std::vector< std::vector<TString> > > &input) {
	TString buffer = "";
	for (unsigned int i = 0; i < input.size(); i++)
		if (input[i][0].size() == input[i][1].size())
			for (unsigned int j = 0; j < input[i][0].size(); j++)
				buffer += TString("").Format("%s=%s,", input[i][0][j].Data(), input[i][1][j].Data());
		else
			std::cout << "ERROR: Number of Parameter Name does NOT Equal Number of Parameter Value!";
	buffer.Remove(buffer.Sizeof() - 2);
	return TString("").Format("EDIT::%s_edit(%s,%s)", name.Data(), name.Data(), buffer.Data());
}

void LikelihoodFitting::SetFactory() {
	//w->factory(GetFactoryString(this->pdf->GetName(), { {this->mu_names, this->formula},{this->mu_names_ggf,this->formula_ggf}, { ScalarToVector(TString("mu_tth_norm")), ScalarToVector(TString("t_t_H_function_Valerio"))} }).Data());
	std::vector< std::vector< std::vector<TString> > > input;
	std::vector< std::vector<TString> > buffer;
	buffer.push_back(this->mu_names);
	buffer.push_back(GetFormulaName("Valerio", this->mu_names.size()));
	input.push_back(buffer);
	buffer.clear();
	if (this->mode == "vv") {
		buffer.push_back(this->mu_names_ggf);
		buffer.push_back(GetFormulaName("Valerio_ggf", this->mu_names_ggf.size()));
		input.push_back(buffer);
	  buffer.clear();
		buffer.push_back(ScalarToVector(TString("mu_tth_norm")));
		buffer.push_back(ScalarToVector(TString("t_t_H_function_Valerio")));
		input.push_back(buffer);
	}
	std::cout << GetFactoryString(this->pdf->GetName(), input).Data() << std::endl;
	w->factory(GetFactoryString(this->pdf->GetName(), input).Data());
}

void LikelihoodFitting_1D::TestCouplingIni() {
	this->test_prova_sm = this->w->var(this->Couplings_Name[0]);
	this->test_prova_bsm1 = this->w->var(this->Couplings_Name[1]);
}

void LikelihoodFitting_2D::TestCouplingIni() {
	this->test_prova_sm = this->w->var(this->Couplings_Name[0]);
	this->test_prova_bsm1 = this->w->var(this->Couplings_Name[1]);
	this->test_prova_bsm2 = this->w->var(this->Couplings_Name[2]);
}

void LikelihoodFitting::LLVarIni() {
	this->NewPDF = this->w->pdf(TString("").Format("%s_edit", this->pdf->GetName()).Data());
  if (this->mode == "vv"){
	  this->test_sm_weight = this->w->var("mu_vbf_BSM0");
	  this->test_bsm1_weight = this->w->var("mu_vbf_BSM1");
	  this->test_bsm2_weight = this->w->var("mu_vbf_BSM2");
	  this->test_bsm3_weight = this->w->var("mu_vbf_BSM3");
	  this->test_bsm4_weight = this->w->var("mu_vbf_BSM4");
   }
}

void LikelihoodFitting_1D::TestCouplingReset(double SM, double BSM_1, bool Fix_BSM_1, double BSM_2, bool Fix_BSM_2) {
	this->test_prova_bsm1->setVal(BSM_1);
	this->test_prova_bsm1->setConstant(Fix_BSM_1);
	this->test_prova_sm->setVal(SM);
	this->test_prova_sm->setConstant(this->SM_fix);
}

void LikelihoodFitting_2D::TestCouplingReset(double SM, double BSM_1, bool Fix_BSM_1, double BSM_2, bool Fix_BSM_2) {
	this->test_prova_bsm1->setVal(BSM_1);
	this->test_prova_bsm1->setConstant(Fix_BSM_1);
	this->test_prova_bsm2->setVal(BSM_2);
	this->test_prova_bsm2->setConstant(Fix_BSM_2);
	this->test_prova_sm->setVal(SM);
	this->test_prova_sm->setConstant(this->SM_fix);
}

void LikelihoodFitting::DoScan_Uncon() {
	int status_uncond = 0;
	TestCouplingReset();
	RooAbsReal* nll_uncond = NewPDF->createNLL(*this->data, RooFit::Constrain(*this->mc->GetNuisanceParameters()), RooFit::GlobalObservables(*this->mc->GetGlobalObservables()), RooFit::Offset(false));
	RooMinimizer uncond(*nll_uncond);
	uncond.setPrintLevel(0);
	uncond.setStrategy(0);
	status_uncond = uncond.minimize("Minuit2");
	this->Likelihood_uncond = nll_uncond->getVal();
}

void LikelihoodFitting_1D::DoScan(const int &NumOfScanX, const double &widthX, const double &Xmin, const int &NumOfScanY, const double &widthY, const double &Ymin, const TString &OutputFilePrefix) {
  TCanvas *c = new TCanvas("LLTest","LLTest");
	double Likelihood_scan[NumOfScanX];
	double coupling_value[NumOfScanX];

	for (int i = 0; i < NumOfScanX; i++) {
		RooRealVar* sistematica = NULL;
		double Likelihood = -999.;
		/*
		for (auto &syst:this->Systematics_Name) {
		sistematica = w->var("alpha_" + syst);
		sistematica->setVal(0.);
		}
		*/
		for (unsigned int j = 0; j < this->Systematics_Name.size(); j++) {
			sistematica = w->var("alpha_" + Systematics_Name[j]);
			sistematica->setVal(0.);
		}
		double coupl_val = Xmin + i*widthX;

		TestCouplingReset(1., coupl_val, true);

		RooAbsReal* nll_cond = this->NewPDF->createNLL(*this->data, RooFit::Constrain(*this->mc->GetNuisanceParameters()), RooFit::GlobalObservables(*this->mc->GetGlobalObservables()), RooFit::Offset(false));
		RooMinimizer SM_HYPO(*nll_cond);
    SM_HYPO.setVerbose(0);
		SM_HYPO.setPrintLevel(0);
		SM_HYPO.setStrategy(0);
		fit_status_ALTHypo = SM_HYPO.minimize("Minuit2");
		Likelihood = nll_cond->getVal();
		m2DLL = 2 * (Likelihood - this->Likelihood_uncond);
		coupling_value[i] = coupl_val;
		Likelihood_scan[i] = m2DLL;
		std::cout << "2DLL = " << m2DLL << "   at a coupl_val " << this->test_prova_bsm1->getVal() << std::endl;

		this->SM_coupling = this->test_prova_sm->getVal();
		this->BSM_coupling1 = this->test_prova_bsm1->getVal();

		for (unsigned int j = 0; j < Systematics_Value.size(); j++)
			Systematics_Value[i] = w->var("alpha_" + Systematics_Name[i])->getVal();
    
    if (this->mode == "vv") {
		  SM_weight = test_sm_weight->getVal();
		  BSM1_weight = test_bsm1_weight->getVal();
		  BSM2_weight = test_bsm2_weight->getVal();
		  BSM3_weight = test_bsm3_weight->getVal();
		  BSM4_weight = test_bsm4_weight->getVal();
    }
		this->Tree_Scan->Fill();
	}

	TString Modified[2] = { "NOT_modified","modified" };
  TString KSM[2] = { "KSM_floating","KSM_fix" };
  TString IsAsimov[2] = {"Observed","Asimov"};
  TString CouplingType = (this->mode == "vv") ? "KXvv": "KXgg";
	TString fname = TString("").Format("ggf_BR_%s_scan_%s_total_width_%s_%s_%s", Modified[this->ggf_BR_modification].Data(), this->Couplings_Name[1].Data(), Modified[this->total_width_fix].Data(),IsAsimov[this->Asimov].Data(), CouplingType.Data());

	TGraph * g_sign = new TGraph(NumOfScanX, coupling_value, Likelihood_scan);
	g_sign->GetXaxis()->SetTitle(this->Couplings_Name[1].ReplaceAll("h","H").ReplaceAll("a","A").ReplaceAll("k","#kappa ") );
	g_sign->GetYaxis()->SetTitle("-2ln(#lambda)");
  g_sign->Draw();
  c->SaveAs(Form("%s/%s_%s.pdf",OutputFilePrefix.Data(),KSM[this->SM_fix].Data(), fname.Data()));
	g_sign->SaveAs(Form("%s/%s_%s.eps",OutputFilePrefix.Data(),KSM[this->SM_fix].Data(), fname.Data()));
	
	TFile *fout = new TFile(Form("%s/%s_%s.root", OutputFilePrefix.Data(), KSM[this->SM_fix].Data(), fname.Data()), "RECREATE");
	g_sign->Write();
	this->Tree_Scan->Write();
	fout->Close();

	std::cout << "***************************  print the ws INFO  ****************************" << std::endl;
	this->w->Print();

	std::cout << "***************************  print the PDF INFO  ****************************" << std::endl;
	NewPDF->Print();
  
  delete c;
	delete g_sign;
	delete fout;
}

void LikelihoodFitting_2D::DoScan(const int &NumOfScanX, const double &widthX, const double &Xmin, const int &NumOfScanY, const double &widthY, const double &Ymin, const TString &OutputFilePrefix) {

	float xMIN = Xmin - (widthX / 2.);
	float xMAX = Xmin + widthX * NumOfScanX - (widthX / 2.);

	float yMIN = Ymin - (widthY / 2.);
	float yMAX = Ymin + widthY * NumOfScanY - (widthY / 2.);


	TH2F *h2f = new TH2F("h2f", "Observed Distribution", NumOfScanX, xMIN, xMAX, NumOfScanY, yMIN, yMAX);
	TH2F *h2f_KSM = new TH2F("h2f_KSM", "Observed KSM", NumOfScanX, xMIN, xMAX, NumOfScanY, yMIN, yMAX);
	TH1F *h1f_1 = new TH1F(Form("h1f_%s", this->Couplings_Name[1].Data()), Form("Distribution_%s", this->Couplings_Name[1].Data()), NumOfScanX, xMIN, xMAX);
	TH1F *h1f_2 = new TH1F(Form("h1f_%s", this->Couplings_Name[2].Data()), Form("Distribution_%s", this->Couplings_Name[2].Data()), NumOfScanY, yMIN, yMAX);


	for (int i = 0; i< NumOfScanX; i++) {

		RooRealVar* sistematica = NULL;
		double Likelihood = -999.;

		for (unsigned int k = 0; k < this->Systematics_Name.size(); k++) {
			sistematica = w->var("alpha_" + Systematics_Name[k]);
			sistematica->setVal(0.);
		}

		for (int j = 0; j < NumOfScanY; j++) {

			double coupl_val = Xmin + i*widthX;
			double coupl_val_2 = Ymin + j*widthY;

			TestCouplingReset(1., coupl_val, true, coupl_val_2, true);

			RooAbsReal* nll_cond = this->NewPDF->createNLL(*data, RooFit::Constrain(*mc->GetNuisanceParameters()), RooFit::GlobalObservables(*mc->GetGlobalObservables()), RooFit::Offset(false));
			RooMinimizer SM_HYPO(*nll_cond);
			SM_HYPO.setPrintLevel(0);
			SM_HYPO.setStrategy(0);
			fit_status_ALTHypo = SM_HYPO.minimize("Minuit2");
			Likelihood = nll_cond->getVal();
			m2DLL = 2 * (Likelihood - Likelihood_uncond);
			std::cout << "2DLL = " << m2DLL << "   at a coupl_val " << this->test_prova_bsm1->getVal() << "  and  coupl_2_val" << this->test_prova_bsm2->getVal() << std::endl;

			this->SM_coupling = this->test_prova_sm->getVal();
			this->BSM_coupling1 = this->test_prova_bsm1->getVal();
			this->BSM_coupling2 = this->test_prova_bsm2->getVal();


			for (unsigned int j = 0; j < Systematics_Value.size(); j++)
				Systematics_Value[i] = w->var("alpha_" + Systematics_Name[i])->getVal();


			SM_weight = test_sm_weight->getVal();
			BSM1_weight = test_bsm1_weight->getVal();
			BSM2_weight = test_bsm2_weight->getVal();
			BSM3_weight = test_bsm3_weight->getVal();
			BSM4_weight = test_bsm4_weight->getVal();


			h2f->Fill(this->BSM_coupling1, this->BSM_coupling2, m2DLL);
			h2f_KSM->Fill(this->BSM_coupling1, this->BSM_coupling2, this->SM_coupling);

			if (DoubleIsEqual(this->BSM_coupling2, 0)) h1f_1->Fill(this->BSM_coupling1, m2DLL);
			if (DoubleIsEqual(this->BSM_coupling1, 0)) h1f_2->Fill(this->BSM_coupling2, m2DLL);

			delete nll_cond;
			this->Tree_Scan->Fill();
		}//close the for for the first coupling
	}//close the for over coupling

	TString Modified[2] = { "NOT_modified","modified" };
	TString fname = TString("").Format("ggf_BR_%s_scan_%s_total_width_%s", Modified[this->ggf_BR_modification].Data(), this->Couplings_Name[1].Data(), Modified[this->total_width_fix].Data());

	TString KSM[2] = { "KSM_floating","KSM_fix" };
	TFile *fout = new TFile(Form("%s/%s_%s.root", OutputFilePrefix.Data(),KSM[this->SM_fix].Data(), fname.Data()), "RECREATE");

	this->Tree_Scan->Write();

	h2f_KSM->GetXaxis()->SetTitle(this->Couplings_Name[1]);
	h2f_KSM->GetYaxis()->SetTitle(this->Couplings_Name[2]);
	h2f->GetXaxis()->SetTitle(this->Couplings_Name[1]);
	h2f->GetYaxis()->SetTitle(this->Couplings_Name[2]);

	h1f_1->GetXaxis()->SetTitle(this->Couplings_Name[1]);
	h1f_1->GetYaxis()->SetTitle("-2DLL");

	h1f_2->GetXaxis()->SetTitle(this->Couplings_Name[2]);
	h1f_2->GetYaxis()->SetTitle("-2DLL");

	h1f_1->Write();
	h1f_2->Write();
	h2f_KSM->Write();
	h2f->Write();

	fout->Close();

	std::cout << "***************************  print the ws INFO  ****************************" << std::endl;
	this->w->Print();

	std::cout << "***************************  print the PDF INFO  ****************************" << std::endl;
	NewPDF->Print();

	delete h2f;
	delete h2f_KSM;
	delete h1f_1;
	delete h1f_2;
	delete fout;
}




/*
TString LikelihoodFitting::GetFactoryString(const TString &name,const  std::initializer_list< std::array<std::vector<TString>, 2>> &input){
TString result = "";
TString buffer = "";
for (auto &ss : input) {
if (ss[0].size() == ss[1].size())
for (int i = 0; i < ss[0].size(); i++) {
buffer += ss[0][i];
buffer += "+";
buffer += ss[1][i];
buffer += ",";
}
else
std::cout << "ERROR: Number of Parameter Name in Factory does NOT Equal Number of Parameter Value!";
}
return result.Format("EDIT::%s_edit(%s,%s)",name.Data(),name.Data(),buffer.Data());
}*/

/*
TString LikelihoodFitting::GetFactoryString(const TString &name, const  std::vector< std::vector< std::vector<TString> > > &input) {
TString result = "";
TString buffer = "";
for (auto &ss : input) {
if (ss[0].size() == ss[1].size())
for (int i = 0; i < ss[0].size(); i++) {
buffer += ss[0][i];
buffer += "+";
buffer += ss[1][i];
buffer += ",";
}
else
std::cout << "ERROR: Number of Parameter Name in Factory does NOT Equal Number of Parameter Value!";
}
return result.Format("EDIT::%s_edit(%s,%s)", name.Data(), name.Data(), buffer.Data());
}
*/







/*
void LikelihoodFitting::SetSystBranch(const std::vector<TString> &sys) {
this->Systematics_Value.clear();
for (auto &s : sys) {
this->Systematics_Value.push_back(-999.);
this->Tree_Scan->Branch("post_fit_" + s, &this->Systematics_Value.back());
}
}*/

/*
void LikelihoodFitting::SetSystBranch(const std::vector<TString> &sys) {
this->Systematics_Value.clear();
for (unsigned int i = 0; i < sys.size(); i++) {
this->Systematics_Value.push_back(-999.);
this->Tree_Scan->Branch("post_fit_" + sys[i], &this->Systematics_Value.back());
}
}
*/

