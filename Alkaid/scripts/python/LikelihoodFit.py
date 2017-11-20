import ROOT
import Utilities
from ROOT import gSystem
import argparse
from pdb import set_trace
from os.path import join

parser = argparse.ArgumentParser()
parser.add_argument(
    'wspath', default = "../../All_Input/workspace/")
parser.add_argument(
    '--couplingtype', default = "khvv")
parser.add_argument(
    '--smfix', action = 'store_true')
parser.add_argument(
    '--Asimov', action = 'store_true')
parser.add_argument(
    '--outpath','-o', default = "../../output/")
parser.add_argument(
    '--discriminant', default = "BDT_1bin")      
    
args = parser.parse_args()

ROOT.gROOT.SetBatch(ROOT.kTRUE)

gSystem.Load("../macros/LikelihoodFit/LikelihoodFit_cpp.so")

base_path = "../../All_Input/input_LLFit/"
#workspace_path = "../../All_Input/workspace/output_combined_datastat_model.root"


Utilities.try_makedir(args.outpath + args.discriminant)
if args.couplingtype == "khvv":
 l = ROOT.LikelihoodFitting_1D( base_path + 'systematics_fake.txt', base_path + 'coupling_name_khvv.txt', base_path + 'function_khvv.txt', base_path + 'function_ggf_SM_1_p6_1_m6.txt', join(args.wspath,"output_combined_datastat_model.root"),  "combined", "ModelConfig", "obsData", args.smfix, True, 5, 3, True, args.Asimov)
 l.DoScan(120,0.1, -6.,120, 0.1,-8., join(args.outpath,args.discriminant))
 del l
elif args.couplingtype == "kavv":
 l = ROOT.LikelihoodFitting_1D( base_path + 'systematics_fake.txt', base_path + 'coupling_name_kavv.txt', base_path + 'function_kavv.txt', base_path + 'function_ggf_SM_1_p6_1_m6.txt', join(args.wspath,"output_combined_datastat_model.root"),  "combined", "ModelConfig", "obsData", args.smfix, True, 5, 3, True, args.Asimov)
 l.DoScan(120,0.1, -6.,120, 0.1,-8., join(args.outpath,args.discriminant))
 del l