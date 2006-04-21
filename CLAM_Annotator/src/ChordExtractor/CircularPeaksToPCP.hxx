#ifndef CircularPeaksToPCP_hxx
#define CircularPeaksToPCP_hxx
#include <list>
#include <vector>
#include <cmath>

namespace Simac
{


class CircularPeaksToPCP
{
public:
	typedef std::vector<std::pair<double, double> > PeakList;
	typedef std::vector<double> PCP;
private:
	PCP _output;
	bool _windowingActivated;
	static const unsigned nSemitones=12;
public:
	CircularPeaksToPCP()
	{
		_output.resize(nSemitones);
		_windowingActivated=false;
	}
	~CircularPeaksToPCP()
	{
	}
	void activateWindowing()
	{
		_windowingActivated=true;
	}
	static double windowedValue(double position, double value)
	{
		return value* (0.54 - 0.46*std::cos(2*M_PI*(position+.5)));
	}
	void doIt(const PeakList & peaks)
	{
		_output.resize(0);
		_output.resize(nSemitones);
		const unsigned nPeaks=peaks.size();
		for (unsigned i=0; i<nPeaks; i++)
		{
			int quantizedSemitone = int(peaks[i].first + .5);
			unsigned semitone = (quantizedSemitone+nSemitones)%nSemitones;
			if (_windowingActivated)
				_output[semitone] += windowedValue(peaks[i].first,peaks[i].second);
			else
				_output[semitone] += peaks[i].second;
		}
	}

	const PCP & output() const
	{
		return _output;
	}
};

} // namespace Simac

#endif// CircularPeaksToPCP_hxx

