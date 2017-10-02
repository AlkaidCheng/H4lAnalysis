import ROOT
from pdb import set_trace

class Minitree_H4l:
 def __init__(self, file,tree):
  self.f = ROOT.TFile(file,"READ")
  self.t = self.f.Get(tree)
  self.NEntries = self.t.GetEntriesFast()
  if self.NEntries == 0:
   raise Exception("ERROR: No Entries Found for tree {0}".format(tree))