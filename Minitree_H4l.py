import ROOT
from collections import namedtuple
from pdb import set_trace
from array import array

class Minitree_H4l:

 proc_type = {
 "sgn": ["ggH","VBF"],
 "bkg": ["redu_bbH","redu_WWZ","redu_WZZ","redu_ZZZ","redu_ttZ","redu_ttW","ZZllll","gg_ZZ"],
 "dat": "data"}
 
 data_driven_bkg = ["redu_bbH", "redu_WWZ","redu_WZZ", "redu_ttZ", "redu_ttW", "redu_ZZZ"]
 
 SamStruct = namedtuple("SamStruct", "proc_id proc reweight_type IsDataDriven")
 sample = {
 'mc15_13TeV.344973.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_yb2_ZZ4lep_noTau':SamStruct('redu_bbH_1',"redu_bbH","default",True),
 'mc15_13TeV.344974.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_ybyt_ZZ4lep_noTau':SamStruct('redu_bbH_2',"redu_bbH","default",True),
 'mc15_13TeV.361621.Sherpa_CT10_WWZ_4l2v':SamStruct('redu_WWZ',"redu_WWZ","default",True),
 'mc15_13TeV.361623.Sherpa_CT10_WZZ_5l1v':SamStruct('redu_WZZ',"redu_WZZ","default",True),
 'mc15_13TeV.361625.Sherpa_CT10_ZZZ_6l0v':SamStruct('redu_ZZZ_1',"redu_ZZZ","default",True),
 'mc15_13TeV.361626.Sherpa_CT10_ZZZ_4l2v':SamStruct('redu_ZZZ_2',"redu_ZZZ","default",True),
 'mc15_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee':SamStruct('redu_ttz_1',"redu_ttZ","default",True),
 'mc15_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu':SamStruct('redu_ttz_2',"redu_ttZ","default",True),
 'mc15_13TeV.410144.Sherpa_NNPDF30NNLO_ttW':SamStruct('redu_ttW',"redu_ttW","default",True),
 'mc15_13TeV.344158.aMcAtNloPythia8EvtGen_A14NNPDF23LO_ppx0_FxFx_Np012_SM':SamStruct('ggH_SM',"ggH",'ggf',False),
 'mc15_13TeV.344167.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAgg0p5_cosa0p707':SamStruct('ggH_Khgg1_Kagg0p5',"ggH",'ggf',False),
 'mc15_13TeV.344168.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAggn0p5_cosa0p707':SamStruct('ggH_Khgg1_Kaggm0p5',"ggH",'ggf',False),
 'mc15_13TeV.345107.Sherpa_221_NNPDF30NNLO_llll_m4l100_300_filt100_150':SamStruct('ZZllll_mll4_1',"ZZllll",'qqZZ',False),
 'mc15_13TeV.363490.Sherpa_221_NNPDF30NNLO_llll':SamStruct('ZZllll_mll4_2',"ZZllll",'qqZZ',False),
 'mc15_13TeV.345108.Sherpa_221_NNPDF30NNLO_llll_m4l300':SamStruct('ZZllll_mll4_3',"ZZllll",'qqZZ',False),
 'mc15_13TeV.343247.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0p':SamStruct('vbfMG5_0p_had',"VBF",'common',False),
 'mc15_13TeV.344135.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0p':SamStruct('vbfMG5_0p_lep',"VBF",'common',False),
 'mc15_13TeV.343212.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_2e2mu_13TeV':SamStruct('gg_ZZ_bkg_2e2mu',"gg_ZZ","default",False),
 'mc15_13TeV.343213.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_4l_noTau_13TeV':SamStruct('gg_ZZ_bkg_4l_noTau',"gg_ZZ","default",False),
 'data_13TeV':SamStruct('data','data',"default",False)}
 
 optimised_binning = {
 #"pt4l_fsr": array('d', [0,2.5,5,7.5,10,12.5,15,17.5,20,22.5,25,27.5,30,32.5,35,37.5,40,42.5,45,47.5,50,52.5,55,57.5,60,62.5,65,67.5,70,72.5,75,77.5,80,85,90,95,100,110,120,130,140,150,200,250,300,400,500])
 }
 def __init__(self, file,tree):
  self.f = ROOT.TFile(file,"READ")
  self.t = self.f.Get(tree)
  self.NEntries = self.t.GetEntriesFast()
  if self.NEntries == 0:
   raise Exception("ERROR: No Entries Found for tree {0}".format(tree))