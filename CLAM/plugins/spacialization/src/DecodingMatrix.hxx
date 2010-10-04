#ifndef DecodingMatrix_hxx
#define DecodingMatrix_hxx

#include <string>

class DecodingMatrix
{
	unsigned _nInputs;
	unsigned _nOutputs;
	float * _weights;
public:
	DecodingMatrix()
		: _nInputs(0)
		, _nOutputs(0)
		, _weights(0)
	{
	}
	~DecodingMatrix()
	{
		if (_weights) delete [] _weights;
	}
	bool load(unsigned nInputs, unsigned nOutputs, const std::string & filePath, std::string & errorMsg);
	float weight(unsigned input, unsigned output) const { return _weights[output*_nInputs+output]; }
};

#endif

