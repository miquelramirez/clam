#ifndef __SMSTools__
#define __SMSTools__

#include "NotGeneratedUserInterface.hxx"
#include "WaitMessageGUI.hxx"
#include "ProgressGUI.hxx"
#include "SMSBase.hxx"
#include "Thread.hxx"
#include "SMS_DataExplorer.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"
using namespace CLAM;

class UserInterface;

namespace CLAMGUI
{

class SMSTools
	: public CLAM::SMSBase
{
public:

	friend class UserInterface;

	SMSTools();

	virtual ~SMSTools();

	void Run();

	Progress* CreateProgress(const char* title,float from,float to);

	WaitMessage* CreateWaitMessage(const char* title);

	void SetCanvas( Fl_Smart_Tile* canvas );

	SigSlot::Slotv1<double>        TimeSelected;
	/** Slot for setting the current score*/
	SigSlot::Slotv1< const SMSTransformationChainConfig& > SetScore;
	SigSlot::Signalv1< const SMSTransformationChainConfig& > ScoreChanged;

protected: // methods

		
	/** callback for the SetScore slot */
	virtual void OnNewScore( const SMSTransformationChainConfig& cfg );

	/** Load transformation score */
	virtual void LoadTransformationScore(const std::string& inputFileName);


	void DoAnalysis();

	void DoSynthesis();

	void DoTracksCleanup();

	void DoLoadAnalysis();

	void DoStoreAnalysis();
	
	void DoTransformation();

	void OnNewTime( double time );

	void DoMorphAnalysis();
	void DoMorphTracksCleanup();

	bool LoadAnalysis();
	void StoreAnalysis();

	void StoreSound(const Audio& audio);
	void StoreOutputSound();
	void StoreOutputSoundResidual();
	void StoreOutputSoundSinusoidal();


private:

	void ExecuteMethodOnThreadKeepingScreenUpToDate( CBL::Functor0 method );

protected:
	int                       mThreadState;
	std::string               mFilename;
	Thread                    mThread;
	CLAMVM::SMS_DataExplorer  mExplorer;
	UserInterface*            mUI;
};

}

#endif // SMSTools.hxx
