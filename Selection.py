from Minitree_H4l import Minitree_H4l
from pdb import set_trace

class WeightInfo:
 luminosity = 36.1
 reweight_factor = {
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
 "ttH":0.91722}
 }


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
 "ljet_highpt4l":"*pt4l_fsr>120"},
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
 
 