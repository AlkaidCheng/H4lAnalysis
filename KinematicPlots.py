import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from PlotUtil import *
import argparse
from pdb import set_trace
from os.path import join
from os import system
ROOT.gROOT.SetBatch(ROOT.kTRUE)

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

for f in files:
 print join(dir,f)
 plot = PlotUtil(join(dir,f),"tree_incl_all")
 out_name = join(outdir,f.replace(".root","_weighted.root"))
 out_list.append(out_name)
 out = ROOT.TFile(out_name,"RECREATE")
 t = out.mkdir(f.strip(".root"))
 t.cd() 
 _reweight_type = None
 if args.reweight:
  _reweight_type = plot.sample[f.replace(".root","")][1]
 for elem in params:
  param = elem[0]
  nbin = eval(elem[1])
  xmin = eval(elem[2])
  xmax = eval(elem[3])
  h = plot.plot_1D_weighted(param, weight ,nbin,xmin,xmax,title = "{0}_{1}".format(param,f.replace(".root","")), reweight_type = _reweight_type)
  plot.format_plot(h, xtitle = param, ytitle = "Event", output = join(outdir,h.GetTitle()+".pdf"))
  out.Write()
 out.Close()
if args.combine:
 os.system("hadd combined.root {0}".format(" ".join(out_list)))