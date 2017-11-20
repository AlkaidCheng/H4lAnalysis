from collections import namedtuple
from pdb import set_trace
import os
from os.path import join
from os import system
from array import array
import re

def IsNumber(s):
    try: 
        float(s)
        return True
    except ValueError:
        return False
        
def EvaluateExpression(s):
 if IsNumber(s):
  return eval(s)
 elif s == "None":
  return None
 else:
  return s

def ImportROOTFilesFromText(text):
 FileStruct = namedtuple("File",['PATH', 'NAME', 'RAWNAME'])
 text = open(text)
 dir = text.readline().strip("#").rstrip()
 files = filter(bool,[f.rstrip() for f in text.readlines() if not f.startswith("#")])
 ROOTFiles = [FileStruct(join(dir,f),f.replace(".root",""),f) for f in files]
 return ROOTFiles
 
def ParsePlotInstructionFromText(text):
 text = open(text)
 instruction = {"PARAM":[]}
 for line in text.readlines():
  if line.startswith("#include"):
   key,value = line.replace("#include","").split("=",2)
   key = key.strip()
   value = [EvaluateExpression(i) for i in value.strip().split()]
   if len(value) == 1:
    value = value[0]
   instruction[key] = value
  elif not line.startswith("#"):
   if "KEYS" in instruction:
    items = [EvaluateExpression(i) for i in line.strip().split()]
    instruction["PARAM"].append(dict(zip(instruction["KEYS"], items)))
   else:
    raise Exception("No Keys for Parameters Found")
 return instruction
 
def AddPrefixToChars(s,prefix = "Tree"):
 params = [i for i in filter(bool, re.split("[^a-zA-Z0-9\[\]\_]*", s)) if not i.isdigit()]
 for param in params:
  s = s.replace(param,'{0}.{1}'.format(prefix,param))
 return s 
 
def ParsePrefixToDictValues(d,prefix = "Tree"):
 for key in d:
  d[key] = AddPrefixToChars(d[key],prefix)
 return d
 
def CountParamInStr(s):
 return len([i for i in filter(bool, re.split("[^a-zA-Z0-9\[\]\_]*", s)) if not i.isdigit()])

def try_remove(f):
 if (os.path.isfile(f)):
  try:
   os.remove(f)
  except OSError:
   pass

def try_makedir(f):
 if (not os.path.isdir(f)):
  os.makedirs(f)