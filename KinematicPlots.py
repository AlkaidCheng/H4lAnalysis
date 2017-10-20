import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from Selection import WeightInfo
import PlotUtil
from PlotUtil import PlotClass
import argparse
from pdb import set_trace
from os.path import join
from os import system
ROOT.gROOT.SetBatch(True)

# For use of python 2.7+ in lxplus, type:
# $setupATLAS
# $localSetupPython 2.7.3-x86_64-slc6-gcc47
# $localSetupROOT --rootVer=5.34.22-x86_64-slc6-gcc48-opt
parser = argparse.ArgumentParser()
parser.add_argument(
    'input',help="""input txt file containing the list of root files to be analyzed""")
parser.add_argument(
    '--param', "-p", default = "param.txt",help="""input txt file containing the list of parameters to be analyzed""")
parser.add_argument(
    '--outdir', "-o",default = "./results",help="""Output directory""")     
parser.add_argument(
    '--combine', action = "store_true",help="""Combine the output root files""")  
parser.add_argument(
    '--reweight', action = "store_true",help="""Include the reweight_factor in the weigiting""")   
parser.add_argument(
    '--stack', action = "store_true",help="""Include the reweight_factor in the weigiting""")
parser.add_argument(
    '--datadriven', action = "store_true",help="""Include data driven backgrounds""")
parser.add_argument(
    '--obs',action = "store_true",help="""Include observed data in the analysis""")   
parser.add_argument(
    '--lumiscale',default = 1.,help="""Luminosity scale""")  
args = parser.parse_args()

RootIn = open(args.input)
dir = RootIn.readline().strip("#").rstrip()
outdir = args.outdir
files = [i.rstrip() for i in RootIn.readlines() if not i.startswith("#")]

ParamIn = open(args.param)
weight = ParamIn.readline().strip("#").rstrip()
params = [i.rstrip() for i in ParamIn.readlines() if not i.startswith("#")]
params = [i.split() for i in params]

outdir = args.outdir

out_list = []

if args.stack:
 hs = {elem[0]:ROOT.THStack("hs","") for elem in params}  #Template for the stacked distribution
 h_proc = {elem[0]:{} for elem in params}    # Dictionary containing distribution for each parameter for each process
 h_data_driven = {elem[0]:{category:[] for category in WeightInfo.category_map.keys()} for elem in params}  #Template for the data driven distribution 
 logys = {elem[0]:("logy" in elem) for elem in params}
 plots = []
 for f in files:
  print join(dir,f)
  sample = f.strip(".root")   
  plots.append(PlotClass(join(dir,f),"tree_incl_all"))
  reweight_type = None if not args.reweight else Minitree_H4l.sample[sample].reweight_type
  for elem in params:
   param = elem[0]
   nbin, xmin, xmax = map(eval,elem[1:4])
   proc = plots[-1].sample[sample].proc  # Identify the type of process the Minitree belongs to
   h, h_by_category = plots[-1].plot_1D_weighted(param, weight ,nbin,xmin,xmax,title = "{0}_{1}".format(param,sample), reweight_type = reweight_type,lumiscale = args.lumiscale)
   if proc not in h_proc[param]:
    h_proc[param][proc] = h
   else:
     h_proc[param][proc].Add(h)
   if args.datadriven:
    for category in h_data_driven[param]:
     if (proc in Minitree_H4l.data_driven_bkg):
      h_data_driven[param][category].append(h_by_category[category])
      
 if args.datadriven:
  h_data_driven = PlotUtil.PlotClass.data_driven_dist(h_data_driven)
  
 for param in h_proc:
  leg = ROOT.TLegend(0.75,0.9 - 0.03*len(h_proc[param]),0.9,0.9)
  leg.SetTextFont(62)
  leg.SetTextSize(0.02)
  Color = PlotUtil.ColorWheel()
  f = open(join(outdir,"{0}_statistics.txt".format(param)),"w")

  if args.datadriven:    #Adding the data driven background distribution to the plot
   f.write("{0}\t{1}\n".format("data driven background",h_data_driven[param].Integral()))
   PlotUtil.GraphSet(h_data_driven[param],PlotUtil.style["data_driven"])
   hs[param].Add(h_data_driven[param])
   leg.AddEntry(h_data_driven[param],"data driven bkg","f")
  
  proc_bkg = [i for i in h_proc[param] if i in Minitree_H4l.proc_type["bkg"]]
  proc_sgn = [i for i in h_proc[param] if i in Minitree_H4l.proc_type["sgn"]]
  for proc in proc_bkg:
   f.write("{0}\t{1}\n".format(proc,h_proc[param][proc].Integral()))
   PlotUtil.GraphSet(h_proc[param][proc],PlotUtil.style["bkg"].replace("Fill_Color",Color.next_bkg()))
   leg.AddEntry(h_proc[param][proc],proc,"f")
   hs[param].Add(h_proc[param][proc])
  
  for proc in proc_sgn:
   f.write("{0}\t{1}\n".format(proc,h_proc[param][proc].Integral()))
   PlotUtil.GraphSet(h_proc[param][proc],PlotUtil.style["sgn"].replace("Line_Color",Color.next_bkg()))
   leg.AddEntry(h_proc[param][proc],proc,"f")
   hs[param].Add(h_proc[param][proc])
  
  h_dat = None
  if args.obs and (Minitree_H4l.proc_type["dat"] in h_proc[param]):
   h_dat = h_proc[param][Minitree_H4l.proc_type["dat"]]
   f.write("{0}\t{1}\n".format("data",h_dat.Integral()))
   PlotUtil.GraphSet(h_dat,PlotUtil.style["dat"])
   leg.AddEntry(h_dat,"obs","l")  

   
  PlotUtil.format_plot(hs[param], xtitle = param, ytitle = "Event",output = join(outdir,"{0}_Stacked.pdf".format(param)),legend = leg,logy=logys[param],h_same = h_dat, ymargin = 0.3)
  
   
else:
 for f in files:  
  print join(dir,f)
  sample = f.strip(".root")   #Name of the Minitree without extension
  plot = PlotClass(join(dir,f),"tree_incl_all")
  out_name = join(outdir,f.replace(".root","_weighted.root"))
  out_list.append(out_name)
  out = ROOT.TFile(out_name,"RECREATE")
  t = out.mkdir(f.strip(".root"))
  t.cd() 
  reweight_type = None if not args.reweight else Minitree_H4l.sample[sample].reweight_type
  for elem in params:
   param = elem[0]
   nbin, xmin, xmax = map(eval,elem[1:4])
   data_driven = Minitree_H4l.sample[sample].IsDataDriven
   h = plot.plot_1D_weighted(param, weight ,nbin,xmin,xmax,title = "{0}_{1}".format(param,sample), reweight_type = reweight_type,lumiscale = args.lumiscale)
   PlotUtil.format_plot(h, xtitle = param, ytitle = "Event", output = join(outdir,h.GetTitle()+".pdf"))
   out.Write()
  out.Close()
 if args.combine:
  os.system("hadd combined.root {0}".format(" ".join(out_list)))
