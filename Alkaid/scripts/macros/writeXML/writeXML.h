#ifndef _writeXML_
#define _writeXML_
#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "../General/RootSysCmd.h"
#include "../General/Utilities.h"
#include "../General/Path.h"
#include "TFile.h"
#include "TH1F.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "TString.h"
#include "TSystem.h"


namespace H4l_XML {
  
	const TString DATA_VERSION[2] = {"data_run_2_obs", "data_run_2_Asimov"};
	const TString DATA_PATH = APATH::BASE_PATH + "/All_Input/input_sample/";
  const TString XML_PATH = APATH::BASE_PATH + "/All_Input/input_xml/";
  const TString BASE_PATH = APATH::BASE_PATH + "/All_Input/";
  const TString SAMPLE_IN = "/samples_BSM_khvv_only.txt";
  const TString CATEGORY_IN =  "/category.txt" ;
  const TString NORM_IN = "norm_factor_EFT_1D.txt";
  const TString SYST_IN = "systematics_fake.txt";
  const TString OUT_PATH = APATH::BASE_PATH + "/All_Input/workspace_xml/";
  const TString DTD_PATH = XML_PATH + "HistFactorySchema.dtd";
  const TString WORKSPACE_PATH  = APATH::BASE_PATH + "All_Input/workspace/";
  const TString DISCRIMINANT = "BDT_1bin";

	struct SysToWrite {
		TString name;
		double high;
		double low;
		double val;
		bool constant;
		SysToWrite(const TString _name_ = "", const double _high_ = 0, const double _low_ = 0, const double _val_ = 0, const bool _constant_ = false) :
			name(_name_), high(_high_), low(_low_), val(_val_), constant(_constant_) {};
		inline friend std::vector<SysToWrite>  operator +(const SysToWrite &a, const SysToWrite &b)
		{
			std::vector<SysToWrite> result;
			result.push_back(a);
			result.push_back(b);
			return result;
		}
		inline friend std::vector<SysToWrite>  operator +(const std::vector<SysToWrite> &a, const SysToWrite &b)
		{
			std::vector<SysToWrite> result = a;
			result.push_back(b);
			return result;
		}
		inline friend std::vector<SysToWrite>  operator +(const SysToWrite &b, const std::vector<SysToWrite> &a)
		{
			return a + b;
		}
	};

	struct Category {
		TString name;
		std::vector<SysToWrite> content;
		Category();
		Category(const TString _name_, const std::vector<SysToWrite> &_content_) :
			name(_name_), content(_content_) {};
		Category(const TString _name_, const SysToWrite &_content_) {
			std::vector<SysToWrite> result;
			result.push_back(_content_);
			name = _name_;
			content = result;
		}
		inline friend std::vector<Category>  operator +(const Category &a, const Category &b)
		{
			std::vector<Category> result;
			result.push_back(a);
			result.push_back(b);
			return result;
		}
		inline friend std::vector<Category>  operator +(const std::vector<Category> &a, const Category &b)
		{
			std::vector<Category> result = a;
			result.push_back(b);
			return result;
		}
		inline friend std::vector<Category>  operator +(const Category &b, const std::vector<Category> &a)
		{
			return a + b;
		}
	};
	struct SamStruct {
		TString name;
		std::vector<Category> condition;
		SamStruct(const TString _name_, std::vector<Category> _condition_) :
			name(_name_), condition(_condition_) {};
		SamStruct(const TString _name_, Category _condition_) {
			std::vector<Category> result;
			result.push_back(_condition_);
			name = _name_;
			condition = result;
		}
		inline friend std::vector<SamStruct>  operator +(const SamStruct &a, const SamStruct &b)
		{
			std::vector<SamStruct> result;
			result.push_back(a);
			result.push_back(b);
			return result;
		}
		inline friend std::vector<SamStruct>  operator +(const std::vector<SamStruct> &a, const SamStruct &b)
		{
			std::vector<SamStruct> result = a;
			result.push_back(b);
			return result;
		}
		inline friend std::vector<SamStruct>  operator +(const SamStruct &b, const std::vector<SamStruct> &a)
		{
			return a + b;
		}
	};


	std::vector<SamStruct> NormFactor_vector_vv = SamStruct("VBFH125", Category("all", SysToWrite("mu_vbf", 20., 0., 1., true))) +
		SamStruct("ggH125", Category("all", SysToWrite("mu_ggh", 90., 0., 1., true))) +
		SamStruct("WH125||ZH125", Category("all", SysToWrite("mu_vh", 90., 0., 1., true))) +
		SamStruct("*bfMG5", Category("all", SysToWrite("mu_vbf_BASE_BSM", 90., 0., 1., true))) +
		SamStruct("*gfMG", Category("~VHlep~ttH~2jet_VBF_highptjet", SysToWrite("mu_ggf_BASE_BSM", 90., 0., 1., true))) +
		SamStruct("ttH||bbH_SUM", Category("all", SysToWrite("mu_tth_norm", 90., 0., 1., true))) +
		SamStruct("ggfMG5_0p", Category("VHlep||ttH||2jet_VBF_highptjet", SysToWrite("mu_tth_norm", 90., 0., 1., true)));
   
	std::vector<SamStruct> NormFactor_vector_ggf = SamStruct("VBFH125", Category("all", SysToWrite("mu_vbf", 20., 0., 1., true))) +
		SamStruct("ggH125", Category("all", SysToWrite("mu_ggh", 90., 0., 1., true))) +
		SamStruct("WH125||ZH125", Category("all", SysToWrite("mu_vh", 90., 0., 1., true))) +
		SamStruct("*gfMG", Category("all", SysToWrite("mu_ggf_BASE_BSM", 90., 0., 1., true)));   

	std::vector<SamStruct> TheorySys_vector =
		SamStruct("*bfMG5", Category("0jet", SysToWrite("ATLAS_UEPS_qqH", 0.9824, 1.0155) +
			SysToWrite("QCDscale_qqH_H4l", 0.9848, 1.0140) +
			SysToWrite("pdf_Higgs_qqH_H4l", 1.0029, 0.9887)) +
			Category("1jet_highpt4l||1jet_mediumpt4l||1jet_lowpt4l", SysToWrite("ATLAS_UEPS_qqH", 0.9937, 1.0096) +
				SysToWrite("QCDscale_qqH_H4l", 0.9967, 1.0048) +
				SysToWrite("pdf_Higgs_qqH_H4l", 1.0029, 0.994)) +
			Category("2jet_VH_highpt4l||2jet_VH_lowpt4l", SysToWrite("ATLAS_UEPS_qqH", 1.0134, 0.9668) +
				SysToWrite("QCDscale_qqH_H4l", 1.0063, 0.9906) +
				SysToWrite("pdf_Higgs_qqH_H4l", 1.0033, 0.9942)) +
			Category("2jet_VBF_highpt4l||2jet_VBF_lowpt4l", SysToWrite("ATLAS_UEPS_qqH", 1.0071, 0.9932) +
				SysToWrite("QCDscale_qqH_H4l", 1.0080, 0.9937) +
				SysToWrite("pdf_Higgs_qqH_H4l", 1.0062, 0.9942)) +
			Category("2jet_VBF_highptjet||2jet_VBF_lowptjet", SysToWrite("ATLAS_UEPS_qqH", 1.0071, 0.9932) +
				SysToWrite("QCDscale_qqH_H4l", 1.0080, 0.9937) +
				SysToWrite("pdf_Higgs_qqH_H4l", 1.0062, 0.9942)) +
			Category("all", SysToWrite("BR_VV", 1.027, 0.973) +
				SysToWrite("QCDscale_qqH_WH_ZH", 1.009, 0.992) +
				SysToWrite("pdf_Higgs_VBF_WH_ZH", 1.02, 0.980))) +
		SamStruct("*gfMG", Category("0jet", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 1.002192, 0.997808) +
			SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.961256, 1.038744) +
			SysToWrite("ATLAS_QCDres_ggF", 0.996396, 1.003604) +
			SysToWrite("ATLAS_QCDscale_ggH_m01", 1.034499, 0.965501) +
			SysToWrite("ATLAS_QCDscale_ggH_m12", 1.003297, 0.996703) +
			SysToWrite("ATLAS_UEPS_ggH", 0.9882, 1.0121)) +
			Category("1jet_lowpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 1.027824, 0.972176) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.952323, 1.047677) +
				SysToWrite("ATLAS_QCDres_ggF", 0.969791, 1.030209) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.965825, 1.034175) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 1.035315, 0.964685)) +
			Category("1jet_mediumpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.971327, 1.028673) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.946167, 1.053833) +
				SysToWrite("ATLAS_QCDres_ggF", 0.953414, 1.046586) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.928297, 1.071703) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 1.045157, 0.954843)) +
			Category("1jet_highpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.863317, 1.136683) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.944378, 1.055622) +
				SysToWrite("ATLAS_QCDres_ggF", 0.950501, 1.049499) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.926644, 1.073356) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 1.036465, 0.963535)) +
			Category("2jet_VH_highpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.880142, 1.119858) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.919116, 1.080884) +
				SysToWrite("ATLAS_QCDres_ggF", 0.919718, 1.080282) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.955072, 1.044928) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.849185, 1.150815)) +
			Category("2jet_VH_lowpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 1.002987, 0.997013) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.930315, 1.069685) +
				SysToWrite("ATLAS_QCDres_ggF", 0.935657, 1.064343) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.953947, 1.046053) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.917889, 1.082111)) +
			Category("2jet_VBF_lowpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.999370, 1.000630) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.926087, 1.073913) +
				SysToWrite("ATLAS_QCDres_ggF", 0.929759, 1.070241) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.954969, 1.045031) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.891207, 1.108793)) +
			Category("1jet_highpt4l||1jet_mediumpt4l||1jet_lowpt4l", SysToWrite("ATLAS_UEPS_ggH", 1.0099, 0.9859)) +
			Category("2jet_VH_highpt4l||2jet_VH_lowpt4l", SysToWrite("ATLAS_UEPS_ggH", 1.0406, 0.9999)) +
			Category("2jet_VBF_highpt4l||2jet_VBF_lowpt4l", SysToWrite("ATLAS_UEPS_ggH", 1.0334, 0.9578)) +
			Category("2jet_VBF_highptjet||2jet_VBF_lowptjet", SysToWrite("ATLAS_UEPS_ggH", 1.0334, 0.9578))) +
		SamStruct("ggfMG5_0p", Category("0jet", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 1.002192, 0.997808) +
			SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.961256, 1.038744) +
			SysToWrite("ATLAS_QCDres_ggF", 0.996396, 1.003604) +
			SysToWrite("ATLAS_QCDscale_ggH_m01", 1.034499, 0.965501) +
			SysToWrite("ATLAS_QCDscale_ggH_m12", 1.003297, 0.996703) +
			SysToWrite("ATLAS_UEPS_ggH", 0.9882, 1.0121)) +
			Category("1jet_lowpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 1.027824, 0.972176) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.952323, 1.047677) +
				SysToWrite("ATLAS_QCDres_ggF", 0.969791, 1.030209) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.965825, 1.034175) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 1.035315, 0.964685)) +
			Category("1jet_mediumpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.971327, 1.028673) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.946167, 1.053833) +
				SysToWrite("ATLAS_QCDres_ggF", 0.953414, 1.046586) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.928297, 1.071703) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 1.045157, 0.954843)) +
			Category("1jet_highpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.863317, 1.136683) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.944378, 1.055622) +
				SysToWrite("ATLAS_QCDres_ggF", 0.950501, 1.049499) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.926644, 1.073356) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 1.036465, 0.963535)) +
			Category("2jet_VH_highpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.880142, 1.119858) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.919116, 1.080884) +
				SysToWrite("ATLAS_QCDres_ggF", 0.919718, 1.080282) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.955072, 1.044928) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.849185, 1.150815)) +
			Category("2jet_VH_lowpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 1.002987, 0.997013) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.930315, 1.069685) +
				SysToWrite("ATLAS_QCDres_ggF", 0.935657, 1.064343) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.953947, 1.046053) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.917889, 1.082111)) +
			Category("2jet_VBF_lowpt4l", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.999370, 1.000630) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.926087, 1.073913) +
				SysToWrite("ATLAS_QCDres_ggF", 0.929759, 1.070241) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.954969, 1.045031) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.891207, 1.108793)) +
			Category("2jet_VBF_highptjet", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.844296, 1.155704) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.918258, 1.081742) +
				SysToWrite("ATLAS_QCDres_ggF", 0.918674, 1.081326) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.956038, 1.043962) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.842827, 1.157173)) +
			Category("VHlep", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.993972, 1.006028) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.945747, 1.054253) +
				SysToWrite("ATLAS_QCDres_ggF", 0.961073, 1.038927) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.969692, 1.030308) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.990975, 1.009025)) +
			Category("ttH", SysToWrite("ATLAS_QCDscale_ggH_pt_m", 0.946800, 1.053200) +
				SysToWrite("ATLAS_QCDscale_ggF_H4l", 0.917775, 1.082225) +
				SysToWrite("ATLAS_QCDres_ggF", 0.918331, 1.081669) +
				SysToWrite("ATLAS_QCDscale_ggH_m01", 0.957818, 1.042182) +
				SysToWrite("ATLAS_QCDscale_ggH_m12", 0.837700, 1.162300)) +
			Category("1jet_highpt4l||1jet_mediumpt4l||1jet_lowpt4l", SysToWrite("ATLAS_UEPS_ggH", 1.0099, 0.9859)) +
			Category("2jet_VH_highpt4l||2jet_VH_lowpt4l", SysToWrite("ATLAS_UEPS_ggH", 1.0406, 0.9999)) +
			Category("2jet_VBF_highpt4l||2jet_VBF_lowpt4l", SysToWrite("ATLAS_UEPS_ggH", 1.0334, 0.9578)) +
			Category("2jet_VBF_highptjet||2jet_VBF_lowptjet", SysToWrite("ATLAS_UEPS_ggH", 1.0334, 0.9578)) +
			Category("all", SysToWrite("pdf_Higgs_gg_ttH", 1.032, 0.968) + SysToWrite("BR_VV", 1.027, 0.973) + SysToWrite("QCDscale_bbH_ggH_ttH", 1.048, 0.931))) +
		SamStruct("all", Category("all", SysToWrite("ATLAS_luminoisty", 1.037, 0.963)));

	std::vector<SamStruct> AddSys_RedSys_vector = SamStruct("Redu_Bkg", Category("0jet", SysToWrite("red_bkg_syst_0jet", 1.0897, 0.9103)) +
		Category("1jet_highpt4l", SysToWrite("red_bkg_syst_1jet", 1.12, 0.88)) +
		Category("1jet_mediumpt4l", SysToWrite("red_bkg_syst_1jet", 1.12, 0.88)) +
		Category("1jet_lowpt4l", SysToWrite("red_bkg_syst_1jet", 1.192, 0.808)) +
		Category("2jet_VBF_highptjet", SysToWrite("red_bkg_syst_2jet_vbf", 1.79, 0.21)) +
		Category("2jet_VH_highpt4l", SysToWrite("red_bkg_syst_2jet_vh", 1.17, 0.83)) +
		Category("2jet_VBF_lowptjet", SysToWrite("red_bkg_syst_2jet_vbf", 1.162, 0.838)) +
		Category("2jet_VH_lowpt4l", SysToWrite("red_bkg_syst_2jet_vh", 1.214, 0.786)) +
		Category("VHlep", SysToWrite("red_bkg_syst_vhlep", 1.17, 0.83)) +
		Category("ttH", SysToWrite("red_bkg_syst_tth", 1.17, 0.83))) +
		SamStruct("", Category("", SysToWrite("")));

	std::vector<SamStruct> AddSys_TheorySys_vector = SamStruct("ZZllll", Category("all", SysToWrite("QCDscale_VV", 1.05, 0.93) + SysToWrite("pdf_qq", 1.05, 0.95))) +
		SamStruct("ggZZ_4l", Category("all", SysToWrite("QCDscale_ggVV", 1.60, 0.40) + SysToWrite("pdf_gg", 1.08, 0.92))) +
		SamStruct("ggZZ_2mu2e", Category("all", SysToWrite("QCDscale_ggVV", 1.60, 0.40) + SysToWrite("pdf_gg", 1.08, 0.92)));

	TString SysMatch(TString sample, TString category, std::vector<SamStruct> vec, TString basename = "OverallSys Name");
                       //ErrorType = "EXP STAT THEO"
	int writeXML(TString ErrorType = "", TString sample_in = XML_PATH + SAMPLE_IN, TString category_in = XML_PATH + CATEGORY_IN, TString norm_in = XML_PATH + NORM_IN, TString syst_in = XML_PATH + SYST_IN, TString jetptcut = "35",TString discriminant = DISCRIMINANT, TString outpath = OUT_PATH, const bool &IsAsimov = true, TString mode = "vv");
  int writeDriver(TString dtd_in = DTD_PATH, TString workspace_out = WORKSPACE_PATH, TString category_in = XML_PATH + CATEGORY_IN, TString outpath = OUT_PATH, TString discriminant = DISCRIMINANT);

}

#endif

