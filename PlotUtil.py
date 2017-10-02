import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from Selection import WeightInfo
from pdb import set_trace

class PlotUtil(SimpleSelection,WeightInfo):

 def __init__(self, file,tree):
  SimpleSelection.__init__(self,file,tree)
  self.c = ROOT.TCanvas()
  
 def plot_1D_weighted(self ,param, weight,nbin,xmin,xmax,title = "", category = None, reweight_type = None):
  h = ROOT.TH1D(param,title,nbin,xmin,xmax)  
  param = "self.t.{0}".format(param)
  weight = "self.t.{0}".format(weight)
  for i in range(self.NEntries):
   self.t.GetEntry(i)
   Prod_Type = self.GetProd_Type()
   Prod_Type_pt = self.GetProd_Type_pt(Prod_Type)
   if (category is None) or ((category is not None) and ((Prod_Type in category) or Prod_Type_pt in category)):
    if reweight_type is None:
     h.Fill(eval(param),self.luminosity*eval(weight)) 
    else:
     h.Fill(eval(param),self.luminosity*self.reweight_factor[reweight_type][self.category_map[Prod_Type_pt]]*eval(weight))
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
 
  