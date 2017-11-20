import ROOT
from collections import namedtuple
from pdb import set_trace
from os.path import join
from os import system
from array import array
from Utilities import *
import re

class Minitree_H4l:

 proc_type = {
 "sgn": set(["ggH","VBF","SM","ggH_BSM","ggH_Kaggm","ggH_Kagg","sgn","ttH","ggfMG5_0p","ggf",]),
 "sm":set(['ggH_SM','ggfMG5_0p','vbfMG5_0p_had','vh_0p_lep']),
 "kA":set(['ggfMG5_0phkAv1','ggfMG5_0phkAvm6','ggfMG5_0phkAv6',"vbfMG5_0pphmkAvm5_had",'vbfMG5_0pphmkAvm2p5_had','vbfMG5_0pphmkAv2p5_had','vbfMG5_0pphmkAv5_had','vh_0pphmkAvm5_lep','vh_0pphmkAvm2p5_lep','vh_0pphmkAv2p5_lep','vh_0pphmkAv5_lep']),
 "kH":set(['ggfMG5_0phkHv1','ggfMG5_0phkHvm6','ggfMG5_0phkHv6','vbfMG5_0pphkHvm5_had','vbfMG5_0pphkHvm2p5_had','vbfMG5_0pphkHv2p5_had','vbfMG5_0pphkHv5_had','vbf_0pphkHvm5_lep','vbf_0pphkHvm2p5_lep','vbf_0pphkHv2p5_lep','vbf_0pphkHv5_lep']),
 "bkg": set(["redu_bbH","redu_WWZ","redu_WZZ","redu_ZZZ","redu_ttZ","redu_ttW","ZZllll","gg_ZZ","bkg","redu"]),
 "datadriven": set(["data driven bkg","data driven"]),
 "dat": set(["data"])}
 
 data_driven_bkg = ["redu_bbH", "redu_WWZ","redu_WZZ", "redu_ttZ", "redu_ttW", "redu_ZZZ"]
 
 SamStruct = namedtuple("SamStruct", "proc_id proc Class reweight_type IsDataDriven")
 sample = {
##Start of Reducible Background Process
 'mc15_13TeV.344973.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_yb2_ZZ4lep_noTau':SamStruct('redu_bbH_1',"redu_bbH",set(["redu","bkg","redu_bbH","ALL"]),"default",True),
 'mc15_13TeV.344974.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_ybyt_ZZ4lep_noTau':SamStruct('redu_bbH_2',"redu_bbH",set(["redu","bkg","redu_bbH","ALL"]),"default",True),
 'mc15_13TeV.361621.Sherpa_CT10_WWZ_4l2v':SamStruct('redu_WWZ',"redu_WWZ",set(["redu","bkg","redu_WWZ","ALL"]),"default",True),
 'mc15_13TeV.361623.Sherpa_CT10_WZZ_5l1v':SamStruct('redu_WZZ',"redu_WZZ",set(["redu","bkg","redu_WZZ","ALL"]),"default",True),
 'mc15_13TeV.361625.Sherpa_CT10_ZZZ_6l0v':SamStruct('redu_ZZZ_1',"redu_ZZZ",set(["redu","bkg","redu_ZZZ","ALL"]),"default",True),
 'mc15_13TeV.361626.Sherpa_CT10_ZZZ_4l2v':SamStruct('redu_ZZZ_2',"redu_ZZZ",set(["redu","bkg","redu_ZZZ","ALL"]),"default",True),
 'mc15_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee':SamStruct('redu_ttZ_1',"redu_ttZ",set(["redu","bkg","redu_ttZ","ALL"]),"default",True),
 'mc15_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu':SamStruct('redu_ttZ_2',"redu_ttZ",set(["redu","bkg","redu_ttZ","ALL"]),"default",True),
 'mc15_13TeV.410144.Sherpa_NNPDF30NNLO_ttW':SamStruct('redu_ttW',"redu_ttW",set(["redu_ttW","bkg","redu_ttW","ALL"]),"default",True),
##Start of Non Reducible Background Process
 'mc15_13TeV.345107.Sherpa_221_NNPDF30NNLO_llll_m4l100_300_filt100_150':SamStruct('ZZllll_mll4_1',"ZZllll",set(["bkg","ZZllll","ALL"]),'qqZZ',False),
 'mc15_13TeV.363490.Sherpa_221_NNPDF30NNLO_llll':SamStruct('ZZllll_mll4_2',"ZZllll",set(["bkg","ZZllll","ALL"]),'qqZZ',False),
 'mc15_13TeV.345108.Sherpa_221_NNPDF30NNLO_llll_m4l300':SamStruct('ZZllll_mll4_3',"ZZllll",set(["bkg","ZZllll","ALL"]),'qqZZ',False), 
 'mc15_13TeV.343212.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_2e2mu_13TeV':SamStruct('ggZZ_2mu2e',"gg_ZZ",set(["bkg","gg_ZZ","ALL"]),"default",False),
 'mc15_13TeV.343213.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_4l_noTau_13TeV':SamStruct('ggZZ_4l',"gg_ZZ",set(["bkg","gg_ZZ","ALL"]),"default",False),
##Start of ggf Process 
 #ggf SM Process                                                                      //or ggf_0p
 'mc15_13TeV.344158.aMcAtNloPythia8EvtGen_A14NNPDF23LO_ppx0_FxFx_Np012_SM':SamStruct('ggfMG5_0p',"ggH_SM",set(["ggH","sgn","ggH_SM","ggH_kSM","ALL"]),'ggf',False),
 #ggf both Agg and Hgg coupling                                                                  
 'mc15_13TeV.344167.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAgg0p5_cosa0p707':SamStruct('ggH_Khgg1_Kagg0p5',"ggH_BSM",set(["ggH","sgn","ggH_BSM","ggH_kA_kH","ALL"]),'ggf4',False),
 'mc15_13TeV.344168.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAggn0p5_cosa0p707':SamStruct('ggH_Khgg1_Kaggm0p5',"ggH_BSM",set(["ggH","sgn","ggH_BSM","ggH_kA_kH","ALL"]),'ggf4',False),
 'mc15_13TeV.344169.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAgg1p41_cosa0p707':SamStruct('ggH_Khgg1_Kagg1p41',"ggH_BSM",set(["ggH","sgn","ggH_BSM","ggH_kA_kH","ALL"]),'ggf4',False),
 'mc15_13TeV.344170.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAgg1_cosa0p707':SamStruct('ggH_Khgg1_Kagg1',"ggH_BSM",set(["ggH","sgn","ggH_BSM","ggH_kA_kH","ALL"]),'ggf4',False),
 #ggf either Avv or Hvv coupling
 'mc15_13TeV.344159.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kHvv1_cosa1p0':SamStruct('ggfMG5_0phkHv1',"ggf",set(["ggf","ggf_kH","ggf_kAorH","ALL"]),'ggf',False),
 'mc15_13TeV.344161.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kHvvn6_cosa1p0':SamStruct('ggfMG5_0phkHvm6',"ggf",set(["ggf","ggf_kH","ggf_kAorH","ALL"]),'ggf',False),
 'mc15_13TeV.344160.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kHvv6_cosa1p0':SamStruct('ggfMG5_0phkHv6',"ggf",set(["ggf","ggf_kH","ggf_kAorH","ALL"]),'ggf',False),  
 'mc15_13TeV.344162.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAvv1_cosa0p707':SamStruct('ggfMG5_0phkAv1',"ggf",set(["ggf","ggf_kA","ggf_kAorH","ALL"]),'ggf4',False),
 'mc15_13TeV.344164.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAvvn6_cosa0p707':SamStruct('ggfMG5_0phkAvm6',"ggf",set(["ggf","ggf_kA","ggf_kAorH","ALL"]),'ggf4',False),
 'mc15_13TeV.344163.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAvv6_cosa0p707':SamStruct('ggfMG5_0phkAv6',"ggf",set(["ggf","ggf_kA","ggf_kAorH","ALL"]),'ggf4',False), 
 #ggf both Avv and Hvv coupling
 'mc15_13TeV.344166.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAHvvn6_cosa0p707':SamStruct('ggfMG5_0phkAvm6_khvm6',"ggf",set(["ggf","ggf_kA_kH","ALL"]),'ggf4',False),
 'mc15_13TeV.344165.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAHvv6_cosa0p707':SamStruct('ggfMG5_0phkAv6_khv6',"ggf",set(["ggf","ggf_kA_kH","ALL"]),'ggf4',False),  
##Start of VBF Process
 #VBF Only SM coupling
 'mc15_13TeV.343247.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0p':SamStruct('vbfMG5_0p_had',"VBF",set(["sgn","VBF","VBF_kSM","ALL"]),'vbf',False),
 #VBF Only Avv or Hvv coupling 
 'mc15_13TeV.343249.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0phkHv10':SamStruct('vbfMG5_0phkHv10_had',"VBF",set(["VBF","VBF_kH","VBF_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.343252.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0mkAv15':SamStruct('vbfMG5_0mkAv15_had',"VBF",set(["VBF","VBF_kA","VBF_kAorH","ALL"]),'vbf',False), #Some issue here
 #VBF SM + Either Avv or Hvv coupling
 'mc15_13TeV.343255.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkAvm5':SamStruct('vbfMG5_0pphmkAvm5_had',"VBF",set(["VBF","VBF_kSM_kA","VBF_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.343256.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkAvm2p5':SamStruct('vbfMG5_0pphmkAvm2p5_had',"VBF",set(["VBF","VBF_kSM_kA","VBF_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.343254.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkAv2p5':SamStruct('vbfMG5_0pphmkAv2p5_had',"VBF",set(["VBF","VBF_kSM_kA","VBF_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.343253.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkAv5':SamStruct('vbfMG5_0pphmkAv5_had',"VBF",set(["VBF","VBF_kSM_kA","VBF_kSM_kAorH","ALL"]),'vbf4',False), 
 'mc15_13TeV.343261.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphkHvm5':SamStruct('vbfMG5_0pphkHvm5_had',"VBF",set(["VBF","VBF_kSM_kH","VBF_kSM_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.343262.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphkHvm2p5':SamStruct('vbfMG5_0pphkHvm2p5_had',"VBF",set(["VBF","VBF_kSM_kH","VBF_kSM_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.343260.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphkHv2p5':SamStruct('vbfMG5_0pphkHv2p5_had',"VBF",set(["VBF","VBF_kSM_kH","VBF_kSM_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.343259.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphkHv5':SamStruct('vbfMG5_0pphkHv5_had',"VBF",set(["VBF","VBF_kSM_kH","VBF_kSM_kAorH","ALL"]),'vbf',False),
 #VBF Both SM and Avv and Hvv coupling
 'mc15_13TeV.344155.MGPy8EG_A14NNPDF23LO_vbfhzz4l_0pphmkHvm2p5kAvm5':SamStruct('vbfMG5_0pphm_khvm2p5_kAvm5_had',"VBF",set(["VBF","VBF_kSM_kA_kH","ALL"]),'vbf4',False), #Probable misnaming
 'mc15_13TeV.344154.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkHv5kAv6':SamStruct('vbfMG5_0pphm_khv5_kAv6_had',"VBF",set(["VBF","VBF_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344152.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkHv2p5kAvm5':SamStruct('vbfMG5_0pphm_khv2p5_kAvm5_had',"VBF",set(["VBF","VBF_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344157.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkHvm5kAvm6':SamStruct('vbfMG5_0pphm_khvm5_kAvm6_had',"VBF",set(["VBF","VBF_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344153.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkHv5kAv5':SamStruct('vbfMG5_0pphm_khv5_kAv5_had',"VBF",set(["VBF","VBF_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344156.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0pphmkHvm5kAv5':SamStruct('vbfMG5_0pphm_khvm5_kAv5_had',"VBF",set(["VBF","VBF_kSM_kA_kH","ALL"]),'vbf4',False),
##Start of VH Process
 #VH Only SM coupling 
 'mc15_13TeV.344135.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0p':SamStruct('vbfMG5_0p_lep',"VH",set(["sgn","VH","VH_kSM","ALL"]),'vbf',False),
 #VH Only Avv or Hvv coupling
 'mc15_13TeV.344145.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0mkAv15':SamStruct('vbfMG5_0mkAv15_lep',"VH",set(["VH","VH_kA","VH_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.344140.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0phkHv10':SamStruct('vbfMG5_0phkHv10_lep',"VH",set(["VH","VH_kA","VH_kAorH","ALL"]),'vbf',False),
 #VH SM + Either Avv or Hvv coupling
 'mc15_13TeV.344142.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkAvm5':SamStruct('vbfMG5_0pphmkAvm5_lep',"VH",set(["VH","VH_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.344144.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkAvm2p5':SamStruct('vbfMG5_0pphmkAvm2p5_lep',"VH",set(["VH","VH_kSM_kA","VH_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.344143.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkAv2p5':SamStruct('vbfMG5_0pphmkAv2p5_lep',"VH",set(["VH","VH_kSM_kA","VH_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.344141.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkAv5':SamStruct('vbfMG5_0pphmkAv5_lep',"VH",set(["VH","VH_kSM_kA","VH_kSM_kAorH","ALL"]),'vbf4',False),
 'mc15_13TeV.344137.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphkHvm5':SamStruct('vbfMG5_0pphkHvm5_lep',"VH",set(["VH","VH_kSM_kH","VH_kSM_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.344139.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphkHvm2p5':SamStruct('vbfMG5_0pphkHvm2p5_lep',"VH",set(["VH","VH_kSM_kH","VH_kSM_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.344138.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphkHv2p5':SamStruct('vbfMG5_0pphkHv2p5_lep',"VH",set(["VH","VH_kSM_kH","VH_kSM_kAorH","ALL"]),'vbf',False),
 'mc15_13TeV.344136.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphkHv5':SamStruct('vbfMG5_0pphkHv5_lep',"VH",set(["VH","VH_kSM_kH","VH_kSM_kAorH","ALL"]),'vbf',False),
 #VH Both SM and Avv and Hvv coupling
 'mc15_13TeV.344149.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHvm5kAvm6':SamStruct('vbfMG5_0pphm_khvm5_kAvm6_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344147.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHvm2p5kAvm5':SamStruct('vbfMG5_0pphm_khvm2p5_kAvm5_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344148.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHvm5kAv5':SamStruct('vbfMG5_0pphm_khvm5_kAv5_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344146.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHv2p5kAvm5':SamStruct('vbfMG5_0pphm_khv2p5_kAvm5_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344151.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHv5kAv5':SamStruct('vbfMG5_0pphm_khv5_kAv5_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.344150.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHv5kAv6':SamStruct('vbfMG5_0pphm_khv5_kAv6_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.345050.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHv5kAv10':SamStruct('vbfMG5_0pphm_khv5_kAv10_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.345051.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHvm10kAv10':SamStruct('vbfMG5_0pphm_khvm10_kAv10_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
 'mc15_13TeV.345052.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0pphmkHv10kAvm10':SamStruct('vbfMG5_0pphm_khv10_kAvm10_lep',"VH",set(["VH","VH_kSM_kA_kH","ALL"]),'vbf4',False),
##Start of ttH Process
 'mc15_13TeV.342561.aMcAtNloHerwigppEvtGen_UEEE5_CTEQ6L1_CT10ME_ttH125_4l':SamStruct('ttH',"ttH",set(["ttH","ALL"]),'default',False),
##Start of Observed Data
 'data_13TeV':SamStruct('data_run_2_obs','data',set(["dat","ALL"]),"default",False)}

 prod_type = {
 0:"0jet",
 1:"1jet",
 2:"2jet_VH",
 3:"2jet_VBF",
 4:"VHlep",
 5:"ttH"}
 
 proc_category = {
 "1jet":{
 "1jet_lowpt4l":"pt4l_fsr<=60.",
 "1jet_mediumpt4l":"60<pt4l_fsr<=120",
 "1jet_highpt4l":"pt4l_fsr>120"},
 "2jet_VH":{
 "2jet_VH_lowpt4l": "pt4l_fsr<=150.",
 "2jet_VH_highpt4l":"pt4l_fsr>150."},
 "2jet_VBF":{
 "2jet_VBF_lowptjet":"pt4l_fsr<=200.",
 "2jet_VBF_highptjet":"pt4l_fsr>200."}}
 
 #prod_type_ptjet = {
 #"2jet_VH":{
 #"2jet_VH_lowptjet":"leading_jet_pt<=200.",
 #"2jet_VH_highptjet":"leading_jet_pt>200."}}
 
 luminosity = 1 # Normalized to 36.1 pb-1 

 category_map = {
 "0jet":0,
 "1jet_highpt4l":1,
 "1jet_mediumpt4l":2,
 "1jet_lowpt4l":3,
 "2jet_VBF_highptjet":4,
 "2jet_VBF_lowptjet":5,
 "2jet_VH_highpt4l":6,
 "2jet_VH_lowpt4l":7,
 "2jet_VH_highptjet":8,
 "2jet_VH_lowptjet":9,
 "VHlep":10,
 "ttH":11}
 
 category_list_valid = ["0jet", "1jet_highpt4l", "1jet_mediumpt4l", "1jet_lowpt4l", "2jet_VBF_highptjet", "2jet_VBF_lowptjet", "2jet_VH_highpt4l", "2jet_VH_lowpt4l", "VHlep", "ttH"]

 category_type = {
 "0jet":set(["0jet","ALL"]),
 "1jet_highpt4l":set(["1jet_highpt4l","1jet","ALL"]),
 "1jet_mediumpt4l":set(["1jet_mediumpt4l","1jet","ALL"]),
 "1jet_lowpt4l":set(["1jet_lowpt4l","1jet","ALL"]),
 "2jet_VBF_highptjet":set(["2jet_VBF_highptjet","2jet_VBF","ALL"]),
 "2jet_VBF_lowptjet":set(["2jet_VBF_lowptjet","2jet_VBF","ALL"]),
 "2jet_VH_highpt4l":set(["2jet_VBF_highpt4l","2jet_VH","ALL"]),
 "2jet_VH_lowpt4l":set(["2jet_VBF_lowpt4l","2jet_VH","ALL"]),
 "2jet_VH_highptjet":set(["2jet_VH_highptjet","2jet_VH","ALL"]),
 "2jet_VH_lowptjet":set(["2jet_VH_lowptjet","2jet_VH","ALL"]),
 "VHlep":set(["VHlep","ALL"]),
 "ttH":set(["ttH","ALL"])}
 #Reweight factor accounting for higher order QCD corrections, e.g. from NLO to N3LO
 reweight_factor = {
 "vbf":[1.35287, 1.53585, 1.42850, 1.32721, 1.01122, 1.04939, 1.36642, 1.32635, 1., 1., 1.31368, 1.94689],
 "vbf4":[4*1.35287, 4*1.53585, 4*1.42850, 4*1.32721, 4*1.01122, 4*1.04939, 4*1.36642, 4*1.32635, 4*1., 4*1., 4*1.31368, 4*1.94689], #Accounting for the mixing angle of 45 degree in the similation for kAv
 "ggf":[1.55827, 1.20101, 1.45916, 1.43005, 0.99916, 1.24639, 0.90243, 1.43910, 1., 1., 1.15480, 0.91722],
 "ggf4":[4*1.55827, 4*1.20101, 4*1.45916, 4*1.43005, 4*0.99916, 4*1.24639, 4*0.90243, 4*1.43910, 4*1., 4*1., 4*1.15480, 4*0.91722], #Accounting for the mixing angle of 45 degree in the similation for kAv
 "qqZZ":[1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.],
 "default":[1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.]}
 
 Data_Driven_Count ={
 '0jet': 2.2372899055480957,
 '1jet_highpt4l': 0.087411001324653625,
 '1jet_lowpt4l': 0.5720289945602417,
 '1jet_mediumpt4l': 0.42000499367713928,
 '2jet_VBF_highptjet': 0.084611698985099792,
 '2jet_VBF_lowptjet': 0.4616599977016449,
 '2jet_VH_highpt4l': 0.016387799754738808,
 '2jet_VH_lowpt4l': 0.16335099935531616,
 '2jet_VH_highptjet':0.,
 '2jet_VH_lowptjet':0.,
 'VHlep': 0.0031107401009649038,
 'ttH': 0.031648699194192886}
 
 optimised_binning = {
 #"pt4l_fsr": array('d', [0,2.5,5,7.5,10,12.5,15,17.5,20,22.5,25,27.5,30,32.5,35,37.5,40,42.5,45,47.5,50,52.5,55,57.5,60,62.5,65,67.5,70,72.5,75,77.5,80,85,90,95,100,110,120,130,140,150,200,250,300,400,500])
 }
 
 
 def __init__(self, files,tree):
  for key in self.proc_category:
   self.proc_category[key] = ParsePrefixToDictValues(self.proc_category[key],'Tree')
  self.MiniTreeStruct = namedtuple("MiniTrees", ['Property','File','Tree','NEntries'])
  self.Trees = {}
  for File in files:
   f = ROOT.TFile(File.PATH,"R")
   t = f.Get(tree)
   try:
    NEntries = t.GetEntriesFast()
   except:
    set_trace()
   if NEntries == 0:
    raise Exception("ERROR: No Entries Found for tree {0} in file {1}".format(tree,File.PATH))
   else:
    self.Trees[File.NAME] = self.MiniTreeStruct(self.sample[File.NAME],f,t,NEntries)
  
 def GetCategories(self, Tree):
  Prod_Type = self.prod_type[Tree.prod_type]
  result = [Prod_Type]
  if Prod_Type in self.proc_category:
   for Category in self.proc_category[Prod_Type]:
    if eval(self.proc_category[Prod_Type][Category]):
     result.append(Category)
     break
  return result #result will include both the prod type and the category (if any)


 def luminosity_scaling(self,histograms,lumiscale):
  if (len(histograms) == 1) and (not isinstance(histograms,dict)) and (not isinstance(histograms,list)):
   histograms.Scale(self.luminosity*lumiscale)
  elif isinstance(histograms,dict):
   for h in histograms:
    histograms[h].Scale(self.luminosity*lumiscale)
  elif isinstance(histograms,list):
   for h in histograms:
    h.Scale(self.luminosity*lumiscale)

 def reweight_factor_scaling(self,histograms, reweight_type):
  if isinstance(histograms,dict):
   for category in histograms:
    histograms[category].Scale(self.reweight_factor[reweight_type][self.category_map[category]])
 
 def SimpleSelection(self, Tree, method = "default"):
  if method == "default": 
   return ((getattr(Tree,'pass_vtx4lCut')) and (118.<getattr(Tree,'m4l_constrained')<129.))
  else:
   return False
    
#Create Weighted Histograms for an Observable in various Categories      
 def weighted_histograms(self , Tree, param, weight,nbin,xmin,xmax ,category = ['ALL'], lumiscale = 1., reweight_type = None): 
  if param in self.optimised_binning:
   histograms = {i:ROOT.TH1D("","",len(self.optimised_binning[param])-1,self.optimised_binning[param]) for i in self.category_list_valid if not category.isdisjoint(self.category_type[i])} #This require python 2.7 or above
  else:
   histograms = {i:ROOT.TH1D("","",nbin,xmin,xmax) for i in self.category_list_valid if not category.isdisjoint(self.category_type[i])} #This require python 2.7 or above
  if CountParamInStr(param) == 1:  #Avoid using eval if only one parameter is considered
   if ('[' in param):
    param,index = filter(bool,re.split("[\[\]]*", param))
    index = int(index)
    for i in range(Tree.GetEntriesFast()):
     Tree.GetEntry(i)
     Categories = self.GetCategories(Tree) #Identify category of an event; i.e. product type & momentum range 
     if ('ALL' in category) or (not set(Categories).isdisjoint(category)):  #Filter unwanted categories
      if (self.SimpleSelection(Tree)):
       histograms[Categories[-1]].Fill(getattr(Tree,param)[index],getattr(Tree,weight))  #Do the weighting  
   else:
    for i in range(Tree.GetEntriesFast()):
     Tree.GetEntry(i)
     Categories = self.GetCategories(Tree) #Identify category of an event; i.e. product type & momentum range 
     if ('ALL' in category) or (not set(Categories).isdisjoint(category)):  #Filter unwanted categories
      if (self.SimpleSelection(Tree)):
       histograms[Categories[-1]].Fill(getattr(Tree,param),getattr(Tree,weight))  #Do the weighting
  else:
   param = AddPrefixToChars(param,'Tree')
   for i in range(Tree.GetEntriesFast()):
    Tree.GetEntry(i)
    Categories = self.GetCategories(Tree) #Identify category of an event; i.e. product type & momentum range 
    if ('ALL' in category) or (not set(Categories).isdisjoint(category)):  #Filter unwanted categories
     if (self.SimpleSelection(Tree)):
      histograms[Categories[-1]].Fill(eval(param),getattr(Tree,weight))  #Do the weighting
  self.luminosity_scaling(histograms,lumiscale)
  if reweight_type is not None:
   self.reweight_factor_scaling(histograms,reweight_type) 
  return histograms

#Form Distribution for Data Driven Background
 def Get_Data_Driven_Distribution(h):
  for category in h:
   normalization = h[category].Integral()
   DatDri_cnt = self.Data_Driven_Count[category]*self.luminosity
   if normalization != 0:
    for i in range(1,h[category].GetNbinsX()+1):
     cnt = h[category].GetBinContent(i)
     h[category].SetBinContent(i,DatDri_cnt*(cnt/normalization))
   else:
    print "No data driven event for category: {0}".format(category)
  return h 