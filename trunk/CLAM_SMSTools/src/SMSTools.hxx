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
#include <string>


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
	SigSlot::Slotv1< const CLAM::SMSTransformationChainConfig& > SetScore;
	SigSlot::Signalv1< const CLAM::SMSTransformationChainConfig& > ScoreChanged;

protected: // methods

		
	/** callback for the SetScore slot */
	virtual void OnNewScore( const CLAM::SMSTransformationChainConfig& cfg );

	/** Load transformation score */
	virtual void LoadTransformationScore(const std::string& inputFileName);


	void DoAnalysis();

	void DoSynthesis();

	void DoTracksCleanup();

	bool DoLoadAnalysis();

	bool DoStoreAnalysis();
	
	void DoTransformation();

	void OnNewTime( double time );

	void DoMorphAnalysis();
	void DoMorphTracksCleanup();

	bool LoadAnalysis();
	void StoreAnalysis();
	void ExecuteMelodyAnalysis();
	void AnalyzeMelody();

	void StoreSound(const CLAM::Audio& audio);
	void StoreOutputSound();
	void StoreOutputSoundResidual();
	void StoreOutputSoundSinusoidal();
	void StoreMelody();

private:

	void ExecuteMethodOnThreadKeepingScreenUpToDate( CBL::Functor0 method );


protected:
	int                       mThreadState;
	std::string               mFilename;
	std::string               mAnalysisInputFile;
	std::string               mAnalysisOutputFile;
	CLAM::Thread              mThread;
	CLAMVM::SMS_DataExplorer  mExplorer;
	UserInterface*            mUI;
};

}

#endif // SMSTools.hxx
