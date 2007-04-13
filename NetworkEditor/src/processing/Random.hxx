
#ifndef _Random_hxx_
#define _Random_hxx_

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Mapping.hxx>
#include <CLAM/Filename.hxx> // TODO: Remove this line
#include <string>

namespace CLAM
{

class InImageFilename : public InFilename
{
public:
	InImageFilename(const std::string & s="") : InFilename(s) {}
	InImageFilename(const char * s) : InFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Portable Network Graphics", "*.png"},
			{"JPEG Image", "*.jpg"},
			{"Scalable Vector Graphics", "*.svg"},
			{0,0}
		};
		return filters;
	}
};

class OutImageFilename : public OutFilename
{
public:
	OutImageFilename(const std::string & s="") : OutFilename(s) {}
	OutImageFilename(const char * s) : OutFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Portable Network Graphics", "*.png"},
			{"JPEG Image", "*.jpg"},
			{0,0}
		};
		return filters;
	}
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, OutImageFilename);
CLAM_TYPEINFOGROUP(BasicCTypeInfo, InImageFilename);


class RandomConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (RandomConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, int, DelayFactor); // To be improved...
	DYN_ATTRIBUTE (2, public, int, MinNote); // Tessitura (min)
	DYN_ATTRIBUTE (3, public, int, MaxNote); // Tessitura (max)
	DYN_ATTRIBUTE (4, public, CLAM::InImageFilename, FooIn);
	DYN_ATTRIBUTE (5, public, CLAM::OutImageFilename, FooOut);

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



