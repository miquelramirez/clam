#include "DecodingMatrix.hxx"
#include <fstream>
#include <sstream>
#include <iostream>


static bool error(std::string & errorMsg, const std::string & message)
{
	errorMsg += message;
	errorMsg += "\n";
	return false;
}

static std::string numberToString(unsigned i)
{
	std::ostringstream os;
	os << i;
	return os.str();
}

bool DecodingMatrix::load(unsigned nInputs, unsigned nOutputs, const std::string & filePath, std::string & errorMsg)
{
	if (filePath.empty()) return error(errorMsg, "No decoding matrix file specified");
	std::ifstream file(filePath.c_str());
	if (!file) return error(errorMsg, "Could not open the decoding matrix file "+filePath);
	_nInputs = nInputs;
	_nOutputs = nOutputs;
	if (_weights) delete _weights;
	_weights = new float[nInputs*nOutputs];
	unsigned parsedOutputs=0;
	for (unsigned row=0; file; row++)
	{
		std::string line;
		getline(file,line);
		if (file.eof()) break;
		if (line[0]=='#') continue;
		std::istringstream is(line);
		for (unsigned column = 0; column<nInputs; column++)
		{
			float & weight = _weights[parsedOutputs*nInputs+column];
			if (not (is >> weight))
				return error(errorMsg, "Missing or wrong weight at line "+numberToString(row));
		}
		is >> std::ws;
		std::string trailingText;
		std::getline(is, trailingText);
		if (not trailingText.empty())
			return error(errorMsg, "Too many columns at line "+numberToString(row));
		parsedOutputs++;
	}
	if (parsedOutputs!=nOutputs)
		error(errorMsg, "The decoding matrix has not the required number of rows for the configured number of speakers");
	return true;
}





