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
 "Uncertainty":"FillStyle=3244,FillColor=16"}

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


def format_plot(h, title = None, xtitle = "", ytitle = "", style = "HIST", nostat = True, normalize = False, logy = False, save = True, legend = None, output = "result.pdf"):
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
 if legend is not None:
  legend.Draw()
 if save:
  c.SaveAs(output)

class PlotClass(SimpleSelection,WeightInfo):

 def __init__(self, file,tree):
  SimpleSelection.__init__(self,file,tree)
  
 def plot_1D_weighted(self ,param, weight,nbin,xmin,xmax,title = "", category = None, reweight_type = None, data_driven = False, result_by_category = False): 
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
    h_dict[Prod_Type_pt].Fill(eval(param_new),eval(weight_new))  #Do the weight scaling 
  for category in h_dict:
   h_dict[category].Scale(self.luminosity)
   if reweight_type is not None:
    h_dict[category].Scale(self.reweight_factor[reweight_type][self.category_map[category]])
   if data_driven:
    count_data_driven = self.Data_Driven_Count[category]
    for i in range(nbin):
     count_old = h_dict[category].GetBinContent(i)
     count_data_driven_normalized = count_data_driven*(h_dict[category].GetBinWidth(i)/(xmax-xmin))
     h_dict[category].SetBinContent(i,count_old+count_data_driven)
  if result_by_category:
   return h_dict  
  else:
   for category in h_dict:
    h.Add(h_dict[category])
   return h
  
