import ROOT
from Minitree_H4l import Minitree_H4l
from Selection import SimpleSelection
from PlotUtil import *
import argparse
from pdb import set_trace
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)

parser = argparse.ArgumentParser()
parser.add_argument(
    'input',help="""input root file""")
parser.add_argument(
    '--param', default = "pt4l_fsr",help="""parameter to plot""")
parser.add_argument(
    '--weight', default = "weight",help="""weight for each event""")
parser.add_argument(
    '--nbin', default = 100,type = int,help="""number of bins""")
parser.add_argument(
    '--min', default = 0.,type = float,help="""x min""")
parser.add_argument(
    '--max', default = 400.,type = float,help="""x max""")
parser.add_argument(
    '--outdir', "-o",default = "./results",help="""Output directory""")  
args = parser.parse_args()

param = args.param
weight = args.weight
  
plot = PlotUtil(args.input,"tree_incl_all")
h = plot.plot_1D_weighted(None, param, weight ,args.nbin,args.min,args.max,title = "{0}_{1}".format(args.param,os.path.split(args.input)[1].replace(".root","")))
format_plot(h, xtitle = param, ytitle = "Event", output = join(outdir,h.GetTitle()))