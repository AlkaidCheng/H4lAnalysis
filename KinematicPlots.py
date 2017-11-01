import ROOT
import H4lAnalysis
from H4lAnalysis import Minitree_H4l
import PlotUtil
import argparse
from pdb import set_trace
from os.path import join
from os import system
from collections import OrderedDict
ROOT.gROOT.SetBatch(True)

# For use of python 2.7+ in lxplus, type:
# $setupATLAS
# $localSetupPython 2.7.3-x86_64-slc6-gcc47
# $localSetupROOT --rootVer=5.34.22-x86_64-slc6-gcc48-opt
parser = argparse.ArgumentParser()
parser.add_argument(
    'input',help="""input txt file containing the list of root files to be analyzed""")
parser.add_argument(
    'param', default = "param.txt",help="""input txt file containing the list of parameters to be analyzed""")
parser.add_argument(
    '--outdir', "-o",default = "./results",help="""Output directory""")       
args = parser.parse_args()


outdir = args.outdir
MiniTrees = Minitree_H4l(H4lAnalysis.ImportROOTFilesFromText(args.input),"tree_incl_all")
Work = H4lAnalysis.ParsePlotInstructionFromText(args.param)

for param in Work["PARAM"]:
#Gather Distributions
 print "Gathering Distributions for the variable {0}".format(param["NAME"])
 MINITREES = set(param["MINITREES"].split(","))
 CATEGORY = set(param["CATEGORY"].split(","))
 histograms = {}    # Dictionary containing distribution for each process
 for MiniTree in MiniTrees.Trees.values():
  proc = MINITREES & MiniTree.Property.Class
  if len(proc) == 1:  
   print "Accessing Minitrees {0}".format(MiniTree.File.GetName())
   proc = list(proc)[0]
   if proc == 'ALL':
    proc = MiniTree.Property.proc
   if "NOMERGE" in Work["SETTING"]:
    proc = MiniTree.Property.proc_id
   reweight_type = MiniTree.Property.reweight_type if "REWEIGHT" in Work["SETTING"] else None
   h = MiniTrees.weighted_histograms(MiniTree.Tree,param["NAME"],Work["WEIGHT"],param["NBIN"],param["MIN"],param["MAX"],category = CATEGORY,lumiscale = Work["LUMISCALE"], reweight_type = reweight_type)
   if proc not in histograms:
    histograms[proc] = h
   else:
    histograms[proc] = PlotUtil.merge_two_hist_by_keys(histograms[proc],h)
   if ("DATADRIVEN" in Work["SETTING"]) and (MiniTree.Property.IsDataDriven):
    if 'data driven bkg' not in histograms:
     histograms['data driven bkg'] = h
    else:
     histograms['data driven bkg'] = PlotUtil.merge_two_hist_by_keys(histograms[proc],h) 
  elif len(proc)>1:
   raise Exception("Not Mutually Exclusive MiniTrees: {0}".format(param["MINITREES"]))

 MajorName = param["NAME"]
 if "FULLNAME" in Work["SETTING"]:
  MajorName = "{0}_Process_{1}_Category_{2}".format(param["NAME"],param["MINITREES"].replace(",","-"),param["CATEGORY"].replace(",","-")) 
 if "ORDERED" in Work["SETTING"]:
  histograms = OrderedDict([[i,histograms[i]] for i in sorted(histograms.iterkeys())])
#Start Plotting 
 if not "BYCATEGORY" in Work["SETTING"]:
  for h in histograms:
   histograms[h] = PlotUtil.merge(histograms[h])
  if "SUMMARY" in Work["SETTING"]:
   PlotUtil.SaveSummary(histograms,join(outdir,"{0}_statistics.txt".format(MajorName)))
  if "STACK" in Work["SETTING"]:
   PlotUtil.stacked_kinematics_plot(histograms, xtitle = param["NAME"],ytitle = "Event", canvasstyle = param["CANVASSTYLE"],normalize = param["NORMALIZATION"],style = param["STYLE"], drawstyle = param["DRAWSTYLE"],output = join(outdir,"{0}_Stacked.pdf".format(MajorName)), ymargin = 0.35)
  else:
   for h in histograms:
    PlotUtil.kinematics_plot(histograms[h],xtitle = param["NAME"],ytitle = "Event", canvasstyle = param["CANVASSTYLE"],normalize = param["NORMALIZATION"],style = param["STYLE"], drawstyle = param["DRAWSTYLE"],output = join(outdir,"{0}_{1}.pdf".format(MajorName,h)), ymargin = 0.35)
 else:
  histograms_by_category = {}
  for h in histograms:
   for category in histograms[h]:
    if category not in histograms_by_category:
      histograms_by_category[category] = {}
    histograms_by_category[category][h] = histograms[h][category]
  for category in histograms_by_category:
   if "SUMMARY" in Work["SETTING"]:
    PlotUtil.SaveSummary(histograms_by_category[category],join(outdir,"{0}_{1}_statistics.txt".format(MajorName,category)))
   if "STACK" in Work["SETTING"]:
    PlotUtil.stacked_kinematics_plot(histograms_by_category[category], xtitle = param["NAME"],ytitle = "Event", canvasstyle = param["CANVASSTYLE"],normalize = param["NORMALIZATION"],style = param["STYLE"], drawstyle = param["DRAWSTYLE"],output = join(outdir,"{0}_{1}_Stacked.pdf".format(MajorName,category)), ymargin = 0.35)
   else:
    for h in histograms_by_category[category]:
     PlotUtil.kinematics_plot(histograms[category][h],xtitle = param["NAME"],ytitle = "Event", canvasstyle = param["CANVASSTYLE"],normalize = param["NORMALIZATION"],style = param["STYLE"], drawstyle = param["DRAWSTYLE"],output = join(outdir,"{0}_{1}_{2}.pdf".format(MajorName,category,h)), ymargin = 0.35)