#ifndef ConstantQFolder_hxx
#define ConstantQFolder_hxx
#include <vector>
namespace Simac
{

/**
 * The ConstantQFolder takes a ConstantQ transform and folds its bins
 * into a single Octave to create a Chromagram.
 * \b Parameters:
 * - nConstantQBins: The number of bins on the Constant Q transform.
 * - binsPerOctave: The number of bins that corresponds to each octave.
 * \b Inputs:
 * - A constant Q transform as a vector containing the real and imaginary pairs
 *   bins, so it is size nConstantQBins * 2
 * \b Outputs:
 * - A chromogram of binsPerOctave bins.
 * @todo Only nConstantQBins that are multiple of binsPerOctave have been tested.
 */
class ConstantQFolder
{
public:
	typedef std::vector<double> Chromagram;
private:
	Chromagram _chromadata;
	unsigned _binsPerOctave;
	unsigned _nConstantQBins;
public:
	ConstantQFolder(unsigned nConstantQBins, int binsPerOctave);
	~ConstantQFolder();
	void doIt(const std::vector<double> & constantQData);

	/**
	 * Returns a chromagram that is the constant q transform for a a vector containing the added complex module of
	 * the bins that correspond to octaves.
	 */
	const Chromagram & chromagram() const {return _chromadata;}
};

}

#endif//ConstantQFolder_hxx 

