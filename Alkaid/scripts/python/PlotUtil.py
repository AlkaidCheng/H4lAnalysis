import ROOT
import Utilities
from H4lAnalysis import Minitree_H4l
from collections import namedtuple
import os
from os.path import join
from os import system
import math
import re
from pdb import set_trace

class ColorWheel:
 color = {
 "bkg" : ["42","39","46","606","862","8","9","30","49","9"],
 "sgn" : ["4","2","3","6"],
 "def" : ["4","2","3","6","7","5","28","39","46","606","862"],
 "kH"  : ["4","2","5"],
 "kA"  : ["6","3","7"],
 "sm": ["1"],
 "datadriven": ["15"],
 "dat": ["1"]}
 def __init__(self):
  self.index = {}
  for key in self.color:
   self.index[key] = -1
 def next(self,proc_type):
  self.index[proc_type] = (self.index[proc_type] + 1) % len(self.color[proc_type])
  return self.color[proc_type][self.index[proc_type]]


Kinematics_Plot_Style = {
 "bkg":"LineColor=1,LineWidth=1,FillColor=ColorWheel",
 "sgn":"LineColor=ColorWheel,LineWidth=1,LineStyle=2,FillColor=0",
 "def":"LineColor=ColorWheel,LineWidth=1,LineStyle=2,FillColor=0",
 "kH":"LineColor=ColorWheel,LineWidth=1,LineStyle=3,FillColor=0",
 "kA":"LineColor=ColorWheel,LineWidth=1,LineStyle=7,FillColor=0",
 "sm":"LineColor=ColorWheel,LineWidth=2,LineStyle=1",
 "uncertainty":"FillStyle=3244,FillColor=16",
 "datadriven":"LineColor=1,LineWidth=1,FillColor=12",
 "dat":"MarkerStyle=8,LineColor=1,LineWidth=1"}
 
Kinematics_Legend_Style = {
 "bkg" : "f",
 "sgn" : "f",
 "datadriven": "f",
 "sm":"f",
 "kA":"f",
 "kH":"f",
 "def": "f",
 "dat" : "l"}

def Set(obj, **kwargs):
 for key, value in kwargs.iteritems():
  if value is None:
   getattr(obj, 'Set' + key)()
  elif isinstance(value, (list, tuple)):
   getattr(obj, 'Set' + key)(*value)
  else:
   getattr(obj, 'Set' + key)(value)

def GraphSet(obj, style):
 if ("=" in style):
  settings = {x.split('=')[0]: eval(x.split('=')[1]) for x in style.split(',')}
  Set(obj, **settings)
 
def LegendIni(style = "default",ncol = 1):
 leg = ROOT.TLegend(0.9-0.25*ncol,0.9 ,0.9,0.9)
 leg.SetTextFont(62)
 leg.SetTextSize(0.02)
 leg.SetNColumns(ncol)
 return leg
  
def LegendResize(leg,height = 0.03):
 leg.SetY1(leg.GetY1() - leg.GetNRows()*0.03)
 return leg


def SaveSummary(h,output):
 f = open(output,"w")
 if isinstance(h,dict):
  for key in h:
   f.write("{0}\t{1}\n".format(key,h[key].Integral()))
 f.close()

def stacked_kinematics_plot(h,title = "", xtitle = "", ytitle = "",canvasstyle = None, style = None, drawstyle = None,nostat = True, normalize = None,ymargin = 0.,save = True,output = "result.pdf", saveROOT = False):

 c = ROOT.TCanvas()
 c.cd()
 hs = ROOT.THStack(title,title)
 leg = LegendIni(ncol = int(math.ceil(len(h)/6.)))
 color = ColorWheel()
 h_data = None
 if normalize == "OVERALL":
  sum = 0
  for category in h:
   sum+=h[category].Integral()
  for category in h:
   h[category].Scale(1/abs(sum))
 elif normalize == "INDIVIDUAL":
  for category in h:
   h[category].Scale(1/h[category].Integral())
 for category in h:
 ########################################################################
  proc_type = "def"
  for proc in Minitree_H4l.proc_type:
   if category in Minitree_H4l.proc_type[proc]:
    proc_type = proc
 ########################################################################
  GraphSet(h[category],Kinematics_Plot_Style[proc_type].replace("ColorWheel",color.next(proc_type)))
  leg.AddEntry(h[category],category,Kinematics_Legend_Style[proc_type])
  if proc_type == "dat":
   h_data = h[category]
   continue  
  hs.Add(h[category])
 leg = LegendResize(leg)
 if canvasstyle is not None:
  GraphSet(c,canvasstyle)
 if style is not None:
  GraphSet(hs,style)

 if nostat and hasattr(hs,'SetStats'):
  hs.SetStats(0)
 hs.Draw("HIST" if drawstyle is None else drawstyle+" HIST")
 hs.GetXaxis().SetTitle(xtitle)
 hs.GetYaxis().SetTitle(ytitle)
 hs.GetYaxis().SetTitleOffset(1.4)
 if drawstyle == "NOSTACK":
  maximum = 0
  for category in h:
   if h[category].GetMaximum() > maximum:
    maximum = h[category].GetMaximum()
  hs.SetMaximum(maximum*(1+ymargin))
 else:
  hs.SetMaximum(hs.GetMaximum()*(1+ymargin))

 leg.Draw()
 if h_data is not None:
  h_data.Draw("SAME")
  hs.SetMaximum(h_data.GetMaximum()*(1+ymargin))
  
 Utilities.try_makedir(os.path.dirname(output))
 if save:
  c.SaveAs(output)
 if saveROOT:
  f = ROOT.TFile(output.replace(".pdf",".root"),"RECREATE")
  hs.Write()
  f.Close()
  


def kinematics_plot(h,title = "",xtitle = "", ytitle = "",canvasstyle = None,style = None, drawstyle = None,nostat = True, normalize = False,ymargin = 0.,save = True,output = "result.pdf", saveROOT = False):
 c = ROOT.TCanvas()
 c.cd()
 if canvasstyle is not None:
  GraphSet(c,style)
 if style is not None:
  GraphSet(h,style)
 if canvasstyle is not None:
  GraphSet(c,canvasstyle)
 if normalize:
  h.Scale(1/abs(h.Integral()))
 if nostat and hasattr(h,'SetStats'):
  h.SetStats(0)
 h.Draw("" if drawstyle is None else drawstyle)
 h.SetTitle(title)
 h.SetName(title)
 h.GetXaxis().SetTitle(xtitle)
 h.GetYaxis().SetTitle(ytitle)
 h.GetYaxis().SetTitleOffset(1.4)
 h.SetMaximum(h.GetMaximum()*(1+ymargin))
 Utilities.try_makedir(os.path.dirname(output))
 if save:
  c.SaveAs(output)
 if saveROOT:
  f = ROOT.TFile(output.replace(".pdf",".root"),"RECREATE")
  h.Write()
  f.Close()

def format_plot(h, title = None, xtitle = "", ytitle = "", style = "HIST", nostat = True, normalize = False, logy = False, save = True, legend = None, output = "result.pdf",h_same = None, ymargin = 0.):
 c = ROOT.TCanvas()
 if logy:
  c.SetLogy(logy)
 if normalize:
  h.Scale(1/abs(h.Integral()))
 if nostat and hasattr(h, 'SetStats'):
  h.SetStats(0)
 if title is not None:
  h.SetTitle(title)
 c.cd()
 h.Draw(style)
 h.GetXaxis().SetTitle(xtitle)
 h.GetYaxis().SetTitle(ytitle)
 h.GetYaxis().SetTitleOffset(1.4)
 h.SetMaximum(h.GetMaximum()*(1+ymargin))
 if h_same is not None:
  h_same.Draw("SAME")
 if legend is not None:
  legend.Draw()
 redrawBorder()
 Utilities.try_makedir(os.path.dirname(output))
 if save:
  c.SaveAs(output)
  
def merge(h):
 result = None
 if cmp_binning(h):
  h_iter = iter(h)
  if type(h) == type({}):
   result = h[next(h_iter)].Clone()
   for i in h_iter:
    result.Add(h[i])   
  elif type(h) == type([]):
   result = next(h_iter).Clone()
   for i in h_iter:
    result.Add(i)    
 else:
  print "ERROR: merging histograms with different binnings"
 return result
      
def merge_two_hist_by_keys(h1,h2):
 if (set(h1.keys()) == set(h2.keys())):
  for h in h1:
   h1[h].Add(h2[h])
  return h1
 else:
  raise Exception("Combining Dictionary of Histograms with Different Keys")
 return result

def cmp_binning(h):
 h_iter = iter(h)
 try:
  first = next(h_iter)
  if type(h) == type({}):
   Nbins = all(h[first].GetNbinsX() == h[rest].GetNbinsX() for rest in h_iter)
   h_iter = iter(h)
   Xmin = all(h[first].GetXaxis().GetXmin() == h[rest].GetXaxis().GetXmin() for rest in h_iter)
   h_iter = iter(h)
   if Nbins:
    Widths = all([all([h[first].GetBinWidth(j) == h[rest].GetBinWidth(j) for j in range(1,h[rest].GetNbinsX()+1)]) for rest in h_iter])
  elif type(h) == type([]):
   Nbins = all(first.GetNbinsX() == rest.GetNbinsX() for rest in h_iter)
   h_iter = iter(h)
   Xmin = all(first.GetXaxis().GetXmin() == rest.GetXaxis().GetXmin() for rest in h_iter)
   h_iter = iter(h)
   if Nbins:
    Widths = all([all([first.GetBinWidth(j) == rest.GetBinWidth(j) for j in range(1,rest.GetNbinsX()+1)]) for rest in h_iter])
  return all([Nbins,Xmin,Widths])
 except StopIteration:
  return False

  
def redrawBorder():
 #this little macro redraws the axis tick marks and the pad border lines.
 ROOT.gPad.Update()
 ROOT.gPad.RedrawAxis()

def CreateOneBinHistInRoot(NbinsX,Xmin,Xmax, Y, fout_name = None, hout_name = ""):
 h = ROOT.TH1D(hout_name,hout_name, NbinsX, Xmin,Xmax)
 h.Fill((Xmin+Xmax)/2,Y)
 if fout_name is not None:
  f = ROOT.TFile(fout_name,"RECREATE")
  h.Write()
  f.Close()
 return h

 
 
def AddTH1DFromRoot(samples, fout_name = None, hout_name = None):
 files = [ROOT.TFile(i) for i in samples if ".root" in i]
 h = []
 for f in files:
  h += [f.Get(i.GetName()) for i in  f.GetListOfKeys()]
 h = merge(h)
 if fout_name is not None:
  f = ROOT.TFile(fout_name,"RECREATE")
  if hout_name is not None:
   h.SetName(hout_name)
   h.SetTitle(hout_name)
  h.Write()
  f.Close()
 return h

def SaveHistToRoot(h, fout_name = "test.root", hout_name = "test"):
 f = ROOT.TFile(fout_name,"RECREATE")
 h.SetName(hout_name)
 h.SetTitle(hout_name)
 h.Write()
 f.Close()
 
def NormalizeHistFromRoot(f, norm, fout_name = None, hout_name = None):
 F = ROOT.TFile(f)
 h = F.Get(F.GetListOfKeys()[0].GetName())
 if (h.Integral() != 0):
  h.Scale(norm/h.Integral())
 if fout_name is not None:
  SaveHistToRoot(h,fout_name,hout_name)
 F.Close()
 return h
 
def SubtractTH1DFromRoot(f1, f2, fout_name = None, hout_name = None):
 F1 = ROOT.TFile(f1)
 F2 = ROOT.TFile(f2)
 h1 = F1.Get(F1.GetListOfKeys()[0].GetName())
 h2 = F2.Get(F2.GetListOfKeys()[0].GetName())
 h3 = h1.Clone()
 h3.Add(h2,-1)
 if fout_name is not None:
  f = ROOT.TFile(fout_name,"RECREATE")
  if hout_name is not None:
   h3.SetName(hout_name)
   h3.SetTitle(hout_name)
  h3.Write()
  f.Close()
 return h3

def LatexText(font, size):
 t = ROOT.TLatex()
 t.SetNDC()
 t.SetTextFont(font)
 t.SetTextSize(size)
 return t
 

def LLScanPlot( f_Asimov, f_Obs, leg_Asimov = "SM Expected", leg_Obs = "Observed",xtitle = "#kappa", ytitle = "-2ln(#lambda)", SMfix = True, higgs = "H", key = "Graph", output = "result.pdf"):
 
 c = ROOT.TCanvas("","", 800, 800)
 
 pad = ROOT.TPad("", "", 0., 0., 1., 1.)
 pad.cd()
 pad.SetFillStyle(4000)
 pad.SetTickx(1)
 pad.SetTicky(1)
 ROOT.SetOwnership(c, False)    #Prevent Corruption
 ROOT.SetOwnership(pad, False)  #Prevent Corruption
 f_asimov = ROOT.TFile(f_Asimov)
 f_obs = ROOT.TFile(f_Obs)
 g_asimov = f_asimov.Get(key)
 g_obs = f_obs.Get(key)
 g_asimov.SetLineColor(856)
 g_obs.SetLineColor(1)
 g_obs.SetFillColor(0)
 g_asimov.SetFillColor(0)
 g_asimov.SetLineWidth(2)
 g_obs.SetTitle("")
 g_asimov.SetTitle("")
 g_obs.SetLineWidth(2)
 g_asimov.SetLineStyle(7)
 g_obs.SetLineStyle(1)
 g_obs.SetMarkerColorAlpha(0,0)
 g_asimov.SetMarkerColorAlpha(0,0)
 Y_obs = list(g_obs.GetY())
 Y_asi = list(g_asimov.GetY())
 g_obs.SetMinimum(0)
 g_obs.SetMaximum(1.5*max(Y_obs))
 g_obs.GetXaxis().SetTitle(xtitle)
 g_obs.GetYaxis().SetTitle(ytitle)
 g_obs.Draw("")
 g_asimov.Draw("SAME")
 
 leg = ROOT.TLegend(0.65,0.85 ,0.85,0.75)
 leg.SetBorderSize(0)
 leg.SetTextFont(62)
 leg.AddEntry(g_asimov, leg_Asimov)
 leg.AddEntry(g_obs, leg_Obs)
 leg.Draw()
 pos_obs = Y_obs.index(min(Y_obs))
 pos_asi = Y_asi.index(min(Y_asi))
 t_obs = f_obs.Get("Likelihood")
 t_obs.GetEntry(pos_obs)
 t_asi = f_asimov.Get("Likelihood")
 t_asi.GetEntry(pos_asi)
 BSM_min_obs = t_obs.BSM_coupling
 SM_min_obs = t_obs.SM_coupling
 BSM_min_asi = t_asi.BSM_coupling
 SM_min_asi = t_asi.SM_coupling
 text1 = LatexText(73,20)
 text1.DrawLatex(0.15,0.85,"H#rightarrow ZZ* #rightarrow 4l")
 text2 = LatexText(73,18)
 text2.DrawLatex(0.15,0.81,"13 TeV, 36.1 fb^{-1}")
 text3 = LatexText(73,18)
 
 if SMfix is True:
  text3.DrawLatex(0.15,0.77,"#kappa_{Hgg}=1,#kappa_{SM} = 1") 
 else:
  text3.DrawLatex(0.15,0.77,"#kappa_{Hgg}=1,#kappa_{SM} = free")
 text4 = LatexText(73,18)
 text5 = LatexText(73,18)
 text4.DrawLatex(0.15,0.73, "Observed: #kappa_{"+higgs+"vv} = " +"{0:.1f}".format(BSM_min_obs) + ", #kappa_{SM} = " + "{0:.2f}".format(SM_min_obs))
 text5.DrawLatex(0.15,0.69, "Expected: #kappa_{"+higgs+"vv} = " +"{0:.1f}".format(BSM_min_asi) + ", #kappa_{SM} = " + "{0:.2f}".format(SM_min_asi))
 c.SaveAs(output)