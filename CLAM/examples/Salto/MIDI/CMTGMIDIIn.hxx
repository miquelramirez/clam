#ifndef __CMTGMIDIIn__
#define __CMTGMIDIIn__

#ifdef ALSAMIDI
//#include <sys/asoundlib.h>
#include <alsa/asoundlib.h>
#endif

class CMTGMIDITarget;

#include <sys/time.h>
#include "MTGDataTypes.hxx"

/**
* Channel message IDs. This corresponds with the 4 most significant 
* bits of the MIDI Channel message.
*/
enum EMTGChnMsg {
    kMTGNoteOff = 0,
    kMTGNoteOn = 1,
    kMTGPolyAftertouch = 2,
    kMTGControlChange = 3,
    kMTGModeChange = 4,
    kMTGAftertouch = 5,
    kMTGPitchbend = 6,
    kMTGSystem = 7
};

/**
* System message IDs. This corresponds with the 4 least significant 
* bits of the MIDI System message (#Fx).
*/
enum EMTGSysMsg {
    kMTGExclusiveStart = 0,
    kMTGQuarterFrams = 1,
    kMTGSongPosition = 2,
    kMTGSongSelect = 3,
    kMTGUndefined1 = 4,
    kMTGUndefined2 = 5,
    kMTGTuneRequest = 6,
    kMTGExclusiveEnd = 7,
    kMTGTimingClock = 8,
    kMTGTimingTick = 9,
    kMTGStart = 10,
    kMTGContinue = 11,
    kMTGStop = 12,
    kMTGUndefined3 = 13,
    kMTGActiveSensing = 14,
    kMTGSystemReset = 15
};

/**
* Convert the first byte of a MIDI Channel Message to a EMTGChnMsg.
* @param b The midi byte
*/
inline EMTGChnMsg EMTGMIDIChnMsgToChnMsg(const TMIDIByte& b)
{
	return (EMTGChnMsg)((b >> 4)&7);
}

/**
* Convert the first byte of a MIDI Channel Message to a EMTGSysMsg.
* @param b The midi byte, which should be have the hexadecimal value #Fx
*/
inline EMTGSysMsg EMTGMIDIChnMsgToSysMsg(const TMIDIByte& b)
{
	return (EMTGSysMsg)(b&15);
}

/**
* Platform independent representation of a MIDI capable soundcard.
*/
class CMTGMIDICard
{
	friend class CMTGMIDISettings;
	friend class CMTGMIDIIn;
protected:
	int mId;
	
public:
	CMTGMIDICard(const int& id = 0): mId(id)
  {
    Init();
  }
	~CMTGMIDICard()
  {
    Exit();
  }
  void Init(void);
  void Exit(void);
};

/**
* Platform independent representation of a MIDI device.
*/
class CMTGMIDIDevice
{
	friend class CMTGMIDISettings;
	friend class CMTGMIDIIn;
protected:
	int mId;
public:
	CMTGMIDIDevice(const int& id = 0): mId(id)
  {
  }
};

/**
* Settings for MIDI I/O.
*/
class CMTGMIDISettings
{
	friend class CMTGMIDIIn;
protected:
	//* The sound card to use for the MIDI I/O
	const CMTGMIDICard &mCard;
	//* The midi device on the sound card to use for the MIDI I/O
	const	CMTGMIDIDevice &mDevice;
	bool mBlockmode;
public:
	CMTGMIDISettings(
	    const CMTGMIDICard& card, const CMTGMIDIDevice& device, bool blockmode = true);
};

/**
* Platform independent representation of a source of realtime MIDI input.
*/
class CMTGMIDIIn
{
protected:
	const CMTGMIDISettings mSettings;
#ifdef ALSAMIDI
	snd_rawmidi_t *mHandleIn;
#endif
	TMIDITime mStartTime;
	CMTGMIDITarget* mpTarget;
public:
	static const int sNBytesPerChnMsg[];
	static const int sNBytesPerSysMsg[];

	CMTGMIDIIn(const CMTGMIDISettings& settings, CMTGMIDITarget& target);
	virtual ~CMTGMIDIIn();

	//* Read data from the MIDI input, and pass it to the mpTarget
  void Read(void);

	/** Current time, used by Read() to set the timetag of the incoming
	* MIDI data
	*/
	TMIDITime Time(void);
	
	void EnableMIDIIn(void);
	void DisableMIDIIn(void);
	
};


inline TMIDITime CMTGMIDIIn::Time(void)
{
#ifdef ALSAMIDI
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec*1000 + tv.tv_usec / 1000;
#endif
  return 0;
}

#endif
