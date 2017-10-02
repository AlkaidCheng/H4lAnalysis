from Minitree_H4l import Minitree_H4l
from pdb import set_trace

class WeightInfo:
 luminosity = 36.1
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
 reweight_factor = {
 "common":[1.35287, 1.53585, 1.42850, 1.32721, 1.01122, 1.04939, 1.36642, 1.32635, 1., 1., 1.31368, 1.94689],
 "ggf":[1.55827, 1.20101, 1.45916, 1.43005, 0.99916, 1.24639, 0.90243, 1.43910, 1., 1., 1.15480, 0.91722],
 "qqZZ":[1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.],
 "default":[1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.]}
 
 sample = {
 'mc15_13TeV.344973.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_yb2_ZZ4lep_noTau':['redu_bbH_1',"default"],
 'mc15_13TeV.344974.aMcAtNloPythia8EvtGen_A14NNPDF23LO_bbH125_ybyt_ZZ4lep_noTau':['redu_bbH_2',"default"],
 'mc15_13TeV.361621.Sherpa_CT10_WWZ_4l2v':['redu_WWZ',"default"],
 'mc15_13TeV.361623.Sherpa_CT10_WZZ_5l1v':['redu_WZZ',"default"],
 'mc15_13TeV.361625.Sherpa_CT10_ZZZ_6l0v':['redu_ZZZ_1',"default"],
 'mc15_13TeV.361626.Sherpa_CT10_ZZZ_4l2v':['redu_ZZZ_2',"default"],
 'mc15_13TeV.410218.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttee':['redu_ttz_1',"default"],
 'mc15_13TeV.410219.aMcAtNloPythia8EvtGen_MEN30NLO_A14N23LO_ttmumu':['redu_ttz_2',"default"],
 'mc15_13TeV.410144.Sherpa_NNPDF30NNLO_ttW':['redu_ttW',"default"],
 'mc15_13TeV.344158.aMcAtNloPythia8EvtGen_A14NNPDF23LO_ppx0_FxFx_Np012_SM':['ggH_SM','ggf'],
 'mc15_13TeV.344167.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAgg0p5_cosa0p707':['ggH_Khgg1_Kagg0p5','ggf'],
 'mc15_13TeV.344168.aMcAtNloPythia8EvtGen_A14NNPDF23LO_FxFx_kAggn0p5_cosa0p707':['ggH_Khgg1_Kaggm0p5','ggf'],
 'mc15_13TeV.345107.Sherpa_221_NNPDF30NNLO_llll_m4l100_300_filt100_150':['ZZllll_mll4_1','qqZZ'],
 'mc15_13TeV.363490.Sherpa_221_NNPDF30NNLO_llll':['ZZllll_mll4_2','qqZZ'],
 'mc15_13TeV.345108.Sherpa_221_NNPDF30NNLO_llll_m4l300':['ZZllll_mll4_3','qqZZ'],
 'mc15_13TeV.343247.MadGraphPythia8EvtGen_A14NNPDF23LO_vbfhzz4l_0p':['vbfMG5_0p_had','common'],
 'mc15_13TeV.344135.MGPy8EG_A14NNPDF23LO_vhlep125_4l_0p':['vbfMG5_0p_lep','common'],
 'mc15_13TeV.343212.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_2e2mu_13TeV':['gg_ZZ_bkg_2e2mu',"default"],
 'mc15_13TeV.343213.Powheggg2vvPythia8EvtGen_gg_ZZ_bkg_4l_noTau_13TeV':['gg_ZZ_bkg_4l_noTau',"default"],
 'data_run_2':['data_run_2']}

class SimpleSelection(Minitree_H4l):
 prod_type = {
 0:"0jet",
 1:"1jet",
 2:"2jet_VH",
 3:"2jet_VBF",
 4:"VHlep",
 5:"ttH"}
 
 prod_type_pt4l = {
 "1jet":{
 "1jet_lowpt4l":"*pt4l_fsr<=60.",
 "1jet_mediumpt4l":"60<*pt4l_fsr<=120",
 "1jet_highpt4l":"*pt4l_fsr>120"},
 "2jet_VH":{
 "2jet_VH_lowpt4l": "*pt4l_fsr<=150.",
 "2jet_VH_highpt4l":"*pt4l_fsr>150."},
 "2jet_VBF":{
 "2jet_VBF_lowptjet":"*pt4l_fsr<=200.",
 "2jet_VBF_highptjet":"*pt4l_fsr>200."}}
 
 prod_type_ptjet = {
 "2jet_VH":{
 "2jet_VH_lowptjet":"*leading_jet_pt<=200.",
 "2jet_VH_highptjet":"*leading_jet_pt>200."}}
 def __init__(self, file,tree):
  Minitree_H4l.__init__(self,file,tree)
  for category in self.prod_type_pt4l:
   for subcategory in self.prod_type_pt4l[category]:
    self.prod_type_pt4l[category][subcategory] = self.prod_type_pt4l[category][subcategory].replace("*","self.t.")
#  for category in self.prod_type_ptjet:
#   for subcategory in self.prod_type_ptjet[category]:
#    self.prod_type_ptjet[category][subcategory] = self.prod_type_ptjet[category][subcategory].replace("*","self.t.")
 def GetProd_Type(self):
  return self.prod_type[self.t.prod_type]
 def GetProd_Type_pt(self, prod_type):
  result = prod_type
  if prod_type in self.prod_type_pt4l:
   for category in self.prod_type_pt4l[prod_type]:
    if eval(self.prod_type_pt4l[prod_type][category]):
     result = category
     break
  return result
  
  
  
  
  
  
  
  
 '''
 "common":{
 "0jet":1.35287,
 "1jet_highpt4l":1.53585,
 "1jet_mediumpt4l":1.42850,
 "1jet_lowpt4l":1.32721,
 "2jet_VBF_highptjet":1.01122,
 "2jet_VBF_lowptjet":1.04939,
 "2jet_VH_highpt4l":1.36642,
 "2jet_VH_lowpt4l":1.32635,
 "2jet_VH_lowptjet":1.,
 "2jet_VH_highptjet":1.,
 "VHlep":1.31368,
 "ttH":1.94689},
 "ggf":{
 "0jet":1.55827,
 "1jet_highpt4l":1.20101,
 "1jet_mediumpt4l":1.45916,
 "1jet_lowpt4l":1.43005,
 "2jet_VBF_highptjet":0.99916,
 "2jet_VBF_lowptjet":1.24639,
 "2jet_VH_highpt4l":0.90243,
 "2jet_VH_lowpt4l":1.43910,
 "2jet_VH_lowptjet":1.,
 "2jet_VH_highptjet":1.,
 "VHlep":1.15480,
 "ttH":0.91722},
 "qqZZ":{
 "0jet":1.,
 "1jet_highpt4l":1.,
 "1jet_mediumpt4l":1.,
 "1jet_lowpt4l":1.,
 "2jet_VBF_highptjet":1.,
 "2jet_VBF_lowptjet":1.,
 "2jet_VH_highpt4l":1.,
 "2jet_VH_lowpt4l":1.,
 "2jet_VH_lowptjet":1.,
 "2jet_VH_highptjet":1.,
 "VHlep":1.,
 "ttH":1.},
 "default":{
 "0jet":1.,
 "1jet_highpt4l":1.,
 "1jet_mediumpt4l":1.,
 "1jet_lowpt4l":1.,
 "2jet_VBF_highptjet":1.,
 "2jet_VBF_lowptjet":1.,
 "2jet_VH_highpt4l":1.,
 "2jet_VH_lowpt4l":1.,
 "2jet_VH_lowptjet":1.,
 "2jet_VH_highptjet":1.,
 "VHlep":1.,
 "ttH":1.}
 '''
 
 