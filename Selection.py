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

 
 