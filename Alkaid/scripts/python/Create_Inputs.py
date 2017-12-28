import H4lAnalysis
import Utilities as Util
import PlotUtil
import ROOT
from collections import namedtuple
import glob
import argparse
import os
from os import listdir
from os.path import join
from pdb import set_trace

parser = argparse.ArgumentParser()
parser.add_argument(
    'dir', default = "../../All_Input/input_sample/", help="""Path to the input samples""")
parser.add_argument(
    '--discriminant','-d', default = "BDT_1bin", help="""Name of the discriminant added to the histogram name""")
parser.add_argument(
    '--mode','-m', default = "vv", help="""Name of the discriminant added to the histogram name""")
args = parser.parse_args()


categories = H4lAnalysis.Minitree_H4l.category_list_valid


CombineStruct = namedtuple("CombineStruct", "name input output hname")

Combine01 = [ CombineStruct('ZZllll','*ZZllll_mll4_*.root','output_category_{0}_ZZllll_nominal.root','ZZllll'),
              CombineStruct('MC_REDU_BKG','*_redu_*.root','output_category_{0}_MC_REDU_BKG_nominal.root','MC_REDU_BKG'),
              CombineStruct('bbH','*_redu_bbH_*.root','output_category_{0}_bbH_SUM_nominal.root','bbH_SUM'),
              CombineStruct('vbfMG5_0p','*vbfMG5_0p_*.root','output_category_{0}_vbfMG5_0p_nominal.root','vbfMG5_0p'),
              CombineStruct('vbfMG5_0mkAv15','*vbfMG5_0mkAv15_*.root','output_category_{0}_vbfMG5_0mkAv15_nominal.root','vbfMG5_0mkAv15'),
              CombineStruct('vbfMG5_0phkHv10','*vbfMG5_0phkHv10_*.root','output_category_{0}_vbfMG5_0phkHv10_nominal.root','vbfMG5_0phkHv10'),
              CombineStruct('vbfMG5_0pphkHv2p5','*vbfMG5_0pphkHv2p5_*.root','output_category_{0}_vbfMG5_0pphkHv2p5_nominal.root','vbfMG5_0pphkHv2p5'),
              CombineStruct('vbfMG5_0pphkHv5','*vbfMG5_0pphkHv5_*.root','output_category_{0}_vbfMG5_0pphkHv5_nominal.root','vbfMG5_0pphkHv5'),
              CombineStruct('vbfMG5_0pphkHvm2p5','*vbfMG5_0pphkHvm2p5_*.root','output_category_{0}_vbfMG5_0pphkHvm2p5_nominal.root','vbfMG5_0pphkHvm2p5'),
              CombineStruct('vbfMG5_0pphkHvm5','*vbfMG5_0pphkHvm5_*.root','output_category_{0}_vbfMG5_0pphkHvm5_nominal.root','vbfMG5_0pphkHvm5'),
              CombineStruct('vbfMG5_0pphm_khv2p5_kAvm5','*vbfMG5_0pphm_khv2p5_kAvm5_*.root','output_category_{0}_vbfMG5_0pphm_khv2p5_kAvm5_nominal.root','vbfMG5_0pphm_khv2p5_kAvm5'),
              CombineStruct('vbfMG5_0pphm_khv5_kAv5','*vbfMG5_0pphm_khv5_kAv5_*.root','output_category_{0}_vbfMG5_0pphm_khv5_kAv5_nominal.root','vbfMG5_0pphm_khv5_kAv5'),
              CombineStruct('vbfMG5_0pphm_khv5_kAv6','*vbfMG5_0pphm_khv5_kAv6_*.root','output_category_{0}_vbfMG5_0pphm_khv5_kAv6_nominal.root','vbfMG5_0pphm_khv5_kAv6'),
              CombineStruct('vbfMG5_0pphm_khvm2p5_kAvm5','*vbfMG5_0pphm_khvm2p5_kAvm5_*.root','output_category_{0}_vbfMG5_0pphm_khvm2p5_kAvm5_nominal.root','vbfMG5_0pphm_khvm2p5_kAvm5'),
              CombineStruct('vbfMG5_0pphm_khvm5_kAv5','*vbfMG5_0pphm_khvm5_kAv5_*.root','output_category_{0}_vbfMG5_0pphm_khvm5_kAv5_nominal.root','vbfMG5_0pphm_khvm5_kAv5'),
              CombineStruct('vbfMG5_0pphm_khvm5_kAvm6','*vbfMG5_0pphm_khvm5_kAvm6_*.root','output_category_{0}_vbfMG5_0pphm_khvm5_kAvm6_nominal.root','vbfMG5_0pphm_khvm5_kAvm6'),
              CombineStruct('vbfMG5_0pphmkAv2p5','*vbfMG5_0pphmkAv2p5_*.root','output_category_{0}_vbfMG5_0pphmkAv2p5_nominal.root','vbfMG5_0pphmkAv2p5'),
              CombineStruct('vbfMG5_0pphmkAv5','*vbfMG5_0pphmkAv5_*.root','output_category_{0}_vbfMG5_0pphmkAv5_nominal.root','vbfMG5_0pphmkAv5'),
              CombineStruct('vbfMG5_0pphmkAvm2p5','*vbfMG5_0pphmkAvm2p5_*.root','output_category_{0}_vbfMG5_0pphmkAvm2p5_nominal.root','vbfMG5_0pphmkAvm2p5'),
              CombineStruct('vbfMG5_0pphmkAvm5','*vbfMG5_0pphmkAvm5_*.root','output_category_{0}_vbfMG5_0pphmkAvm5_nominal.root','vbfMG5_0pphmkAvm5')]
              
CombineMC = [ 'output_category_{0}_MC_REDU_BKG_nominal.root', 'output_category_{0}_bbH_SUM_nominal.root']

CombineAsimov = [ 'ggfMG5_0p', 'vbfMG5_0p', 'ZZllll', 'ggZZ_2mu2e' , 'ggZZ_4l', 'Redu_Bkg', 'ttH', 'MC_REDU_BKG', 'bbH_SUM' ]

#Redu_Bkg: Data Driven Background
#MC_REDU_BKG: Reducible Background from MC Data

if (not os.path.isdir(args.dir)):
 raise Exception("ERROR: Incorrect Path: {0}".format(args.dir))

print "PROCESSING DISCRIMINANT: {0}".format(args.discriminant)

for category in categories:
 print "Starting Category: {0}".format(category)
 newdir = join(args.dir,args.discriminant,category)
 for element in Combine01:
  Util.try_remove(join(newdir,element.output.format(category)))  

  os.system("python MergeHistFromRootFiles.py {0} -o={1} --hname={2}".format(" ".join(glob.glob(join(newdir,element.input))),join(newdir,element.output.format(category)),element.hname+"_"+args.discriminant))
  print "Finish combining {0}".format(element.name)

 os.system("python MergeHistFromRootFiles.py {0} {1} -o={2} --hname={3} --method=dif".format(join(newdir,CombineMC[0].format(category)),join(newdir,CombineMC[1].format(category)),join(newdir,CombineMC[0].format(category)),'MC_REDU_BKG' + "_"+args.discriminant))
 print "Finish combining MC Reducible Background"
 
 PlotUtil.NormalizeHistFromRoot(join(newdir,'output_category_{0}_MC_REDU_BKG_nominal.root'.format(category)), H4lAnalysis.Minitree_H4l.Data_Driven_Count[category],fout_name = join(newdir,'output_category_{0}_Redu_Bkg_nominal.root'.format(category)), hout_name = 'Redu_Bkg_{0}'.format(args.discriminant))
 print "Finish producing data driven backgroun samples"
 
 Util.try_remove(join(newdir,'output_category_{0}_data_run_2_Asimov_nominal.root'.format(category)))
 os.system("python MergeHistFromRootFiles.py {0} -o={1} --hname={2} ".format(" ".join([join(newdir,'output_category_{0}_{1}_nominal.root'.format(category,i)) for i in CombineAsimov]), join(newdir,'output_category_{0}_data_run_2_Asimov_nominal.root'.format(category)), 'data_run_2_Asimov_{0}'.format(args.discriminant)))
 print "Finish producing Asimov data"
 
