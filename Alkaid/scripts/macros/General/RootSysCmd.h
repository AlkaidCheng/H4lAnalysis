#ifndef _RootSysCmd_
#define _RootSysCmd_
#pragma once
#include <string>
#include "TString.h"
#include "TSystem.h"

namespace RSysCmd {
  inline bool file_exists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
  }
  
  inline bool file_exists(const TString& name) {
    ifstream f(name.Data());
    return f.good();
  }

	inline int rm(TString s) {	//Remove file matching the string s
    if (file_exists(s))
		  gSystem->Exec(TString("").Format("rm %s", s.Data()));
    return 0;
	}
	inline int rmrec(TString s) {
   if (file_exists(s))
		gSystem->Exec(TString("").Format("rm -r %s", s.Data()));
    return 0;
	}
	inline int mkdir(TString s) {
    if (!file_exists(s))
		  gSystem->Exec(TString("").Format("mkdir -p %s", s.Data()));
    return 0;
	}


}

#endif