#!/usr/bin/env python

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from ROOT import *
ROOT.gROOT.SetBatch(ROOT.kTRUE)

from array import *
import os

def convertStr(s):
    ret = str(s)
    return ret


gSystem.Load("macros/create_all_categories_inputs_correct_lumi_C.so")
gSystem.Load("macros/add_inputs_BSM_tth_ggf_C.so")
gSystem.Load("macros/add_inputs_mg5_all_C.so")
gSystem.Load("macros/create_inputs_normsyst_correct_lumi_C.so")
gSystem.Load("macros/add_ggH_histo_and_ttH_C.so")
gSystem.Load("macros/add_inputs_some_redu_bkg_no_bbh_tth_sum_C.so")
gSystem.Load("macros/add_inputs_bbH_sum_C.so")
gSystem.Load("macros/sumup_ZZ_contributions_C.so")

#RD_link = '/afs/cern.ch/atlas/groups/HSG2/H4l/run2/2016/MiniTrees/Prod_v04/'
#RD_link = '/afs/cern.ch/atlas/groups/HSG2/H4l/run2/2016/MiniTrees/Prod_v09_2/'
RD_link = '/afs/cern.ch/atlas/groups/HSG2/H4l/run2/2016/MiniTrees/Prod_v12/'
#data_link = '/afs/cern.ch/atlas/groups/HSG2/H4l/run2/2016/MiniTrees/Prod_v09/data/Nominal/data_13TeV.root'
data_link = '/afs/cern.ch/atlas/groups/HSG2/H4l/run2/2016/MiniTrees/Prod_v12/data/Nominal/data_13TeV.root'

coupling_type = 'khvv'#'khvv' # 'kavv'

ptj = 30.  #35. 30. 25.

#if(coupling_type == 'khvv') : m_sam = ['ZZllll_second_file','ggZZ_4l','ggZZ_2mu2e','ttH125','ggH125','ZZllll','vbfMG5_0p','vbfMG5_0phkHv10','vbfMG5_0pphkHv5','vbfMG5_0pphkHvm5','vbfMG5_0pphkHv2p5','vbfMG5_0pphkHvm2p5','data_run_2']
if(coupling_type == 'khvv') : m_sam = ['redu_bbH_1', 'redu_bbH_2', 'redu_WWZ' , 'redu_WZZ' , 'redu_ZZZ_1', 'redu_ZZZ_2' , 'redu_ttZ_1', 'redu_ttZ_2','redu_ttW','ggH_SM','ggH_Khgg1_Kagg0p5','ggH_Khgg1_Kaggm0p5','ZZllll_mll4_1','ZZllll_mll4_2','ZZllll_mll4_3','vbfMG5_0p_had','vbfMG5_0p_lep','gg_ZZ_bkg_2e2mu','gg_ZZ_bkg_4l_noTau','data_run_2']

#to do: 
#1) sum lep and had vbf for MG5 or using powheg
#2) add samples :
#	mc15_13TeV.361603.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZllll_mll4.root

#/afs/cern.ch/atlas/groups/HSG2/H4l/run2/2016/MiniTrees/Prod_v09_2/mc/Nominal/mc15_13TeV.344158.aMcAtNloPythia8EvtGen_A14NNPDF23LO_ppx0_FxFx_Np012_SM
if(coupling_type == 'khvv') : m_sample = ['mc15_13TeV.344973.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_yb2_ZZ4lep_noTau', 'mc15_13TeV.344974.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_ybyt_ZZ4lep_noTau', 'mc15_13TeV.361621.Sherpa_CT10_WWZ_4l2v', 'mc15_13TeV.361623.Sherpa_CT10_WZZ_5l1v', 'mc15_13TeV.361625.Sherpa_CT10_ZZZ_6l0v' , 'mc15_13TeV.361626.Sherpa_CT10_ZZZ_4l2v', 'mc15_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee','mc15_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu', 'mc15_13TeV.410144.Sherpa_NNPDF30NNLO_ttW', 'mc15_13TeV.344158.aMcAtNloPythia8EvtGen_A14NNPDF23LO_ppx0_FxFx_Np012_SM','mc15_13TeV.344167.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAgg0p5_cosa0p707','mc15_13TeV.344168.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAggn0p5_cosa0p707','mc15_13TeV.345107.Sherpa_221_NNPDF30NNLO_llll_m4l100_300_filt100_150','mc15_13TeV.363490.Sherpa_221_NNPDF30NNLO_llll','mc15_13TeV.345108.Sherpa_221_NNPDF30NNLO_llll_m4l300','mc15_13TeV.343247.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0p','mc15_13TeV.344135.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0p','mc15_13TeV.343212.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_2e2mu_13TeV', 'mc15_13TeV.343213.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_4l_noTau_13TeV','data_run_2']

m_category = ['0jet','1jet_highpt4l','1jet_mediumpt4l','1jet_lowpt4l','2jet_VBF_highptjet','2jet_VBF_lowptjet','2jet_VH_highpt4l','2jet_VH_lowpt4l','VHlep', 'ttH']
# old values

#rescale to NNNNLO prodv10
#reweight_factor     = [1.33481, 1.53603, 1.41750, 1.31191, 1.00693, 1.04045, 1.36269, 1.31042, 1.30428, 2.03011]
#reweight_factor_ggf = [1.57324, 1.21386, 1.45960, 1.43423, 1.00681, 1.25451, 0.89917, 1.45197, 1.11383, 0.88220]

#rescale to NNNNLO prodv11
#reweight_factor     = [1.350833, 1.5381489, 1.4292038, 1.3267807, 1.0138088, 1.0492949, 1.3711441, 1.3261007, 1.3121445, 1.96821756]
#reweight_factor_ggf = [1.5589336, 1.200558, 1.45666, 1.428596, 1.0073846, 1.2483427, 0.9175183, 1.437565, 1.140881, 0.9207075]

#rescale to NNNNLO prodv12
reweight_factor     = [1.35287, 1.53585, 1.42850, 1.32721, 1.01122, 1.04939, 1.36642, 1.32635, 1.31368, 1.94689]
reweight_factor_ggf = [1.55827, 1.20101, 1.45916, 1.43005, 0.99916, 1.24639, 0.90243, 1.43910, 1.15480, 0.91722]

#no rescaled
#reweight_factor     = [1., 1., 1., 1., 1., 1., 1., 1., 1., 1.]
#reweight_factor_ggf = [1., 1., 1., 1., 1., 1., 1., 1., 1., 1.]
# this are used for 
 
#OLD VERSION
#reweight_factor_qqZZ = [0.93,0.97,0.97,0.97,1.51,1.51,1.51,1.51,1.51,1.51]
#from Haider 3 March 2017
#reweight_factor_qqZZ = [0.949,0.977,0.977,0.977,1.773,1.773,1.773,1.773,1.773,1.773]
reweight_factor_qqZZ = [1.,1.,1.,1.,1.,1.,1.,1.,1.,1.]
#

#m_syst = ['JET_JvtEfficiency__1up','JET_JvtEfficiency__1down','EG_RESOLUTION_ALL__1down', 'EG_RESOLUTION_ALL__1up', 'EG_SCALE_ALLCORR__1down', 'EG_SCALE_ALLCORR__1up', 'EG_SCALE_E4SCINTILLATOR__1down', 'EG_SCALE_E4SCINTILLATOR__1up', 'EG_SCALE_LARCALIB_EXTRA2015PRE__1down', 'EG_SCALE_LARCALIB_EXTRA2015PRE__1up', 'EG_SCALE_LARTEMPERATURE_EXTRA2015PRE__1down', 'EG_SCALE_LARTEMPERATURE_EXTRA2015PRE__1up', 'EG_SCALE_LARTEMPERATURE_EXTRA2016PRE__1down', 'EG_SCALE_LARTEMPERATURE_EXTRA2016PRE__1up', 'JET_BJES_Response__1down', 'JET_BJES_Response__1up', 'JET_EffectiveNP_1__1down', 'JET_EffectiveNP_1__1up', 'JET_EffectiveNP_2__1down', 'JET_EffectiveNP_2__1up', 'JET_EffectiveNP_3__1down', 'JET_EffectiveNP_3__1up', 'JET_EffectiveNP_4__1down', 'JET_EffectiveNP_4__1up', 'JET_EffectiveNP_5__1down', 'JET_EffectiveNP_5__1up', 'JET_EffectiveNP_6__1down', 'JET_EffectiveNP_6__1up', 'JET_EffectiveNP_7__1down', 'JET_EffectiveNP_7__1up', 'JET_EffectiveNP_8restTerm__1down', 'JET_EffectiveNP_8restTerm__1up', 'JET_EtaIntercalibration_Modelling__1down', 'JET_EtaIntercalibration_Modelling__1up', 'JET_EtaIntercalibration_NonClosure__1down', 'JET_EtaIntercalibration_NonClosure__1up', 'JET_EtaIntercalibration_TotalStat__1down', 'JET_EtaIntercalibration_TotalStat__1up', 'JET_Flavor_Composition__1down', 'JET_Flavor_Composition__1up', 'JET_Flavor_Response__1down', 'JET_Flavor_Response__1up', 'JET_JER_SINGLE_NP__1up', 'JET_Pileup_OffsetMu__1down', 'JET_Pileup_OffsetMu__1up', 'JET_Pileup_OffsetNPV__1down', 'JET_Pileup_OffsetNPV__1up', 'JET_Pileup_PtTerm__1down', 'JET_Pileup_PtTerm__1up', 'JET_Pileup_RhoTopology__1down', 'JET_Pileup_RhoTopology__1up', 'JET_PunchThrough_MC15__1down', 'JET_PunchThrough_MC15__1up', 'JET_SingleParticle_HighPt__1down', 'JET_SingleParticle_HighPt__1up', 'MUON_ID__1down', 'MUON_ID__1up', 'MUON_MS__1down', 'MUON_MS__1up', 'MUON_SAGITTA_RESBIAS__1down', 'MUON_SAGITTA_RESBIAS__1up', 'MUON_SAGITTA_RHO__1down', 'MUON_SAGITTA_RHO__1up', 'MUON_SCALE__1down', 'MUON_SCALE__1up', 'NormSystematic']#, 'TheorySystematic'] 

m_syst = ['NormSystematic']
m_syst = ['JET_JvtEfficiency__1up','JET_JvtEfficiency__1down']
m_syst = []
#MY VERSION # m_syst_norm = ['weight_var_EL_EFF_ID_CorrUncertaintyNP0','weight_var_EL_EFF_ID_CorrUncertaintyNP1','weight_var_EL_EFF_ID_CorrUncertaintyNP2','weight_var_EL_EFF_ID_CorrUncertaintyNP3','weight_var_EL_EFF_ID_CorrUncertaintyNP4','weight_var_EL_EFF_ID_CorrUncertaintyNP5','weight_var_EL_EFF_ID_CorrUncertaintyNP6','weight_var_EL_EFF_ID_CorrUncertaintyNP7','weight_var_EL_EFF_ID_CorrUncertaintyNP8','weight_var_EL_EFF_ID_CorrUncertaintyNP9','weight_var_EL_EFF_ID_CorrUncertaintyNP10','weight_var_EL_EFF_ID_CorrUncertaintyNP11','weight_var_EL_EFF_ID_CorrUncertaintyNP12','weight_var_EL_EFF_ID_CorrUncertaintyNP13','weight_var_EL_EFF_ID_CorrUncertaintyNP14','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP0','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP1','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP2','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP3','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP4','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP5','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP6','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP7','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP8','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP9','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP10','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP11','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP12','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP13','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP14','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP15','weight_var_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1down', 'weight_var_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1up','weight_var_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1down', 'weight_var_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1up', 'weight_var_FT_EFF_Eigen_B_0__1down','weight_var_FT_EFF_Eigen_B_0__1up','weight_var_FT_EFF_Eigen_B_1__1down','weight_var_FT_EFF_Eigen_B_1__1up','weight_var_FT_EFF_Eigen_B_2__1down','weight_var_FT_EFF_Eigen_B_2__1up','weight_var_FT_EFF_Eigen_C_0__1down','weight_var_FT_EFF_Eigen_C_0__1up','weight_var_FT_EFF_Eigen_C_1__1down','weight_var_FT_EFF_Eigen_C_1__1up','weight_var_FT_EFF_Eigen_C_2__1down','weight_var_FT_EFF_Eigen_C_2__1up','weight_var_FT_EFF_Eigen_C_3__1down','weight_var_FT_EFF_Eigen_C_3__1up','weight_var_FT_EFF_Eigen_Light_0__1down','weight_var_FT_EFF_Eigen_Light_0__1up','weight_var_FT_EFF_Eigen_Light_1__1down','weight_var_FT_EFF_Eigen_Light_1__1up','weight_var_FT_EFF_Eigen_Light_2__1down','weight_var_FT_EFF_Eigen_Light_2__1up','weight_var_FT_EFF_Eigen_Light_3__1down','weight_var_FT_EFF_Eigen_Light_3__1up','weight_var_FT_EFF_Eigen_Light_4__1down','weight_var_FT_EFF_Eigen_Light_4__1up','weight_var_FT_EFF_extrapolation__1down','weight_var_FT_EFF_extrapolation__1up','weight_var_FT_EFF_extrapolation_from_charm__1down','weight_var_FT_EFF_extrapolation_from_charm__1up','weight_var_HOEW_QCD_syst__1down', 'weight_var_HOEW_QCD_syst__1up', 'weight_var_HOEW_syst__1down', 'weight_var_HOEW_syst__1up','weight_var_HOQCD_scale_syst__1down', 'weight_var_HOQCD_scale_syst__1up','weight_var_MUON_EFF_STAT__1down', 'weight_var_MUON_EFF_STAT__1up', 'weight_var_MUON_EFF_STAT_LOWPT__1down', 'weight_var_MUON_EFF_STAT_LOWPT__1up','weight_var_MUON_EFF_SYS__1down', 'weight_var_MUON_EFF_SYS__1up', 'weight_var_MUON_EFF_SYS_LOWPT__1down', 'weight_var_MUON_EFF_SYS_LOWPT__1up', 'weight_var_MUON_ISO_STAT__1down', 'weight_var_MUON_ISO_STAT__1up', 'weight_var_MUON_ISO_SYS__1down', 'weight_var_MUON_ISO_SYS__1up', 'weight_var_MUON_TTVA_STAT__1down', 'weight_var_MUON_TTVA_STAT__1up', 'weight_var_MUON_TTVA_SYS__1down', 'weight_var_MUON_TTVA_SYS__1up', 'weight_var_PRW_DATASF__1down', 'weight_var_PRW_DATASF__1up','weight_var_FT_EFF_Eigen_Light_0__1up']

#TAK VERSION
m_syst_norm = ['weight_var_EL_EFF_ID_CorrUncertaintyNP0__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP0__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP1__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP1__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP2__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP2__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP3__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP3__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP4__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP4__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP5__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP5__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP6__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP6__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP7__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP7__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP8__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP8__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP9__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP9__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP10__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP10__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP11__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP11__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP12__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP12__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP13__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP13__1up','weight_var_EL_EFF_ID_CorrUncertaintyNP14__1down','weight_var_EL_EFF_ID_CorrUncertaintyNP14__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP0__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP0__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP1__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP1__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP2__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP2__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP3__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP3__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP4__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP4__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP5__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP5__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP6__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP6__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP7__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP7__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP8__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP8__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP9__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP9__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP10__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP10__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP11__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP11__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP12__1down','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP12__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP13__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP13__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP14__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP14__1up','weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP15__1down', 'weight_var_EL_EFF_ID_SIMPLIFIED_UncorrUncertaintyNP15__1up','weight_var_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1down', 'weight_var_EL_EFF_Iso_TOTAL_1NPCOR_PLUS_UNCOR__1up', 'weight_var_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1down', 'weight_var_EL_EFF_Reco_TOTAL_1NPCOR_PLUS_UNCOR__1up','weight_var_FT_EFF_Eigen_B_0__1down','weight_var_FT_EFF_Eigen_B_0__1up','weight_var_FT_EFF_Eigen_B_1__1down','weight_var_FT_EFF_Eigen_B_1__1up','weight_var_FT_EFF_Eigen_B_2__1down','weight_var_FT_EFF_Eigen_B_2__1up','weight_var_FT_EFF_Eigen_C_0__1down','weight_var_FT_EFF_Eigen_C_0__1up','weight_var_FT_EFF_Eigen_C_1__1down','weight_var_FT_EFF_Eigen_C_1__1up','weight_var_FT_EFF_Eigen_C_2__1down']
#,'weight_var_FT_EFF_Eigen_C_2__1up','weight_var_FT_EFF_Eigen_C_3__1down','weight_var_FT_EFF_Eigen_C_3__1up','weight_var_FT_EFF_Eigen_Light_0__1down','weight_var_FT_EFF_Eigen_Light_0__1up','weight_var_FT_EFF_Eigen_Light_1__1down','weight_var_FT_EFF_Eigen_Light_1__1up','weight_var_FT_EFF_Eigen_Light_2__1down','weight_var_FT_EFF_Eigen_Light_2__1up','weight_var_FT_EFF_Eigen_Light_3__1down','weight_var_FT_EFF_Eigen_Light_3__1up','weight_var_FT_EFF_Eigen_Light_4__1down','weight_var_FT_EFF_Eigen_Light_4__1up','weight_var_FT_EFF_extrapolation__1down','weight_var_FT_EFF_extrapolation__1up','weight_var_FT_EFF_extrapolation_from_charm__1down','weight_var_FT_EFF_extrapolation_from_charm__1up','weight_var_HOEW_QCD_syst__1down', 'weight_var_HOEW_QCD_syst__1up', 'weight_var_HOEW_syst__1down', 'weight_var_HOEW_syst__1up','weight_var_HOQCD_scale_syst__1down', 'weight_var_HOQCD_scale_syst__1up','weight_var_JET_JvtEfficiency__1down', 'weight_var_JET_JvtEfficiency__1up','weight_var_MUON_EFF_STAT__1down', 'weight_var_MUON_EFF_STAT__1up', 'weight_var_MUON_EFF_STAT_LOWPT__1down', 'weight_var_MUON_EFF_STAT_LOWPT__1up', 'weight_var_MUON_EFF_SYS__1down', 'weight_var_MUON_EFF_SYS__1up', 'weight_var_MUON_EFF_SYS_LOWPT__1down', 'weight_var_MUON_EFF_SYS_LOWPT__1up', 'weight_var_MUON_ISO_STAT__1down', 'weight_var_MUON_ISO_STAT__1up', 'weight_var_MUON_ISO_SYS__1down', 'weight_var_MUON_ISO_SYS__1up', 'weight_var_MUON_TTVA_STAT__1down', 'weight_var_MUON_TTVA_STAT__1up', 'weight_var_MUON_TTVA_SYS__1down', 'weight_var_MUON_TTVA_SYS__1up', 'weight_var_PRW_DATASF__1down', 'weight_var_PRW_DATASF__1up']
m_syst = []


m_discriminant1 = ['BDT_discriminant','BDT_OneJet_jptetadR_discriminant','BDT_OneJet_jptetadR_discriminant','BDT_OneJet_jptetadR_discriminant','BDT_TwoJet_discriminant','BDT_TwoJet_discriminant','BDT_VH_noptHjj_discriminant','BDT_VH_noptHjj_discriminant','m4l_constrained_HM','m4l_constrained_HM']
m_discriminant2 = ['BDT_discriminant','BDT_OneJet_jptetadR_discriminant','BDT_OneJet_jptetadR_discriminant','BDT_OneJet_jptetadR_discriminant','BDT_TwoJet_discriminant','BDT_TwoJet_discriminant','BDT_VH_noptHjj_discriminant','BDT_VH_noptHjj_discriminant','m4l_constrained_HM','m4l_constrained_HM']
m_min = [-1.,-1.,-1.,-1.,-1.,-1.,-1.,-1.,110.,110.]  # the values are the minimums for each cat
m_max = [1.,1.,1.,1.,1.,1.,1.,1.,145.,145.]   # the values are the maximums for each cat     




reweight_kfactor_fxfx = 1.
#reweight_kfactor_fxfx already in the minitrees
# 21.2302 - ggF125
#14.5683 - FxFx
#kFactor = 1.457

luminosita_nostra = 1.
#luminosity = 14.78
luminosity = luminosita_nostra
#looking at the correct lumi via w_lumi in the create_input
m_ptj = convertStr(ptj)
s_ptj = m_ptj.split('.')[0]
m_type = 'asimovData'#'asimovData' #'ObsData'
asimov_type = 'ggH_SM'#'ggH_Khgg1_Kaggm0p5' #'ggH_Khgg1_Kagg0p5'#'ggH_SM' #'vbfMG5_0pphkHv5' #'mc15_13TeV.343247.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0p'#'vbfMG5_0pphmkAvm2p5' #'vbfMG5_0p' # 'vbfMG5_0pphkHv5' #'vbfMG5_0p'
#kfactor_vh_had = 0.826446281 # 1. divided by 1.21

for cat_i in range(len(m_category)) :
    lumi = luminosity
    category = m_category[cat_i]
    n_min = m_min[cat_i]
    n_max = m_max[cat_i]
    discriminant1 = m_discriminant1[cat_i]
    discriminant2 = m_discriminant2[cat_i]
    value_reweight_factor = reweight_factor[cat_i]
    value_reweight_factor_ggf = reweight_factor_ggf[cat_i]
    value_reweight_factor_qqZZ = reweight_factor_qqZZ[cat_i]
    n_bin = 1

#    print 'Processing: ', category , n_min , n_max , discriminant1 , discriminant2 , n_bin
#    for sample in m_sample :
    for ind_i in range(len(m_sample)) :
        lumi = luminosity
     
        sam = m_sam[ind_i]
        sample = m_sample[ind_i]


        if ( category == 'VHlep') :  sample = m_sample[ind_i]
#	if ( category == 'VHlep') :
#        print 'Processing: ', category , sample , ptj , lumi
#       if 'kAv' in sample : lumi = lumi * 4.0024 # to eliminate the cos alpha form the Lagrangian


# to recover the K factor in VH Lep
        if (sample!='data_run_2') :
	   if 'vbfMG' in sam :  
		ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor,category,sam)
		if '_le' in sam :  ROOT.add_inputs_mg5_all(sam,n_bin,n_min,n_max,category,"nominal")
           elif 'Zllll_mll4_1' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_qqZZ,category,sam)
           elif 'Zllll_mll4_2' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_qqZZ,category,sam)
           elif 'Zllll_mll4_3' in sam :  
		ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_qqZZ,category,sam)
		ROOT.sumup_ZZ_contributions(sam+'_BDT',n_bin,n_min,n_max,category,asimov_type)
#g_ZZ_bk
           elif 'g_ZZ_bk' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,sam)
           elif 'edu_' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,sam)
           elif 'gH_SM' in sam :  
#mc15_13TeV.342561.aMcAtNloHerwigppEvtGen_UEEE5_CTEQ6L1_CT10ME_ttH125_4l
		ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/mc15_13TeV.342561.aMcAtNloHerwigppEvtGen_UEEE5_CTEQ6L1_CT10ME_ttH125_4l.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_ttH_nominal.root','ttH_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,'ttH')
                ROOT.add_inputs_some_redu_bkg_no_bbh_tth_sum( n_bin , n_min, n_max, category, "nominal")
		ROOT.add_inputs_bbH_sum( n_bin , n_min, n_max, category, "nominal")
		ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_ggf,category,sam)
#		ROOT.add_ggH_histo_and_ttH(sam,n_bin,n_min,n_max,category,"nominal")
           elif 'Kagg' in sam :  
		ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Nominal/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*4*value_reweight_factor_ggf,category,sam)
#                ROOT.add_ggH_histo_and_ttH(sam,n_bin,n_min,n_max,category,"nominal")
        elif ( (sample=='data_run_2')and(m_type=='ObsData') ) :
#	    ROOT.add_inputs_some_redu_bkg_no_bbh_tth_sum(n_bin,n_min,n_max,category,'nominal')
            ROOT.create_all_categories_inputs_correct_lumi(data_link,'tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_nominal.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,1.,category,sam)
        elif ( (sample=='data_run_2')and(m_type=='asimovData') ) :
#	    ROOT.add_inputs_some_redu_bkg_no_bbh_tth_sum(n_bin,n_min,n_max,category,'nominal')
            ROOT.add_inputs_BSM_tth_ggf(sam+'_BDT',n_bin,n_min,n_max,category,asimov_type)

    for syst in m_syst :
        for ind_i in range(len(m_sample)) :
            lumi = luminosity
            if(syst!='NormSystematic') :
                sam = m_sam[ind_i]
                sample = m_sample[ind_i]


	        if (sample!='data_run_2') :
	           if 'vbfMG' in sam :  
			ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+syst+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor,category,sam)
		  	if '_le' in sam : ROOT.add_inputs_mg5_all(sam,n_bin,n_min,n_max,category,syst)
	           elif 'Zllll_mll4' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+syst+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_qqZZ,category,sam)
#g_ZZ_bk
                   elif 'edu_' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+syst+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,sam)
                   elif 'g_ZZ_bk' in sam :  ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+syst+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,sam)
	           elif 'gH_SM' in sam :  
			ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/mc15_13TeV.342561.aMcAtNloHerwigppEvtGen_UEEE5_CTEQ6L1_CT10ME_ttH125_4l.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_ttH_'+syst+'.root','ttH_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,'ttH')
			ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+syst+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_ggf,category,sam)
#			ROOT.add_ggH_histo_and_ttH(sam,n_bin,n_min,n_max,category,syst)
#			I put here the SUM OF THE RED TO BE SURE THAT WE ARE DOING IT
	                ROOT.add_inputs_some_redu_bkg_no_bbh_tth_sum(n_bin,n_min,n_max,category,syst)
			ROOT.add_inputs_bbH_sum(n_bin,n_min,n_max,category,syst)
	           elif 'Kagg' in sam :  
			ROOT.create_all_categories_inputs_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+syst+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*4*value_reweight_factor_ggf,category,sam)
#			ROOT.add_ggH_histo_and_ttH(sam,n_bin,n_min,n_max,category,syst)



            if(syst=='NormSystematic') :
                lumi = luminosity
                for norm in m_syst_norm :
                   lumi = luminosity
                   sam = m_sam[ind_i]
                   sample = m_sample[ind_i]

                   print ' NOW YOU ARE Processing: ', norm
                   if (sample!='data_run_2') :
                       if 'vbfMG' in sam :  
			  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+norm+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor,category,sam,norm)
			  if '_le' in sam : ROOT.add_inputs_mg5_all(sam,n_bin,n_min,n_max,category,norm)
                       elif 'Zllll_mll4' in sam :  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+norm+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_qqZZ,category,sam,norm)
#g_ZZ_bk
                       elif 'edu_' in sam :  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+norm+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,sam,norm)
                       elif 'g_ZZ_bk' in sam :  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+norm+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,sam,norm)
#g_ZZ_bk
                       elif 'gH_SM' in sam :  
			  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/mc15_13TeV.342561.aMcAtNloHerwigppEvtGen_UEEE5_CTEQ6L1_CT10ME_ttH125_4l.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_ttH_'+norm+'.root','ttH_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi,category,'ttH',norm)
			  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+norm+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*value_reweight_factor_ggf,category,sam,norm)
#			  ROOT.add_ggH_histo_and_ttH(sam,n_bin,n_min,n_max,category,norm)
#                       I put here the SUM OF THE RED TO BE SURE THAT WE ARE DOING IT
                          ROOT.add_inputs_some_redu_bkg_no_bbh_tth_sum(n_bin,n_min,n_max,category,norm)
			  ROOT.add_inputs_bbH_sum(n_bin,n_min,n_max,category,norm)

                       elif 'Kagg' in sam :  
			  ROOT.create_inputs_normsyst_correct_lumi(RD_link+'mc/Systematics/'+syst+'/'+sample+'.root','tree_incl_all','outputs'+category+'/output_category_'+category+'_'+sam+'_'+norm+'.root',sam+'_BDT',discriminant1,discriminant2,n_bin,n_min,n_max,lumi*4*value_reweight_factor_ggf,category,sam,norm)
#			  ROOT.add_ggH_histo_and_ttH(sam,n_bin,n_min,n_max,category,norm)

