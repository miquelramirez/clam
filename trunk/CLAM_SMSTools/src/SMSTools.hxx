#ifndef __SMSTools__
#define __SMSTools__

#include "NotGeneratedUserInterface.hxx"
#include "WaitMessageGUI.hxx"
#include "ProgressGUI.hxx"
#include "SMSBase.hxx"
#include "Thread.hxx"
#include "SMS_DataExplorer.hxx"
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

protected: // methods

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
