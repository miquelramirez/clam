#include <qlayout.h>
#include <qframe.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <qmessagebox.h>
#include "MelodyPlayer.hxx"
#include "MIDIMelodyPlayer.hxx"
#include "MIDISettings.hxx"
#include "QtBPFPlayer.hxx"

namespace CLAM
{
    namespace VM
    {
	const TData min_ref = 8.1758; // 8.1758Hz = key MIDI 0
	const TData local_max = 12545.9+min_ref; // 12545.9 = key MIDI 127

	QtBPFPlayer::QtBPFPlayer(QWidget* parent) 
	    : QtMultiPlayer(parent),
	      mThreadIsCancelled(false),
	      mOwnedDuration(TData(0.0)),
	      global_max(TData(1.0)), // prevent to divide by zero
	      mMustDoMapping(false),
	      mPlaySimultaneously(false)
	{
	    setFixedHeight(30);
	    AddPlayer(new MelodyPlayer());
	    AddPlayer(new MIDIMelodyPlayer());
	    BuildPlayer();
	    LoadMIDIDevices();
	    LoadMIDIInstruments();

	    mThread.SetThreadCode(makeMemberFunctor0((*this), QtBPFPlayer, thread_code));
	}
		
	QtBPFPlayer::~QtBPFPlayer()
	{
	    StopThread();
	}

	void QtBPFPlayer::SetData(const BPF& bpf)
	{
	    mOwnedBPF = bpf;
	    mThread.Start();
	}

	void QtBPFPlayer::SetAudioPtr(const Audio* audio)
	{
	    ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->SetAudioPtr(audio);
	}

	void QtBPFPlayer::SetDuration(const TData& dur)
	{
	    mOwnedDuration=dur;
	    ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->SetDuration(mOwnedDuration);
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->SetDuration(mOwnedDuration);
	}

	void QtBPFPlayer::SetPitchBounds(const TData& lowest, const TData& highest)
	{
	    TData max = local_max-min_ref;
	    TData inc = (highest-lowest)*0.1;
	    global_max = highest+lowest;
	    if(highest < TData(50.0)) 
	    {
		mMustDoMapping = true;
		return;
	    }
	    if(highest <= local_max-min_ref) return;
	    mMustDoMapping = highest > max+inc;
	}

	Melody& QtBPFPlayer::GetMelody()
	{
	    return ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->GetMelody();
	}

	MIDIMelody& QtBPFPlayer::GetMIDIMelody()
	{
	    return ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->GetMIDIMelody();
	}

	void QtBPFPlayer::updateNotePitch(int index, float newPitch)
	{
	    ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->UpdateNotePitch(TIndex(index), TData(newPitch));
	    Note note;
	    note.SetFundFreq(newPitch);
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->UpdateNoteKey(TIndex(index), note.GetNoteNumber());
	}

	void QtBPFPlayer::updateNoteDuration(int index, float beginTime)
	{
	    ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->UpdateNoteDuration(TIndex(index), TData(beginTime));
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->UpdateNoteDuration(TIndex(index), TData(beginTime));
	}

	void QtBPFPlayer::addNote(int index, float beginTime, float pitch)
	{
	    ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->AddNote(TIndex(index), TData(beginTime), TData(pitch));
	    Note note;
	    note.SetFundFreq(pitch);
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->AddNote(TIndex(index), TData(beginTime), note.GetNoteNumber());
	}

	void QtBPFPlayer::removeNote(int index)
	{
	   ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->RemoveNote(TIndex(index));
	   ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->RemoveNote(TIndex(index));
	}

	void QtBPFPlayer::SetColorMap(ColorMap map)
	{
	     switch(map)
	     {
		 case CLAM::VM::WindowMode:
		     WindowModeCM();
		     break;
		 case CLAM::VM::BlackBackground:
		     BlackBackgroundCM();
		     break;
		 case CLAM::VM::WhiteBackground:
		     WhiteBackgroundCM();
		     break;
		 default:
		     break;
	     }
	}

	void QtBPFPlayer::play()
	{
	    if(!mPlayAudio->isChecked() && !mPlayMIDI->isChecked()) return;
	    if(mPlayAudio->isChecked())
	    {
		((MelodyPlayer*)mPlayers[MELODY_PLAYER])->PlayMelody(true);
		SetCurrentPlayer(MELODY_PLAYER);
	    }
	    else if(mPlayMIDI->isChecked())
	    {
		((MelodyPlayer*)mPlayers[MELODY_PLAYER])->PlayMelody(false);
		AllPlayers(true);
	    }
	    QtMultiPlayer::play(); 
	}

	void QtBPFPlayer::uncheckAudio()
	{
	    mPlayAudio->setChecked(false);
	    mPlayMIDI->setChecked(true);
	}

	void QtBPFPlayer::uncheckMIDI()
	{
	    mPlayMIDI->setChecked(false);
	    mPlayAudio->setChecked(true);
	}

	void QtBPFPlayer::setMIDIDevice(int index)
	{
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->SetMIDIDevice(mMIDIDevices[index]);
	}

	void QtBPFPlayer::setMIDIProgram(int index)
	{
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->SetMIDIProgram(mMIDIPrograms[index]);
	}

	void QtBPFPlayer::SetCurrentPlayer(int playerID)
	{
	    mCurrentPlayer = playerID;
	    AllPlayers(false);
	}

	void QtBPFPlayer::WindowModeCM()
	{
	    QtPlayer::WindowModeCM();
	    QColor fgcolor(0,0,0);
	    mPlayAudio->setPaletteBackgroundColor(winBackground);
	    mPlayAudio->setPaletteForegroundColor(fgcolor);
	    mPlayMIDI->setPaletteBackgroundColor(winBackground);
	    mPlayMIDI->setPaletteForegroundColor(fgcolor);
	    mMIDIDevicesCB->setPaletteBackgroundColor(winBackground);
	    mMIDIDevicesCB->setPaletteForegroundColor(fgcolor);
	    mMIDIInstrumentsCB->setPaletteBackgroundColor(winBackground);
	    mMIDIInstrumentsCB->setPaletteForegroundColor(fgcolor);
	}

	void QtBPFPlayer::BlackBackgroundCM()
	{
	    QtPlayer::BlackBackgroundCM();
	    QColor bkcolor(0,0,0);
	    QColor fgcolor(255,255,255);
	    mPlayAudio->setPaletteBackgroundColor(bkcolor);
	    mPlayAudio->setPaletteForegroundColor(fgcolor);
	    mPlayMIDI->setPaletteBackgroundColor(bkcolor);
	    mPlayMIDI->setPaletteForegroundColor(fgcolor);
	    mMIDIDevicesCB->setPaletteBackgroundColor(fgcolor);
	    mMIDIDevicesCB->setPaletteForegroundColor(bkcolor);
	    mMIDIInstrumentsCB->setPaletteBackgroundColor(fgcolor);
	    mMIDIInstrumentsCB->setPaletteForegroundColor(bkcolor);
	}

	void QtBPFPlayer::WhiteBackgroundCM()
	{
	    QtPlayer::WhiteBackgroundCM();
	    QColor bkcolor(255,255,255);
	    QColor fgcolor(0,0,0);
	    mPlayAudio->setPaletteBackgroundColor(bkcolor);
	    mPlayAudio->setPaletteForegroundColor(fgcolor);
	    mPlayMIDI->setPaletteBackgroundColor(bkcolor);
	    mPlayMIDI->setPaletteForegroundColor(fgcolor);
	    mMIDIDevicesCB->setPaletteBackgroundColor(bkcolor);
	    mMIDIDevicesCB->setPaletteForegroundColor(fgcolor);
	    mMIDIInstrumentsCB->setPaletteBackgroundColor(bkcolor);
	    mMIDIInstrumentsCB->setPaletteForegroundColor(fgcolor);
	}

	void QtBPFPlayer::BuildMelodies()
	{
	    // disable widget: wait while building melodies
	    this->setEnabled(false);

	    // informative tooltip
	    QToolTip::add(this,"Please wait: disabled while building a huge melody.");
	    
	    Melody melody;
	    MIDIMelody midiMelody;
	    melody.AddNumberOfNotes();
	    midiMelody.AddNumberOfNotes();
	    melody.UpdateData();
	    midiMelody.UpdateData();
	    melody.SetNumberOfNotes(0);
	    midiMelody.SetNumberOfNotes(0);

	    if(mOwnedBPF.Size() && mOwnedDuration < mOwnedBPF.GetXValue(mOwnedBPF.Size()-1)) 
	    {
		mOwnedDuration = mOwnedBPF.GetXValue(mOwnedBPF.Size()-1)+TData(0.2);
	    }
	    List<Note> notes;
	    List<MIDINote> midiNotes;
	    for(TIndex i=0; i < mOwnedBPF.Size(); i++)
	    {
		if(mThreadIsCancelled) break;

		MediaTime time;
		Note note;
		MIDINote midiNote;
		time.SetBegin(mOwnedBPF.GetXValue(i));
		if(i == mOwnedBPF.Size()-1)
		{
		    time.SetEnd(mOwnedDuration);
		}
		else
		{
		    time.SetEnd(mOwnedBPF.GetXValue(i+1));
		}

		TData pitch = mOwnedBPF.GetValueFromIndex(i);

		if(mMustDoMapping)
		{
		    pitch = pitch*local_max/global_max+min_ref;
		}

		if(pitch < min_ref)
		{
		    pitch = min_ref;
		}
		else if(pitch > local_max-min_ref)
		{
		    pitch = local_max-min_ref;
		}
		  
		note.SetFundFreq(pitch);
		note.SetTime(time);
		midiNote.SetKey(note.GetNoteNumber());
		midiNote.SetVelocity(120);
		midiNote.SetTime(note.GetTime());

		notes.AddElem(note);
		midiNotes.AddElem(midiNote);
	    }

	    if(!mThreadIsCancelled)
	    {
		melody.SetNoteArray(notes);
		melody.SetNumberOfNotes(notes.Size());
		midiMelody.SetNoteArray(midiNotes);
		midiMelody.SetNumberOfNotes(midiNotes.Size()); 

		// set players' data
		((MelodyPlayer*)mPlayers[MELODY_PLAYER])->SetData(melody,mOwnedDuration);
		((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->SetData(midiMelody, 
								    mMIDIDevices[mMIDIDevicesCB->currentItem()],
								    mMIDIPrograms[mMIDIInstrumentsCB->currentItem()],
								    mOwnedDuration);
	    }
	    // remove informative tooltip
	    QToolTip::remove(this);

	    // enable widget when the melodies are fully loaded
	    this->setEnabled(true);
	}

	void QtBPFPlayer::BuildPlayer()
	{
       
	    QFont f("Sans",8);
	    f.setFamily("fixed");
	    f.setPointSize(10);
	    f.setBold(true);
	    f.setStyleHint(QFont::Courier,QFont::NoAntialias);
	    
	    QFontMetrics fm(f);
	    int audioLabelWidth = fm.width("Audio");
	    int midiLabelWidth = fm.width("MIDI");

	    // play mode panel
	    radioPanel = new QFrame(this);

	    mPlayAudio = new QRadioButton(radioPanel);
	    mPlayAudio->setChecked(true); // defaut 
	    mPlayAudio->setFont(f);
	    mPlayAudio->setText("Audio");
	    mPlayAudio->setGeometry(0,1,audioLabelWidth+20,mPlayAudio->height());
	    QToolTip::add(mPlayAudio,"Play Audio");

	    mPlayMIDI = new QRadioButton(radioPanel);
	    mPlayMIDI->setFont(f);
	    mPlayMIDI->setText("MIDI");
	    mPlayMIDI->setGeometry(mPlayAudio->width()+10,mPlayAudio->y(),midiLabelWidth+20,mPlayMIDI->height());
	    QToolTip::add(mPlayMIDI,"Play MIDI");

	    radioPanel->setFixedSize(mPlayMIDI->x()+mPlayMIDI->width(),30);

	    layout->addStretch(1);
	    layout->addWidget(radioPanel);
	    layout->addStretch(1);

	    // MIDI settings panel
	    midiSettingsPanel = new QFrame(this);
	    
	    mMIDIInstrumentsCB = new QComboBox(midiSettingsPanel);
	    mMIDIInstrumentsCB->setGeometry(0,5,85,20);
	    QToolTip::add(mMIDIInstrumentsCB,"MIDI Instruments");

	    mMIDIDevicesCB = new QComboBox(midiSettingsPanel);
	    mMIDIDevicesCB->setGeometry(mMIDIInstrumentsCB->width()+5,mMIDIInstrumentsCB->y(),
					mMIDIInstrumentsCB->width(),mMIDIInstrumentsCB->height());
	    QToolTip::add(mMIDIDevicesCB,"MIDI Devices");
	    
	    midiSettingsPanel->setFixedSize(mMIDIDevicesCB->x()+mMIDIDevicesCB->width(),30);

	    layout->addWidget(midiSettingsPanel);

	    // connections
	    connect(mPlayAudio,SIGNAL(clicked()),this,SLOT(uncheckMIDI()));
	    connect(mPlayMIDI,SIGNAL(clicked()),this,SLOT(uncheckAudio()));

	    connect(mMIDIDevicesCB,SIGNAL(highlighted(int)),this,SLOT(setMIDIDevice(int)));
	    connect(mMIDIInstrumentsCB,SIGNAL(highlighted(int)),this,SLOT(setMIDIProgram(int)));
	    
	}

	void QtBPFPlayer::LoadMIDIDevices()
	{
	    MIDISettings settings;
	    mMIDIDevices = settings.GetMIDIDevices();
	    if(!mMIDIDevices.size())
	    {
		QMessageBox message("MIDI support not available",
				    "Seems that your system has not MIDI support. MIDI settings will be deactivated",
				    QMessageBox::Warning,
				    QMessageBox::Ok,
				    QMessageBox::NoButton,
				    QMessageBox::NoButton); 

			message.exec();
	    }
	    else
	    {
		std::vector<std::string> deviceNames = settings.GetMIDIDeviceNameLst();
		std::vector<std::string>::iterator it = deviceNames.begin();
		for(int index=0; it != deviceNames.end(); it++, index++)
		{
		    mMIDIDevicesCB->insertItem(QString((*it).c_str()),index);
		}
		// set default MIDI device
		((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->SetMIDIDevice(mMIDIDevices[mMIDIDevicesCB->currentItem()]);
	    }

	    if(!mMIDIDevices.size())
	    {
		mPlayMIDI->setEnabled(false);
		mMIDIDevicesCB->setEnabled(false);
	    }
	}

	void QtBPFPlayer::LoadMIDIInstruments()
	{
	    if(!mMIDIDevices.size()) 
	    {
		mMIDIInstrumentsCB->setEnabled(false);
		return;
	    }

	    MIDISettings settings;
	    mMIDIPrograms = settings.GetAvailableMIDIPrograms();
	    std::vector<std::string> programNames = settings.GetAvailableMIDIProgramNameLst();
	    std::vector<std::string>::iterator it = programNames.begin();
	    for(int index=0; it != programNames.end(); it++, index++)
	    {
		mMIDIInstrumentsCB->insertItem(QString((*it).c_str()),index);
	    }
	    // set default MIDI program
	    ((MIDIMelodyPlayer*)mPlayers[MIDI_PLAYER])->SetMIDIProgram(mMIDIPrograms[mMIDIInstrumentsCB->currentItem()]);
	}

	void QtBPFPlayer::thread_code()
	{
	    // is better buid the melodies on a second plane (maybe the player gets a huge bpf)
	    BuildMelodies();
	}

	void QtBPFPlayer::StopThread()
	{
	    mThreadIsCancelled = true;
	    if(mThread.IsRunning()) mThread.Stop();
	}

	void QtBPFPlayer::PlaySimultaneously(bool psi)
	{
	    mPlaySimultaneously = psi;
	    ((MelodyPlayer*)mPlayers[MELODY_PLAYER])->PlayAudio(mPlaySimultaneously);
	}
    }
}

// END
