import ROOT
import os
import argparse
from os import listdir
from os.path import join
import PlotUtil
ROOT.gROOT.SetBatch(True)

# For use of python 2.7+ in lxplus, type:
# $setupATLAS
# $localSetupPython 2.7.3-x86_64-slc6-gcc47
# $localSetupROOT --rootVer=5.34.22-x86_64-slc6-gcc48-opt

parser = argparse.ArgumentParser()
parser.add_argument(
    'files',nargs='+',help="""Root Files Containing Histograms to Merge""")
parser.add_argument(
    '--method',default = "sum", help = """Method of merging. Default is summing all distributions""")
parser.add_argument(
    '--output','-o',default = "combined.root", help = """Output file name""")
parser.add_argument(
    '--hname',default = "combined", help = """Name of histogram stored in the output root file""")
args = parser.parse_args()

if args.method == "sum":
 PlotUtil.AddTH1DFromRoot(args.files,fout_name = args.output, hout_name = args.hname)
elif (args.method == "dif"):
 if (len(args.files) == 2):
  PlotUtil.SubtractTH1DFromRoot(args.files[0],args.files[1],fout_name = args.output, hout_name = args.hname)
 else:
  raise Exception("Exactly two histograms should be provided for taking difference. {0} provided.".format(len(args.files)))