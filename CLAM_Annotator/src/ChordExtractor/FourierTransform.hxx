#ifndef FourierTransform_hxx
#define FourierTransform_hxx
#include <vector>

class FourierTransform {
	std::vector<double> datah; //(=insegment)
	unsigned long mFrameSize;
	bool mIsComplex;
	double mDataNorm;
public:
	FourierTransform(unsigned long int frameSize, double normalizationFactor, bool isComplex);
	~FourierTransform();

	void doIt(const float * input);
	void doIt(const double * input);
	const std::vector<double> & spectrum() {return datah;};
};

#endif//FourierTransform_hxx

