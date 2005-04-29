#ifndef __QTBPFPLAYER__
#define __QTBPFPLAYER__

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
	    Q_OBJECT
	public:
	    QtBPFPlayer(QWidget* parent=0);
	    ~QtBPFPlayer();

	    void SetData(const BPF& bpf);
	    void SetAudioPtr(const Audio* audio);

	    void SetDuration(const TData& dur);
	    
	    Melody& GetMelody();
	    MIDIMelody& GetMIDIMelody();

	    void SetPitchBounds(const TData& lowest, const TData& highest);

	    void SetColorMap(ColorMap map);

	    void StopThread();

	    void PlaySimultaneously(bool psi);

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
	    bool mThreadIsCancelled;
	    BPF mOwnedBPF;
	    TData mOwnedDuration;
	    TData global_max;
	    bool mMustDoMapping;
	    bool mPlaySimultaneously;

	    QFrame *radioPanel,*midiSettingsPanel;
	    QRadioButton *mPlayAudio, *mPlayMIDI;
	    QComboBox *mMIDIInstrumentsCB, *mMIDIDevicesCB;

	    std::vector<int> mMIDIPrograms;
	    std::vector<std::string> mMIDIDevices;

	    enum { MELODY_PLAYER=0, MIDI_PLAYER };
	    
	    void BuildMelodies();

	    void BuildPlayer();

	    void LoadMIDIDevices();
	    void LoadMIDIInstruments();

	    void thread_code();

	};
    }
}

#endif
