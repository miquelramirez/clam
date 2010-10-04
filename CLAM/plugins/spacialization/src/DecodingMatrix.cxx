#include "DecodingMatrix.hxx"
#include <fstream>
#include <sstream>


static bool error(std::string & errorMsg, const std::string & message)
{
	errorMsg += message;
	errorMsg += "\n";
	return false;
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
	unsigned parsedRows=0;
	for (unsigned row=0; true; row++)
	{
		std::string line;
		getline(file,line);
		if (file.eof()) break;
		if (line[0]=='#') continue;
		parsedRows++;
		std::ostringstream os;
		os << row;
		std::string rowString = os.str();
		std::istringstream is(line);
		for (unsigned column = 0; column<nInputs; column++)
		{
			float & weight = _weights[row*nOutputs+column];
			if (not (is >> weight))
				return error(errorMsg, "Missing or wrong weight at line "+rowString);
			is >> std::ws;
			std::string name;
			std::getline(is, name);
			if (not name.empty())
				return error(errorMsg, "Too many columns at line "+rowString);
		}
	}
	if (parsedRows!=nOutputs)
		error(errorMsg, "The decoding matrix has not the required number of rows for the configured number of speakers");
	return true;
}





