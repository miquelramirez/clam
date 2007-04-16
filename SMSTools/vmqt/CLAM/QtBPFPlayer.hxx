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
			struct DataInfo
			{
				std::string key;
				BPF         data;
				TData       min;
				TData       max;
				bool        mapping;

				DataInfo()
					: key("default")
					, min(TData(0.0))
					, max(TData(1.0))
					, mapping(false)
					{
					}
				DataInfo(const std::string& key_, 
						 const BPF& bpf_, 
						 const TData& min_, 
						 const TData& max_, 
						 const bool& mapping_)
					: key(key_)
					, data(bpf_)
					, min(min_)
					, max(max_)
					, mapping(mapping_)
					{
					}
				DataInfo(const DataInfo& dataInfo)
					: key(dataInfo.key)
					, data(dataInfo.data)
					, min(dataInfo.min)
					, max(dataInfo.max)
					, mapping(dataInfo.mapping)
					{
					}
				DataInfo& operator=(const DataInfo& dataInfo)
					{
						key = dataInfo.key;
						data = dataInfo.data;
						min = dataInfo.min;
						max = dataInfo.max;
						mapping = dataInfo.mapping;
						return *this;
					}
			};

			typedef std::queue<DataInfo> IncomingData;

			Q_OBJECT
		public:
			QtBPFPlayer(QWidget* parent=0);
			~QtBPFPlayer();

			void AddData(const std::string& key, const BPF& bpf);
			Melody& GetMelody(const std::string& key);
			MIDIMelody& GetMIDIMelody(const std::string& key);

			void SetAudioPtr(const Audio* audio);
			void SetDuration(const TData& dur);
	    
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
			TData  mOwnedDuration;
			bool   mPlaySimultaneously;

			QFrame *radioPanel,*midiSettingsPanel;
			QRadioButton *mPlayAudio, *mPlayMIDI;
			QComboBox *mMIDIInstrumentsCB, *mMIDIDevicesCB;

			std::vector<int>         mMIDIPrograms;
			std::vector<std::string> mMIDIDevices;

			IncomingData mEnqueuedData;

			enum { MELODY_PLAYER=0, MIDI_PLAYER };
	    
			void BuildMelodies();
			void BuildPlayer();
			void LoadMIDIDevices();
			void LoadMIDIInstruments();
			void thread_code();

			void ProcessIncomingBPF();
			void CheckPendent();

			TData GetMinY(const BPF& bpf);
			TData GetMaxY(const BPF& bpf);
			
			void EnqueueData(const std::string& key, const BPF& bpf);

		};
    }
}

#endif

