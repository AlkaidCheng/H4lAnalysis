import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from pdb import set_trace

class PlotUtil(SimpleSelection):

 def __init__(self, file,tree):
  SimpleSelection.__init__(self,file,tree)
  self.c = ROOT.TCanvas()
  
 def plot_1D_weighted(self,category ,param, weight,nbin,xmin,xmax,title = ""):
  param = "self.t.{0}".format(param)
  weight = "self.t.{0}".format(weight)
  h = ROOT.TH1D(title,title,nbin,xmin,xmax)
  for i in range(self.NEntries):
   self.t.GetEntry(i)
   if ((category is None) or (self.IsPassed(category))):
    h.Fill( eval(param),eval(weight))
  return h
 
 def format_plot(self, h, title = None, xtitle = "", ytitle = "", style = "", nostat = True, normalize = False, logy = False, save = True, output = "result.pdf"):
  self.c.SetLogy(logy)
  if normalize:
   h.Scale(1/abs(h.Integral()))
  if nostat:
   h.SetStats(0)
  if title is not None:
   h.SetTitle(title)
  h.GetXaxis().SetTitle(xtitle)
  h.GetYaxis().SetTitle(ytitle)
  h.GetYaxis().SetTitleOffset(1.4)
  h.Draw(style)
  if save:
   self.c.SaveAs(output)
 
  