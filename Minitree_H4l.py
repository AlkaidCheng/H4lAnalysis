import ROOT
import PlotUtil
from collections import namedtuple
from pdb import set_trace
from array import array



def ImportROOTFilesFromText(text):
 FileStruct = namedtuple("File",['PATH', 'NAME', 'RAWNAME'])
 text = open(text)
 dir = text.readline().strip("#").rstrip()
 files = [f.rstrip() for f in text.readlines() if not f.startswith("#")]
 ROOTFiles = [FileStruct(os.join(dir,f),f.replace(".root",""),f) for f in files]
 return ROOTFiles
 
def ParsePlotInstructionFromText(text):
 text = open(text)
 instruction = {"PARAM":[]}
 for line in text.readlines():
  if line.startswith("#include"):
   items = line.replace("#include","").split("=",2)
   instruction[items[0].strip()] = items[-1].split()
  else if not line.startswith("#"):
   if "KEYS" in instruction:
    items = line.strip().split()
    instrction["PARAM"].append(dict(zip(instruction["KEYS"], items)))
   else:
    raise Exception("No Keys for Parameters Found")
 return instruction
 
def AddPrefixToChars(s,prefix = "Tree"):
 params = filter(bool, re.split("[^a-zA-Z]*", s))
 for param in params:
  s = s.replace(param,'{0}.{1}'.format(prefix,param))
 return s 
 
def ParsePrefixToDictValues(d,prefix = "Tree"):
 for key in d:
  d[key] = AddPrefixToChars(d[key],prefix)
 return d

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

 prod_type = {
 0:"0jet",
 1:"1jet",
 2:"2jet_VH",
 3:"2jet_VBF",
 4:"VHlep",
 5:"ttH"}
 
 category = {
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
 #Reweight factor accounting for higher order QCD corrections, e.g. from NLO to N3LO
 reweight_factor = {
 "common":[1.35287, 1.53585, 1.42850, 1.32721, 1.01122, 1.04939, 1.36642, 1.32635, 1., 1., 1.31368, 1.94689],
 "ggf":[1.55827, 1.20101, 1.45916, 1.43005, 0.99916, 1.24639, 0.90243, 1.43910, 1., 1., 1.15480, 0.91722],
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
  prod_type_pt4l = ParsePrefixToDictValues(prod_type_pt4l,'Tree')
  MiniTreeStruct = namedtuple("MiniTrees", ['Property','File','Tree','NEntries')
  self.Trees = {}
  for File in files:
   f = ROOT.TFile(File.PATH,"R")
   t = f.Get(tree)
   NEntries = t.GetEntriesFast()
   if NEntries == 0:
    raise Exception("ERROR: No Entries Found for tree {0} in file {1}".format(tree,File.PATH))
   else:
    self.Trees[f.NAME] = MiniTreesStruct(self.sample[f.NAME],f,t,NEntries)
  
 def GetCategories(self, Tree):
  Prod_Type = self.prod_type[Tree.prod_type]
  result.append(Prod_Type)
  for Category in self.category[Prod_Type]:
   if eval(self.category[prod_type][Category]):
    result.append(Category)
    break
  return result #result will include both the prod type and the category (if any)


 def luminosity_scaling(self,histograms,lumiscale):
  if len(histograms) == 1:
   histograms.Scale(self.luminosity*lumiscale)
  else if isinstance(histogram,dict):
   for h in histograms:
    histograms[h].Scale(self.luminosity*lumiscale)
  else if isinstance(histogram,list):
   for h in histograms:
    h.Scale(self.luminosity*lumiscale)

 def reweight_factor_scaling(self,histograms, reweight_type):
  if isinstance(histograms,dict):
   for category in histograms:
    histograms[category].Scale(self.reweight_factor[reweight_type][self.category_map[category]])
    
      
 def weighted_histograms(self , MiniTree, param, weight,nbin,xmin,xmax ,category = None, lumiscale = 1., reweight_type = None): 
  
  if param in self.optimised_binning:
   h_dict = {i:ROOT.TH1D("{0}_{1}".format(param,i),"{0}_{1}".format(title,i),len(self.optimised_binning[param])-1,self.optimised_binning[param]) for i in self.category_map.keys()} #This require python 2.7 or above
  else:
   h_dict = {i:ROOT.TH1D("{0}_{1}".format(param,i),"{0}_{1}".format(title,i),nbin,xmin,xmax) for i in self.category_map.keys()} #This require python 2.7 or above

  Tree = self.Trees[MiniTree].Tree
  param = ParseParameter(param,'Tree')
  weight = ParseParameter(weight,'Tree')
  
  for i in range(Tree.GetEntriesFast()):
   Tree.GetEntry(i)
   Categories = GetCategories(Tree) #Identify category of an event; i.e. product type & momentum range 
   if (category is None) or ((Categories[0] in category) or (Categories[-1] in category)):  #Filter unwanted categories
    histograms[Categories[-1]].Fill(eval(param),eval(weight))  #Do the weighting

  luminosity_scaling(histograms,lumiscale)
  if reweight_type is not None:
   reweight_factor_scaling(histograms,reweight_type) 

 def Get_Data_Driven_Distribution(h):
  for param in h:
   for category in h[param]:
     h[param][category] = PlotUtil.merge(h[param][category])
     normalization = h[param][category].Integral()
     DatDri_cnt = self.Data_Driven_Count[category]*self.luminosity
     if normalization != 0:
      for i in range(1,h[param][category].GetNbinsX()+1):
       cnt = h[param][category].GetBinContent(i)
       h[param][category].SetBinContent(i,DatDri_cnt*(cnt/normalization))
     else:
      print "No data driven event for category: {0}".format(category)
   h[param] = PlotUtil.merge(h[param])
  return h 