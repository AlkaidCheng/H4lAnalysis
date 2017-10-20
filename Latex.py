from os import listdir
from os.path import join
import argparse
from pdb import set_trace
parser = argparse.ArgumentParser()
parser.add_argument(
    'input')
 
args = parser.parse_args()
dir = [i for i in listdir(args.input) if ".txt" in i]
result = {i.replace("_statistics.txt",""):{} for i in dir }
data = {i.replace("_statistics.txt",""):[j.strip("\n").split("\t") for j in open(join(args.input,i)).readlines()] for i in dir }
#set_trace()
for i in data:
 total = 0
 total_bkg = 0
 for j in data[i]:
  result[i][j[0]] = int(round(eval(j[1])))
  if j[0] != "data":
   total += eval(j[1])
   if (j[0] != "ggH") and (j[0] !="VBF"):
    total_bkg += eval(j[1])
 #set_trace() 
 result[i]["Total background"] = int(round(total_bkg))
 result[i]["Total event"] = int(round(total))
param = [["lepton_eta[0]","jet_eta[0]"],["lepton_phi[0]","jet_phi[0]"],["phi","phi1"],["lepton_pt[0]","jet_pt[0]"],["pt4l_fsr","m4l_fsr"]]

f = open("latex.txt","w")
for p in param:
 f.write('\\begin{center}\n')
 f.write('\\begin{tabular}{ c c c}\n')
 f.write('\\hline\n')
 f.write('& $H\\rightarrow 4l$ channel&  \\\\\\hline\n')
 f.write('& $\sqrt{s} = 13 $\\,TeV &luminosity = 36.1\,pb$^{-1}$\\\\\n')
 f.write('Parameter & \\textbf{'+p[0].replace('_','\_')+'} & \\textbf{'+p[1].replace('_','\_')+'}\\\\\\hline\n')
 f.write('Process & Count & Count\\\\\\hline'+"\n")
 if 'data driven background' in result[p[0]]:
  f.write('data driven bkg&{0}&{1}'.format(result[p[0]]['data driven background'],result[p[1]]['data driven background'])+'\\\\\n')
 f.write('redu\_ttW&{0}&{1}'.format(result[p[0]]['redu_ttW'],result[p[1]]['redu_ttW'])+'\\\\\n')
 f.write('redu\_ttZ&{0}&{1}'.format(result[p[0]]['redu_ttZ'],result[p[1]]['redu_ttZ'])+'\\\\\n')
 f.write('redu\_bbH&{0}&{1}'.format(result[p[0]]['redu_bbH'],result[p[1]]['redu_bbH'])+'\\\\\n')
 f.write('redu\_ZZZ&{0}&{1}'.format(result[p[0]]['redu_ZZZ'],result[p[1]]['redu_ZZZ'])+'\\\\\n')
 f.write('redu\_WZZ&{0}&{1}'.format(result[p[0]]['redu_WZZ'],result[p[1]]['redu_WZZ'])+'\\\\\n')
 f.write('redu\_WWZ&{0}&{1}'.format(result[p[0]]['redu_WWZ'],result[p[1]]['redu_WWZ'])+'\\\\\n')
 f.write('gg\_ZZ&{0}&{1}'.format(result[p[0]]['gg_ZZ'],result[p[1]]['gg_ZZ'])+'\\\\\n')
 f.write('ZZllll&{0}&{1}'.format(result[p[0]]['ZZllll'],result[p[1]]['ZZllll'])+'\\\\\\hline\n')
 f.write('Total background&{0}&{1}'.format(result[p[0]]['Total background'],result[p[1]]['Total background'])+'\\\\\n')
 f.write('VBF&{0}&{1}'.format(result[p[0]]['VBF'],result[p[1]]['VBF'])+'\\\\\n')
 f.write('ggH&{0}&{1}'.format(result[p[0]]['ggH'],result[p[1]]['ggH'])+'\\\\\\hline\n')
 f.write('Total event&{0}&{1}'.format(result[p[0]]['Total event'],result[p[1]]['Total event'])+'\\\\\n')
 f.write('Observed data&{0}&{1}'.format(result[p[0]]['data'],result[p[1]]['data'])+'\\\\\\hline\n')
 f.write('\\end{tabular}\n')
 f.write('\\captionof{{table}}{{Observed and expected number of events for the $H\\rightarrow 4l$ channel for the parameter {0} and {1}}}\n'.format(p[0].replace('_','\_'),p[1].replace('_','\_')))
 f.write('\\end{center}\n') 
 f.write('\n')
f.close()
 
 
 
 
 
 