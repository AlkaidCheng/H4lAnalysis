from Minitree_H4l import Minitree_H4l
from pdb import set_trace

class SimpleSelection(Minitree_H4l):
 criteria = {
 "0jet": "*prod_type == 0",
 "1jet": "*prod_type == 1",
 "2jet_VH": "*prod_type == 2",
 "2jet_VBF": "*prod_type == 3",
 "VHlep": "*prod_type == 4",
 "ttH": "*prod_type == 5",
 "2jet_merged": "*prod_type in [2,3]", 
 "1jet_lowpt4l": "(*prod_type == 1)and(*pt4l_fsr<=60.)",
 "ljet_mediumpt4l": "(*prod_type == 1)and(60<*pt4l_fsr<=120)",
 "ljet_highpt4l":"(*prod_type == 1)and(*pt4l_fsr>120)",
 "2jet_VH_lowpt4l":"(*prod_type == 2)and(*pt4l_fsr<=150.)",
 "2jet_VH_highpt4l":"(*prod_type == 2)and(*pt4l_fsr>150.)",
 "2jet_VH_lowptjet":"(*prod_type == 2)and(*pt4l_fsr<=200.)",
 "2jet_VH_highptjet":"(*prod_type == 2)and(*pt4l_fsr>200.)",
 "2jet_VBF_lowptjet":"(*prod_type == 3)and(*pt4l_fsr<=200.)",
 "2jet_VBF_highptjet":"(*prod_type == 3)and(*pt4l_fsr>200.)"}
 def __init__(self, file,tree):
  Minitree_H4l.__init__(self,file,tree)
  for category in self.criteria:
   self.criteria[category].replace("*","self.t.")
 def IsPassed(self,category):
  try:
   return eval(self.criteria[category])
  except:
   print "ERROR: No category named {0}".format(category)
 
 