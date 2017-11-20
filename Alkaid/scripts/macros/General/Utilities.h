#ifndef _Utilities_
#define _Utilities_
#pragma once
#include <vector>
#include "TString.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include "TObjString.h"
#include "TObjArray.h"

std::vector<TString> ParseInputTextToVector(TString input);

bool IsMatch(TString parent, TString child);
bool DoubleIsEqual(double a, double b);

template <typename T>
std::vector<T> ExtractVector(const std::vector<T> &v, unsigned int start , unsigned int end){
  std::vector<T> result(v.begin()+start, v.begin()+end);
  return result;
}
template <typename T>
std::vector<T> ExtractVector(const std::vector<T> &v, unsigned int pos){
  std::vector<T> result(v.begin()+pos, v.begin()+pos+1);
  return result;
}

template <typename T>
std::vector<T> ScalarToVector(const T &s) {
	std::vector<T> result;
	result.push_back(s);
	return result;
}


#endif