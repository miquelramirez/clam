
#ifndef RandomPitches_hxx
#define RandomPitches_hxx

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <string>

namespace CLAM
{

class RandomPitchesConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (RandomPitchesConfig, 0, ProcessingConfig);
protected:
	void DefaultInit(void);
};

class RandomPitches : public Processing
{
private:
	
	RandomPitchesConfig mConfig;
	AudioInPort mInput;
	OutPort<std::vector<CLAM::TData> > mOutput;

public:
	RandomPitches();
	RandomPitches( const RandomPitchesConfig & config);

	bool Do();
	virtual ~RandomPitches();
	const char * GetClassName() const {return "RandomPitches";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);
public:
	std::vector<CLAM::TData> _lastValues;

};

} //namespace CLAM

#endif



