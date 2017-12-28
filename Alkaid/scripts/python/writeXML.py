import ROOT
import Utilities
from ROOT import gSystem
import argparse
from pdb import set_trace
import os
from os.path import join

parser = argparse.ArgumentParser()
parser.add_argument(
    '--err', default = "")
parser.add_argument(
    '--sample', default = "../../All_Input/input_xml/samples_BSM_khvv_only.txt")
parser.add_argument(
    '--category', default = "../../All_Input/input_xml/category.txt")    
parser.add_argument(
    '--norm', default = "../../All_Input/input_xml/norm_factor_EFT_1D.txt")    
parser.add_argument(
    '--syst', default = "../../All_Input/input_xml/systematics_fake.txt")       
parser.add_argument(
    '--jetptcut', default = "35")
parser.add_argument(
    '--discriminant', default = "BDT_1bin")
parser.add_argument(
    '--outpath','-o', default = "../../All_Input/workspace_xml/")        
parser.add_argument(
    '--Asimov', action = "store_true")
parser.add_argument(
    '--dtd', default = "../../All_Input/input_xml/HistFactorySchema.dtd")
parser.add_argument(
    '--wspath', default = "../../All_Input/workspace/")
parser.add_argument(
    '--mode', default = "vv")        
    
args = parser.parse_args()

ROOT.gROOT.SetBatch(ROOT.kTRUE)

gSystem.Load("../macros/writeXML/writeXML_cpp.so")

Utilities.try_makedir(join(args.outpath,args.discriminant))
Utilities.try_makedir(join(args.wspath,args.discriminant))
#ROOT.H4l_XML.writeXML(ErrorType = "THEO SYS EXP", sample_in = XML_PATH + SAMPLE_IN, category_in = XML_PATH + CATEGORY_IN, norm_in = XML_PATH + NORM_IN, syst_in = XML_PATH + SYST_IN, jetptcut = "35", discriminant = "BDT", IsAsimov = true)
ROOT.H4l_XML.writeXML(args.err, args.sample,args.category,args.norm,args.syst,args.jetptcut, args.discriminant, args.outpath, args.Asimov, args.mode)
ROOT.H4l_XML.writeDriver(args.dtd, args.wspath , args.category, args.outpath, args.discriminant) 
os.system("hist2workspace -standard_form {0}".format(join(args.outpath,args.discriminant,"driver.xml")))
