import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from Selection import WeightInfo
from pdb import set_trace

class ColorWheel:
 bkg = ["42","39","46","606","862","8","9","30","49","9"]
 sgn = ["4","2","3","6"]
 def __init__(self):
  self.sgn_index = -1
  self.bkg_index = -1
 def next_sgn(self):
  self.sgn_index = (self.sgn_index + 1) % len(self.sgn)
  return self.sgn[self.sgn_index]
 def next_bkg(self):
  self.bkg_index = (self.bkg_index + 1) % len(self.bkg)
  return self.bkg[self.bkg_index]


style = {
 "bkg":"LineColor=1,LineWidth=1,FillColor=Fill_Color",
 "sgn":"LineColor=Line_Color,LineWidth=1,LineStyle=2,FillColor=0",
 "Uncertainty":"FillStyle=3244,FillColor=16",
 "data_driven":"LineColor=1,LineWidth=1,FillColor=12",
 "dat":"MarkerStyle=8,LineColor=1,LineWidth=1"}

def Set(obj, **kwargs):
 for key, value in kwargs.iteritems():
  if value is None:
   getattr(obj, 'Set' + key)()
  elif isinstance(value, (list, tuple)):
   getattr(obj, 'Set' + key)(*value)
  else:
   getattr(obj, 'Set' + key)(value)

def GraphSet(obj, style):
 settings = {x.split('=')[0]: eval(x.split('=')[1]) for x in style.split(',')}
 Set(obj, **settings)


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

class PlotClass(SimpleSelection,WeightInfo):

 def __init__(self, file,tree):
  SimpleSelection.__init__(self,file,tree)
  
 def plot_1D_weighted(self ,param, weight,nbin,xmin,xmax,title = "", lumiscale = 1., category = None, reweight_type = None): 
  kwargs = ""
  if param in self.optimised_binning:
   h_dict = {i:ROOT.TH1D("{0}_{1}".format(param,i),"{0}_{1}".format(title,i),len(self.optimised_binning[param])-1,self.optimised_binning[param]) for i in self.category_map.keys()} #This require python 2.7 or above
   h = ROOT.TH1D(param,title,len(self.optimised_binning[param])-1,self.optimised_binning[param])
  else:
   h_dict = {i:ROOT.TH1D("{0}_{1}".format(param,i),"{0}_{1}".format(title,i),nbin,xmin,xmax) for i in self.category_map.keys()} #This require python 2.7 or above
   h = ROOT.TH1D(param,title,nbin,xmin,xmax)

  param_new = "self.t.{0}".format(param)
  weight_new = "self.t.{0}".format(weight)
  for i in range(self.NEntries):
   self.t.GetEntry(i)
   Prod_Type = self.GetProd_Type()
   Prod_Type_pt = self.GetProd_Type_pt(Prod_Type)    #Obtain the category of the event, i.e. Prod_Type_pt == category
   if (category is None) or ((category is not None) and ((Prod_Type in category) or Prod_Type_pt in category)):  #Filter unwanted category
    h_dict[Prod_Type_pt].Fill(eval(param_new),eval(weight_new))  #Do the weighting 
  for category in h_dict:
   h_dict[category].Scale(self.luminosity*lumiscale)
   if reweight_type is not None:
    h_dict[category].Scale(self.reweight_factor[reweight_type][self.category_map[category]])
  return merge(h_dict),h_dict  #Return both combined distribution and distribution by category

 @staticmethod
 def data_driven_dist(h):
  for param in h:
   for category in h[param]:
     h[param][category] = merge(h[param][category])
     normalization = h[param][category].Integral()
     DatDri_cnt = PlotClass.Data_Driven_Count[category]*PlotClass.luminosity
     if normalization != 0:
      for i in range(1,h[param][category].GetNbinsX()+1):
       cnt = h[param][category].GetBinContent(i)
       h[param][category].SetBinContent(i,DatDri_cnt*(cnt/normalization))
     else:
      print "No data driven event for category: {0}".format(category)
   h[param] = merge(h[param])
  return h 
