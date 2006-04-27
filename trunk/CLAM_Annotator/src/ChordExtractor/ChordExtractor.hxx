#ifndef ChordExtractor_hxx
#define ChordExtractor_hxx

#include "ChordCorrelator.hxx"
#include "CircularPeakPicking.hxx"
#include "CircularPeaksToPCP.hxx"
#include "CircularPeakTunner.hxx"
#include "ConstantQFolder.hxx"
#include "ConstantQTransform.hxx"
#include "Err.hxx"
#include "FourierTransform.hxx"
#include "InstantTunningEstimator.hxx"
#include "SemitoneCenterFinder.hxx"
#include "PCPSmother.hxx"

namespace Simac
{

class ChordExtractor
{
	double _sparseConstantQKernelThreshold;
	ConstantQTransform _constantQTransform;
	ConstantQFolder _constantQFolder;
	FourierTransform _fourierTransform;
	CircularPeakPicking _circularPeakPicking;
	InstantTunningEstimator _instantTunningEstimator;
	CircularPeakTunner _circularPeakTunner;
	CircularPeaksToPCP _circularPeaksToPCP;
	PCPSmother _filter;
	ChordCorrelator _chordCorrelator;
	bool _tunningEnabled;
	bool _peakWindowingEnabled;
	double _hopRatio;
	unsigned _estimatedChord;
	unsigned _secondCandidate;
	double _squaredRootEnergy;
public:
	static double maximumFrequency(double sampleRate) { return sampleRate/2.1; } // Just below nyquist
	typedef float * AudioFrame;

	ChordExtractor(unsigned sampleRate=44100, double minimumFrequency=98, unsigned binsPerOctave=36)
		: _sparseConstantQKernelThreshold(0.0054)
		, _constantQTransform(sampleRate, minimumFrequency, maximumFrequency(sampleRate), binsPerOctave)
		, _constantQFolder(_constantQTransform.getK(), binsPerOctave)
		, _fourierTransform(_constantQTransform.getfftlength(),1,0)
		, _circularPeakPicking(binsPerOctave, /*scaling factor*/ 12.0/binsPerOctave)
		, _instantTunningEstimator(/*Inertia*/ 1.0, /*bins per semitone*/  1)
		, _circularPeakTunner(/*reference tunning*/ 0.0)
		, _filter(0.0)
		, _tunningEnabled(true)
		, _peakWindowingEnabled(true)
		, _hopRatio(8.0) // On the original Chromagram cpp code was 32
		, _estimatedChord(0)
		, _secondCandidate(0)
	{
		_constantQTransform.sparsekernel(_sparseConstantQKernelThreshold);
		if (_peakWindowingEnabled)
			_circularPeaksToPCP.activateWindowing();
	}
	~ChordExtractor()
	{
	}

	unsigned hop() const {return _constantQTransform.getfftlength()/_hopRatio;}
	unsigned frameSize() const {return _constantQTransform.getfftlength();}

	void doIt(const AudioFrame & input)
	{
		_squaredRootEnergy = 0.0;
		for (unsigned i=0; i<frameSize(); i++)
			_squaredRootEnergy += input[i]*input[i];
			
		_fourierTransform.doIt(input);
		_constantQTransform.doIt(_fourierTransform.spectrum());
		_constantQFolder.doIt(_constantQTransform.constantQSpectrum());
		_circularPeakPicking.doIt(_constantQFolder.chromagram());
		_instantTunningEstimator.doIt(_circularPeakPicking.output());
		_circularPeakTunner.doIt(_instantTunningEstimator.output().first, _circularPeakPicking.output());
		if (_tunningEnabled)
			_circularPeaksToPCP.doIt(_circularPeakTunner.output());
		else
			_circularPeaksToPCP.doIt(_circularPeakPicking.output());
		_filter.doIt(_circularPeaksToPCP.output());
		_chordCorrelator.doIt(_filter.output());
		estimateChord(_chordCorrelator.output());
	}
	void estimateChord(const ChordCorrelator::ChordCorrelation & correlation)
	{
		double maxCorrelation = 0;
		double underMaxCorrelation = 0;
		unsigned maxIndex = 0;
		unsigned underMaxIndex = 0;
		for (unsigned i=0; i<correlation.size(); i++)
		{
			if (correlation[i]<underMaxCorrelation) continue;
			if (correlation[i]<maxCorrelation)
			{
				underMaxIndex=i;
				underMaxCorrelation=correlation[i];
				continue;
			}
			underMaxIndex=maxIndex;
			underMaxCorrelation=maxCorrelation;
			maxIndex=i;
			maxCorrelation=correlation[i];
		}
		_estimatedChord = maxIndex;
		_secondCandidate = underMaxIndex;
	}
	std::string chordRepresentation(unsigned chordIndex) const
	{
		return _chordCorrelator.chordRepresentation(chordIndex);
	}
	std::string root(unsigned chordIndex) const
	{
		return _chordCorrelator.root(chordIndex);
	}
	std::string mode(unsigned chordIndex) const
	{
		return _chordCorrelator.mode(chordIndex);
	}
	const std::string chordEstimation() const
	{
		const ChordCorrelator::ChordCorrelation & correlation = _chordCorrelator.output();
		double maxCorrelation=correlation[_estimatedChord];
		double underMaxCorrelation=correlation[_secondCandidate];
		if (maxCorrelation*0.7<=correlation[0]) return "None";
		bool estimationIsClear = maxCorrelation*0.9>underMaxCorrelation;
		std::ostringstream os;
		os << _chordCorrelator.chordRepresentation(_estimatedChord);
		if (!estimationIsClear) 
			os << " [or "<< _chordCorrelator.chordRepresentation(_secondCandidate)<< "]";
		os << " (" << (correlation[0]/maxCorrelation) << ")";
		if (!estimationIsClear) 
			os << " (" << (underMaxCorrelation/(underMaxCorrelation+maxCorrelation)) << ")";
		return os.str();
	}
	const std::vector<double> & chromagram() const
	{
		return _constantQFolder.chromagram();
	}
	const std::vector<double> & pcp() const
	{
		return _circularPeaksToPCP.output();
	}
	const std::vector<std::pair<double, double> > & peaks() const
	{
		return _circularPeakPicking.output();
	}
	const std::vector<double> & chordCorrelation() const
	{
		return _chordCorrelator.output();
	}
	double tunning() const {return _instantTunningEstimator.output().first; }
	double tunningStrength() const {return _instantTunningEstimator.output().second; }
	double energy() const {return _squaredRootEnergy; }
	unsigned firstCandidate() const {return _estimatedChord;}
	unsigned secondCandidate() const {return _secondCandidate;}

};
}

#endif//ChordExtractor

