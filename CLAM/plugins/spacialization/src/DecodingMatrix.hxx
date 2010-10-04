#ifndef DecodingMatrix_hxx
#define DecodingMatrix_hxx

#include <string>
#include <CLAM/Assert.hxx>

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
	float weight(unsigned input, unsigned output) const
	{
		CLAM_ASSERT(input<_nInputs,"Accessing to an input beyond limit");
		CLAM_ASSERT(output<_nOutputs,"Accessing to an output beyond limit");
		return _weights[output*_nInputs+input];
	}
};

#endif

