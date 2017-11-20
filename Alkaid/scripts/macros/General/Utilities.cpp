#include "Utilities.h"

std::vector<TString> ParseInputTextToVector(TString input) {
	std::ifstream Input_Files(input.Data(), std::ifstream::in);
	if (!Input_Files.is_open())
		std::cout << "Cannot open file: " << input << std::endl;
	std::vector< TString > result;
	TString buffer;
	while (Input_Files >> buffer)
		result.push_back(buffer);
	Input_Files.close();
	return result;
}

bool IsMatch(TString parent, TString child) {
	bool result = false;
	if (!parent.CompareTo("all")) {
		return true;
	}
	else if (!parent.Contains("*") && !parent.Contains("||") && !parent.Contains("~")) {
		if (!parent.CompareTo(child))
			return true;
	}
	else if (parent.Contains("*")) {
		if (child.Contains(parent.ReplaceAll("*", "")))
			return true;
	}
	else if (parent.Contains("||")) {
		TObjArray* token = parent.Tokenize("||");
		for (int i = 0; i< token->GetEntriesFast(); i++) {
			if (!child.CompareTo(((TObjString*)token->At(i))->String()))
				return true;
		}
	}
	else if (parent.Contains("~")) {
		TObjArray* token = parent.Tokenize("~");
		result = true;
		for (int i = 0; i< token->GetEntriesFast(); i++) {
			if (!child.CompareTo(((TObjString*)token->At(i))->String()))
				return false;
		}
	}
	return result;
}

bool DoubleIsEqual(double a, double b) {
	return (a == b) || (std::abs(a - b) < std::abs(std::min(a, b))*std::numeric_limits<double>::epsilon());
}
