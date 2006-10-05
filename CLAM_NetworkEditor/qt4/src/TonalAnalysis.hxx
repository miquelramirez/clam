
#ifndef TonalAnalysis_hxx
#define TonalAnalysis_hxx

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <string>

namespace Simac { class ChordExtractor; }

namespace CLAM
{

class TonalAnalysisConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (TonalAnalysisConfig, 0, ProcessingConfig);
protected:
	void DefaultInit(void);
};

class TonalAnalysis : public Processing
{
private:
	
	TonalAnalysisConfig _config;
	AudioInPort _input;
	OutPort<std::vector<CLAM::TData> > _output;

public:
	TonalAnalysis( const TonalAnalysisConfig & config = TonalAnalysisConfig() );

	bool Do();
	virtual ~TonalAnalysis();
	const char * GetClassName() const {return "TonalAnalysis";}
	
	inline const ProcessingConfig &GetConfig() const { return _config;}
	bool ConcreteConfigure(const ProcessingConfig& c);
private:
	Simac::ChordExtractor * _implementation;
	std::vector<float> _floatBuffer;

};

} //namespace CLAM

#endif



