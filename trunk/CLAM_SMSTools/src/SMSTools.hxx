#ifndef __SMSTools__
#define __SMSTools__

#include "SMSBase.hxx"
#include "NotGeneratedUserInterface.hxx"
#include "WaitMessageGUI.hxx"
#include "ProgressGUI.hxx"
#include <CLAM/Thread.hxx>
#include "SMS_DataExplorer.hxx"
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>
#include <string>


class UserInterface;

namespace CLAMGUI
{

class SMSTools
	: public CLAM::SMSBase
{
public:

//	friend class UserInterface;

	SMSTools();

	virtual ~SMSTools();

	void Run();


	void StoreSound(const CLAM::Audio& audio);
	void StoreOutputSound();
	void StoreOutputSoundResidual();
	void StoreOutputSoundSinusoidal();
	void StoreMelody();
	
	bool LoadAnalysis();
	
	virtual bool LoadInputSound();

	void StoreAnalysis();
	void ExecuteMelodyAnalysis();
	void AnalyzeMelody();
	
	/** Load transformation score */
	virtual void LoadTransformationScore(const std::string& inputFileName);
	
	bool DoLoadAnalysis();

	bool DoStoreAnalysis();
	
	
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

	

	void DoAnalysis();

	void DoSynthesis();

	void DoTracksCleanup();

	
	void DoTransformation();

	void OnNewTime( double time );

	void DoMorphAnalysis();
	void DoMorphTracksCleanup();

	virtual bool LoadSound(const std::string& filename,CLAM::Segment& segment);

	
	
private:

	void ExecuteMethodOnThreadKeepingScreenUpToDate( CBL::Functor0 method );

public: // methods
	
	CLAMVM::SMS_DataExplorer& SegmentExplorer();

protected:
	int                       mThreadState;
	std::string               mFilename;
	std::string               mAnalysisInputFile;
	std::string               mAnalysisOutputFile;
	CLAM::Thread              mThread;
	CLAMVM::SMS_DataExplorer  mSegmentExplorer;
	UserInterface*            mUI;
};

}

#include "SMSTools_inlines.hxx"

#endif // SMSTools.hxx
