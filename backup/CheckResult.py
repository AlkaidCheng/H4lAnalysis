import ROOT
import os
import argparse
from os import listdir
from os.path import join
ROOT.gROOT.SetBatch(True)

# For use of python 2.7+ in lxplus, type:
# $setupATLAS
# $localSetupPython 2.7.3-x86_64-slc6-gcc47
# $localSetupROOT --rootVer=5.34.22-x86_64-slc6-gcc48-opt

parser = argparse.ArgumentParser()
parser.add_argument(
    'dir1',help="""First directory containing the root files to be compared""")
parser.add_argument(
    'dir2', help="""Second directory containing the root files to be comparedd""")  
parser.add_argument(
    '--tolerance', '-t', default = 0.03,help="""Maximum relative error allowed""")      
parser.add_argument(
    '--allbin', '-a', action = "store_true",help="""Check every bin content""")  
args = parser.parse_args()

f1 = [i for i in listdir(args.dir1) if ".root" in i]
f2 = [i for i in listdir(args.dir2) if ".root" in i]
f = set(f1) & set(f2)

print "Total matched root files: {0}".format(len(f))

f1 = [ROOT.TFile(join(args.dir1,i)) for i in f]
f2 = [ROOT.TFile(join(args.dir2,i)) for i in f]

success = 0
failed = 0
hist_count = 0

for a,b in zip(f1,f2):
 hist_a = [a.Get(i.GetName()) for i in a.GetListOfKeys()]
 hist_b = [b.Get(i.GetName()) for i in b.GetListOfKeys()]
 if len(hist_a)!=len(hist_b):
  raise Exception("ERROR: Comparing root files with different number of histograms!")  
 hist_count += len(hist_a)
 if not args.allbin:
  for ha,hb in zip(hist_a,hist_b):
   count_a = ha.Integral()
   count_b = hb.Integral()
   message = ""
   if abs((count_a-count_b)/count_a)<args.tolerance:
    success += 1
   else:
    failed += 1
    message = "NOT "
   print "{0}Matched Histograms: {1}".format(message,ha.GetName())
   print "Count for Input1 vs Input2 : {0} vs {1}".format(count_a,count_b)
 else:
  for ha,hb in zip(hist_a,hist_b):
   if (ha.GetNbinsX())!=(hb.GetNbinsX()):
    raise Exception("ERROR: Comparing histograms with different binning")
   else:
    count_a = ha.Integral()
    count_b = hb.Integral()
    Nbin = ha.GetNbinsX()
    error = 0
    for i in range(1,Nbin+1):
     error += abs((ha.GetBinContent(i) - hb.GetBinContent(i))/ ha.GetBinContent(i))
    error /= Nbin
    message = ""
    if error < args.tolerance:
     sucess += 1
    else:
     failed += 1
     message += "NOT"
    print "{0}Matched Histograms: {1}".format(message,ha.GetName())
    print "Count for Input1 vs Input2 : {0} vs {1}".format(count_a,count_b)
    
    

print "Total Matched Histograms: {0}/{1}".format(success,hist_count)
print "Total NOT Matched Histograms: {0}/{1}".format(failed,hist_count)
    
      
  
 