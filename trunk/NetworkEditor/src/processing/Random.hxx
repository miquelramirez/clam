
#ifndef _Random_hxx_
#define _Random_hxx_

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Mapping.hxx>
#include <string>

namespace CLAM
{
class RandomConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (RandomConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, int, DelayFactor); // To be improved...
	DYN_ATTRIBUTE (2, public, int, MinNote); // Tessitura (min)
	DYN_ATTRIBUTE (3, public, int, MaxNote); // Tessitura (max)

protected:
	void DefaultInit(void);
};

class Random : public Processing
{
private:
	
	RandomConfig mConfig;
	InControl mInput;
	OutControl mOutput;
	int mDelayFactor;
	int mMinNote;
	int mMaxNote;
	NoteToFreqMapping mMidiToFreq;
	TData mCounter;

public:
	Random();
	Random( const RandomConfig & );

	bool Do();
	virtual ~Random(){}
	const char * GetClassName() const {return "Random";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif



