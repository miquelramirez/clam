#ifndef __ENGINE__
#define __ENGINE__

#include <string>
#include <CLAM/Thread.hxx>
#include "QtProgress.hxx"
#include "QtWaitMessage.hxx"
#include "SMSBase.hxx"
#include <CLAM/Signalv0.hxx>
#include <CLAM/Slotv0.hxx>

namespace QtSMS
{
	class ViewManager;

	class Engine : public CLAM::SMSBase
	{
	public:
		static Engine* Instance();
		static ViewManager* DisplayManager();

	    bool LoadConfiguration(const std::string& filename);
		void StoreConfiguration(const std::string& filename);
		bool LoadAnalysis(const std::string& filename);
		void StoreAnalysis(const std::string& filename);
		void StoreMelody(const std::string& filename);
		void LoadTransformationScore(const std::string& filename);
		void StoreTransformationScore(const std::string& filename);
		void StoreOutputSound(const std::string& filename);
		void StoreOutputSoundResidual(const std::string& filename);
		void StoreOutputSoundSinusoidal(const std::string& filename);
		void Analyze();
		void ExtractMelody();
		void DoTransformations();
		void Synthesize();

		bool HasValidAudioInput();
		void RetrieveAudio(bool r);
		void ConnectSlotAnalysisDataLoaded(SigSlot::Slotv0& slot);

	protected:
		void Run(){ /* empty body */ }

		CLAMGUI::Progress* CreateProgress(const char* title,float from,float to){
			return new CLAMGUI::QtProgress(title,from,to);
		}

		CLAMGUI::WaitMessage* CreateWaitMessage(const char* title){
			return new CLAMGUI::QtWaitMessage(title);
		}

	private:
		Engine();
			
		static Engine* mInstance;

		CLAM::Thread mThread;
		std::string  mCurrentFileName;
		bool         mRetrieveAudio;

		SigSlot::Signalv0 mAnalysisDataLoaded;

	    bool LoadInputSound();
		bool LoadSound(const std::string& filename,CLAM::Segment& segment);
		void StoreSound(const CLAM::Audio& audio,const std::string& filename);

		void LaunchMethodOnThread(CBL::Functor0 method);

		void DoLoadAnalysis();
		void DoStoreAnalysis();
		void DoStoreMelody();
		void DoMelodyExtraction();
	};
}



#endif

