import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from PlotUtil import *
import argparse
from pdb import set_trace
from os.path import join
ROOT.gROOT.SetBatch(ROOT.kTRUE)

parser = argparse.ArgumentParser()
parser.add_argument(
    'input',help="""input txt file containing the list of root files to be analyzed""")
parser.add_argument(
    '--param', "-p", default = "param.txt",help="""input txt file containing the list of parameters to be analyzed""")
parser.add_argument(
    '--outdir', "-o",default = "./results",help="""Output directory""")    
args = parser.parse_args()

RootIn = open(args.input)
dir = RootIn.readline().strip("#").rstrip()
files = [i.rstrip() for i in RootIn.readlines() if not i.startswith("#")]

ParamIn = open(args.param)
weight = ParamIn.readline().strip("#").rstrip()
params = [i.rstrip() for i in ParamIn.readlines() if not i.startswith("#")]
params = [i.split() for i in params]

outdir = args.outdir

for f in files:
 print join(dir,f)
 plot = PlotUtil(join(dir,f),"tree_incl_all")
 for elem in params:
  param = elem[0]
  nbin = eval(elem[1])
  xmin = eval(elem[2])
  xmax = eval(elem[3])
  h = plot.plot_1D_weighted(None, param, weight ,nbin,xmin,xmax,title = "{0}_{1}".format(param,f.replace(".root","")))
  plot.format_plot(h, xtitle = param, ytitle = "Event", output = join(outdir,h.GetTitle()+".pdf"))