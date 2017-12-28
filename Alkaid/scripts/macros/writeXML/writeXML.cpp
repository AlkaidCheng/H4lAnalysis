#include "writeXML.h"
//Matching Systematics Information

TString H4l_XML::SysMatch(TString sample, TString category, std::vector<SamStruct> vec, TString basename) {
	TString result = "";
	TString buffer = "";
	for (auto &v : vec)
		if (IsMatch(v.name, sample))
			for (auto &c : v.condition)
				if (IsMatch(c.name, category))
					for (auto &i : c.content)
						result += "    <" + basename + " =\"" + i.name + "\" High=\"" + buffer.Format("%f", i.high) + "\" Low=\"" + buffer.Format("%f", i.low) + "\"" +
						((i.val != 0) ? buffer.Format(" Val=\"%f\"", i.val) : "") + ((i.constant == true) ? " Const=\"True\"" : "") + "/>\n";
	return result;
}


int H4l_XML::writeXML(TString ErrorType, TString sample_in, TString category_in, TString norm_in, TString syst_in, TString jetptcut, TString discriminant, TString outpath, const bool &IsAsimov, TString mode) {
	if (ErrorType.Contains("EXP"))
		std::cout << "ADDING EXPERIMENTAL SYSTEMATIC ERRORS" << std::endl;
	if (ErrorType.Contains("STAT"))
		std::cout << "ADDING STATISTICAL ERRORS" << std::endl;
	if (ErrorType.Contains("THEO"))
		std::cout << "ADDING THEORETICAL SYSTEMATIC ERRORS" << std::endl;

	//Load the normalization, category, sample and systematic error files
	std::vector<TString> Norm_Factor = ParseInputTextToVector(norm_in);  //Discarded for the moment
	std::vector<TString> Sample = ParseInputTextToVector(sample_in);
	std::vector<TString> Syst = ParseInputTextToVector(syst_in);
	std::vector<TString> Category = ParseInputTextToVector(category_in);

	//RSysCmd::rmrec(TString("").Format("%s/category_*.xml",outpath.Data()));
	RSysCmd::mkdir(TString("").Format("%s/%s", outpath.Data(), discriminant.Data()));

	for (auto &category : Category) {
		int ggf_Base_BSM = 0;
		int vbf_Base_BSM = 0;
		//Prepare for output xml file
		TString buffer = TString("").Format("%s/%s/category_%s.xml", outpath.Data(), discriminant.Data(), category.Data());
		std::ofstream outfile(buffer, std::ofstream::out);
		std::cout << "Preparing xml file:" << buffer << std::endl;
		//Write Standard Format into xml
		outfile << "<!DOCTYPE Channel SYSTEM '" << XML_PATH.Data() << "HistFactorySchema.dtd'>\n\n" << buffer.Format("<Channel Name=\"c_%s\" HistoName=\"\">\n", category.Data());
		outfile << buffer.Format("  <Data  InputFile= \"%s/%s/%s/output_category_%s_%s_nominal.root\" HistoName=\"%s_%s\" HistoPath = \"\"/>\n", DATA_PATH.Data(), discriminant.Data(), category.Data(), category.Data(), DATA_VERSION[IsAsimov].Data(), DATA_VERSION[IsAsimov].Data(), discriminant.Data());
		outfile << " <StatErrorConfig RelErrorThreshold= \"0.\" ConstraintType=\"Poisson\" />\n\n";

		for (auto &sample : Sample) {

			if ((mode == "vv")&&((category == "VHlep" || category == "ttH" || category == "2jet_VBF_highptjet") && (sample.Contains("gfMG") && sample != "ggfMG5_0p")))
				continue;
			TString FileName = TString("").Format("%s/%s/%s/output_category_%s_%s_nominal.root", DATA_PATH.Data(), discriminant.Data(), category.Data(), category.Data(), sample.Data());
			TString HistName = buffer.Format("%s_%s", sample.Data(), discriminant.Data());
			std::cout << "Processing sample:" << sample << std::endl;
			std::cout << "File name: " << FileName.Data() << "\n";
			TFile *RootInput = new TFile(FileName);
			if (!RootInput) {
				std::cout << "ERROR: Unable to open file " << FileName << std::endl;
				continue;
			}
			TH1F *h_signal = (TH1F*)RootInput->Get(HistName);
			if (!h_signal) {
				std::cout << "ERROR: Unable to find histogram " << HistName << std::endl;
				continue;
			}
			double signal_norm = h_signal->Integral();
			RootInput->Close();
			outfile << buffer.Format("  <Sample Name=\"%s\"", sample.Data()) << buffer.Format(" InputFile= \"%s\"", FileName.Data()) << buffer.Format(" HistoPath=\"\" HistoName=\"%s\"", HistName.Data()) << " NormalizeByTheory=\"True\">" << std::endl;
			outfile << buffer.Format("    <StatError Activate=\"%s\"/> ", (ErrorType.Contains("STAT")) ? "True" : "False") << std::endl;

/*************************************************************************************************************************************************************************************************/
/*************************************************************************************************************************************************************************************************/
			if ((mode == "vv") || ((mode == "ggf") && (!(sample == "Redu_Muons" || sample == "Redu_Elec" || sample == "Redu_Bkg")))) {
				for (auto &syst : Syst) {
					buffer = TString("").Format("%s/%s/output_category_%s_%s_%s__1", DATA_PATH.Data(), category.Data(), category.Data(), sample.Data(), syst.Data());
					TFile *f_up = new TFile(buffer + "up.root");
					TFile *f_down;
					if (syst == "JET_JER_SINGLE_NP")
						f_down = new TFile(buffer + "up.root");
					else
						f_down = new TFile(buffer + "down.root");
					if ((!f_up) || (!f_down)) {
						std::cout << "ERROR: Unable to open file " << buffer << "up.root or " << buffer << "down.root\n";
						continue;
					}

					TH1F *h_up = (TH1F*)f_up->Get(HistName);
					TH1F *h_down = (TH1F*)f_down->Get(HistName);
					if ((!h_up) || (!h_down)) {
						std::cout << "ERROR: Unable to find histogram " << HistName << std::endl;
						continue;
					}

					double N_up = h_up->Integral() / signal_norm;
					double N_down = h_down->Integral() / signal_norm;

					f_up->Close();
					f_down->Close();
					if (syst == "JET_JER_SINGLE_NP") {
						if (N_up > 0.99999 && N_up < 1.000001 && N_down <= 1.0) N_up = 1.00001;
						if (N_up > 0.99999 && N_up<1.000001 && N_down>1.0) N_up = 0.99999;
						if (N_down > 0.9999 && N_down < 1.00001 && N_up <= 1.0) N_down = 1.00001;
						if (N_down > 0.9999 && N_down<1.00001 && N_up>1.0) N_down = 0.99999;
					}

					if ((syst == "weight_var_JvtEfficiency") || ((!((N_up) < 1.005 && (N_up) > 0.995) || !((N_down) < 1.005 && (N_down) > 0.995)) && (signal_norm > 0.0001 && ((N_up - 1.)*(N_down - 1.)) < 0)))
						outfile << "    <OverallSys Name=\"" << syst << "\" High= \"" << N_up << "\" Low=\"" << N_down << "\"/>" << std::endl;

				}  //End of Syst
			}
/*************************************************************************************************************************************************************************************************/
/*************************************************************************************************************************************************************************************************/
			//Adding Normalization Factor
      TString NormFactor;
			if (mode=="vv")
				NormFactor = SysMatch(sample, category, NormFactor_vector_vv, "NormFactor Name");
			if (mode=="ggf")
				NormFactor = SysMatch(sample, category, NormFactor_vector_ggf, "NormFactor Name");
			if (NormFactor.Contains("vbf_BASE_BSM")) {
				NormFactor.ReplaceAll("BASE_BSM", "BSM" + std::to_string(vbf_Base_BSM));
				vbf_Base_BSM++;
			}
			else if (NormFactor.Contains("ggf_BASE_BSM")) {
				NormFactor.ReplaceAll("BASE_BSM", "BSM" + std::to_string(ggf_Base_BSM));
				ggf_Base_BSM++;
			}
			outfile << NormFactor;
			//Adding Theoretical Systematic Errors
			if (ErrorType.Contains("THEO"))
				outfile << SysMatch(sample, category, TheorySys_vector, "OverallSys Name");
			//Adding Experimental Errors
			if (ErrorType.Contains("EXP"))
				outfile << SysMatch(sample, category, AddSys_RedSys_vector, "OverallSys Name");
			if (ErrorType.Contains("EXP") && ErrorType.Contains("THEO"))
				outfile << SysMatch(sample, category, AddSys_TheorySys_vector, "OverallSys Name");
			outfile << "  </Sample>\n\n";
		}//End of Sample

		outfile << "</Channel>" << std::endl;
		outfile.close();
	}//End of Category
	return -1;
}

int H4l_XML::writeDriver(TString dtd_in, TString workspace_out, TString category_in, TString outpath, TString discriminant) {
	std::vector<TString> Category = ParseInputTextToVector(category_in);
	TString fname = TString("").Format("%s/%s/driver.xml", outpath.Data(), discriminant.Data());
	std::ofstream outfile(fname.Data(), std::ofstream::out);
	outfile << "<!DOCTYPE Combination  SYSTEM \"" << dtd_in.Data() << "\">\n\n";
	outfile << "<Combination OutputFilePrefix=\"" << workspace_out.Data() << "/" << discriminant.Data() << "/output" << "\" >\n\n";
	for (auto &category : Category)
		outfile << "  <Input>" << outpath.Data() << "/" << discriminant.Data() << "/category_" << category.Data() << ".xml</Input>\n";
	outfile << "\n" << "  <Measurement Name=\"datastat\" Lumi=\"1\" LumiRelErr=\"0.0001\" ExportOnly=\"True\">\n" << "    <POI>mu_vbf mu_ggh mu_vh</POI>\n" << "  </Measurement>\n\n" << "</Combination>\n";
	outfile.close();
	return -1;
}
