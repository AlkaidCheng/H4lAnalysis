import ROOT
import PlotUtil
import argparse
import Utilities
from pdb import set_trace
import os
from os import listdir
from os.path import join
ROOT.gROOT.SetBatch(True)

parser = argparse.ArgumentParser()
parser.add_argument(
    'path', default = "../../output/")
parser.add_argument(
    '--outpath', '-o', default = "../../output_LL/")        
    
args = parser.parse_args()



l = [i for i in listdir(args.path) if os.path.isdir(join(args.path,i))]
l = {i : listdir(join(args.path,i)) for i in l}
for discrim in l:
 L = [i for i in l[discrim] if ".root" in i]
 f_in = zip(sorted([i for i in L if "Asimov" in i]) , sorted([i for i in L if "Observed" in i]))
 for asi,obs in f_in:
  if 'khvv' in asi:
   higgs = 'H'
  elif 'kavv' in asi:
   higgs = 'A'
  if "fix" in asi:
   fix = "fix"
  else:
   fix = "floating"
  Utilities.try_makedir(join(args.outpath,discrim))

  PlotUtil.LLScanPlot(join(args.path,discrim,asi),join(args.path,discrim,obs), "SM Expected", "Observed", xtitle = "#kappa_{"+higgs+"}", ytitle = "-2ln(#lambda)" , SMfix = ("fix" in asi), higgs = higgs,  output = join(args.outpath,discrim,"{0}_{1}.pdf".format(higgs,fix)))