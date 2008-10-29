
#ifndef _OneOverF_hxx_
#define _OneOverF_hxx_

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Mapping.hxx>
#include <string>

#define MAXDICES 7

namespace CLAM
{

class OneOverFConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (OneOverFConfig, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, int, DelayFactor); // To be improved...
	DYN_ATTRIBUTE (2, public, int, MinNote); // Tessitura (min)
	DYN_ATTRIBUTE (3, public, int, MaxNote); // Tessitura (max)
	DYN_ATTRIBUTE (4, public, int, Dices); // 1..7

protected:
	void DefaultInit(void);
};

class OneOverF : public Processing
{
private:

	OneOverFConfig mConfig;
	FloatInControl mInput;
	FloatOutControl mOutput;
	int mDelayFactor;
	int mMinNote;
	int mMaxNote;
	int mDices;
	NoteToFreqMapping mMidiToFreq;
	int      SeqLength;
	int      SeqIndex;
	int      PrevSeqIndex;
	int      RandomMax;
	int      Values[MAXDICES];
	TData mCounter;

	void ResetSeq();

public:
	OneOverF();
	OneOverF( const OneOverFConfig & );

	bool Do();
	virtual ~OneOverF(){}
	const char * GetClassName() const {return "OneOverF";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif



