#ifndef InstantTunningEstimator_hxx
#define InstantTunningEstimator_hxx
#include <list>
#include <vector>
#include <cmath>

namespace Simac
{


class InstantTunningEstimator
{
public:
	typedef std::vector<std::pair<double, double> > PeakList;
private:
	double _fasorX;
	double _fasorY;
	unsigned _binsPerSemitone;
	double _inertia;
public:
	InstantTunningEstimator(double inertia=0.0, unsigned binsPerSemitone=3)
		: _inertia(inertia)
		, _binsPerSemitone(binsPerSemitone)
	{
		_fasorX=1.0;
		_fasorY=0.0;
	}
	~InstantTunningEstimator()
	{
	}
	void setInertia(double inertia)
	{
		_inertia=inertia;
	}
	void doIt(unsigned int nPeaks, const double * peakPositions, const double * peakValues)
	{
		_fasorX*=_inertia;
		_fasorY*=_inertia;
		for (unsigned int peak=0; peak<nPeaks; peak++)
		{
			double radiantTunning=peakPositions[peak]*2*M_PI/_binsPerSemitone;
			_fasorX+=cos(radiantTunning)*peakValues[peak];
			_fasorY+=sin(radiantTunning)*peakValues[peak];
		}
	}
	// TODO: This function is taken by S&R of the previous one, no test!!
	void doIt(const std::vector<std::pair<double, double> >& peaks)
	{
		_fasorX*=_inertia;
		_fasorY*=_inertia;
		for (unsigned int peak=0; peak<peaks.size(); peak++)
		{
			double radiantTunning=peaks[peak].first*2*M_PI/_binsPerSemitone;
			_fasorX+=cos(radiantTunning)*peaks[peak].second;
			_fasorY+=sin(radiantTunning)*peaks[peak].second;
		}
	}
	std::pair<double,double> output() const
	{
		double tunning=std::atan2(_fasorY,_fasorX)*_binsPerSemitone/2/M_PI;
		double strength=std::sqrt(_fasorY*_fasorY+_fasorX*_fasorX);
		return std::make_pair(tunning, strength);
	}
};

} // namespace Simac

#endif// InstantTunningEstimator_hxx

