#ifndef __QTBPFPLAYER__
#define __QTBPFPLAYER__

#include <queue>
#include <string>
#include "Thread.hxx"
#include "BPF.hxx"
#include "Melody.hxx"
#include "MIDIMelody.hxx"
#include "QtMultiPlayer.hxx"

class QRadioButton;
class QComboBox;

namespace CLAM
{
    class Audio;

    namespace VM
    {
		class QtBPFPlayer : public QtMultiPlayer
		{
			typedef std::queue<std::string> Q_Keys;
			typedef std::queue<BPF>         Q_BPFs;

			Q_OBJECT
		public:
			QtBPFPlayer(QWidget* parent=0);
			~QtBPFPlayer();

			void AddData(const std::string& key, const BPF& bpf);
			Melody& GetMelody(const std::string& key);
			MIDIMelody& GetMIDIMelody(const std::string& key);

			void SetAudioPtr(const Audio* audio);
			void SetDuration(const TData& dur);
	    
			void SetPitchBounds(const TData& lowest, const TData& highest);
			void SetColorMap(ColorMap map);
			void StopThread();
			void PlaySimultaneously(bool psi);

			void SetCurrentBPF(const std::string& current);

		public slots:
			void play();
			void updateNotePitch(int, float);
			void updateNoteDuration(int, float);
			void addNote(int, float, float);
			void removeNote(int);

		private slots:
			void uncheckAudio();
			void uncheckMIDI();
			void setMIDIDevice(int);
			void setMIDIProgram(int);

		protected:
			void SetCurrentPlayer(int playerID);
			void WindowModeCM();
			void BlackBackgroundCM();
			void WhiteBackgroundCM();

		private:
			Thread mThread;
			bool   mThreadIsCancelled;
			BPF    mOwnedBPF;
			TData  mOwnedDuration;
			TData  global_max;
			bool   mMustDoMapping;
			bool   mPlaySimultaneously;

			QFrame *radioPanel,*midiSettingsPanel;
			QRadioButton *mPlayAudio, *mPlayMIDI;
			QComboBox *mMIDIInstrumentsCB, *mMIDIDevicesCB;

			std::vector<int>         mMIDIPrograms;
			std::vector<std::string> mMIDIDevices;

			Q_Keys mKeys;
			Q_BPFs mBPFs;

			enum { MELODY_PLAYER=0, MIDI_PLAYER };
	    
			void BuildMelodies();
			void BuildPlayer();
			void LoadMIDIDevices();
			void LoadMIDIInstruments();
			void InitialMIDISettings();
			void thread_code();

			void ProcessIncomingBPF();
			void CheckPendent();

		};
    }
}

#endif
