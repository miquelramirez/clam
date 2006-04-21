#ifndef CircularPeakTunner_hxx
#define CircularPeakTunner_hxx
#include <list>
#include <vector>
#include <cmath>

namespace Simac
{

/**
 * This processing takes an array of circular peaks with position
 * at pitch units [0,12) and corrects them to be tunned given a
 * tunning center and a reference tune.
 * The tunning center is the detected tunning center within a single
 * semitone [0,1).
 * The reference tunning is the reference we take for pitch 0.
 * @todo be more explicit on the meaning of reference tunning.
 */

class CircularPeakTunner
{
public:
	typedef std::vector<std::pair<double, double> > PeakList;
	typedef std::vector<std::pair<double, double> > PitchProfile;
private:
	double _referenceTunning;
	PeakList _output;
public:
	CircularPeakTunner(double referenceTunning)
		: _referenceTunning(referenceTunning)
	{
		
	}
	~CircularPeakTunner()
	{
	}
	/**
	 * @todo Migrate this function tests to one bin per semitone
	 * Given a tunning (in chromogram bins) and a peak position, it
	 * returns the semitone pitch index being the zero semitone the one
	 * containing the reference tunning.
	 * @param tunning is a [0,3) number that indicates the detected
	 * center of a semitone inside a chromogram semitone triplet.
	 **/
	unsigned chromagramToSemitone(double tunning, double peakPosition)
	{
		const unsigned nSemitones = 12;
		const unsigned binsPerSemitone = 3;
		double scaledPosition = peakPosition/binsPerSemitone;
		double scaledTunning = tunning/binsPerSemitone;
		double scaledReference = _referenceTunning/binsPerSemitone;

		double shift = tunningShift(scaledReference, scaledTunning);
		double tunnedPosition = tune(scaledPosition, shift);

		int quantizedSemitone = int(tunnedPosition + .5);
		unsigned refoldedSemitone = (quantizedSemitone+nSemitones)%nSemitones;
		return refoldedSemitone;
	}

	static double tunningShift(double reference, double tunning)
	{
		return - tunning - std::floor(reference-tunning+0.5);
	}
	static double tune(double peakPosition, double shift)
	{
		const unsigned nSemitones = 12;
		double tunnedPosition = peakPosition + shift;
		while (tunnedPosition<0) tunnedPosition += nSemitones;
		while (tunnedPosition>= nSemitones) tunnedPosition -= nSemitones;
		return tunnedPosition;
	}

	void doIt(double center, const PeakList & peaks)
	{
		_output.resize(0);
		const unsigned nPeaks=peaks.size();
		double shift= tunningShift(_referenceTunning, center);
		for (unsigned i=0; i<nPeaks; i++)
		{
			double tunnedPosition =  tune(peaks[i].first,shift);
			_output.push_back(std::make_pair(tunnedPosition,peaks[i].second));
		}
	}

	const PeakList & output() const
	{
		return _output;
	}
};

} // namespace Simac

#endif// CircularPeakTunner_hxx

