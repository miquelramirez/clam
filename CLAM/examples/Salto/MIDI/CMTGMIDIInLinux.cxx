#include "CMTGErr.hxx"
#include "CMTGMIDIIn.hxx"
#include "CMTGMIDITarget.hxx"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream.h>

#define WORKAROUND

void CMTGMIDICard::Init(void)
{
}

void CMTGMIDICard::Exit(void)
{
}

const int CMTGMIDIIn::sNBytesPerChnMsg[] = { 3, 3, 3, 3, 2, 3, 3, 0 };
const int CMTGMIDIIn::sNBytesPerSysMsg[] = { 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

CMTGMIDIIn::CMTGMIDIIn(const CMTGMIDISettings& settings, CMTGMIDITarget& target)
		: mSettings(settings), mpTarget(&target)
{
	// whether or not to use block mode probably depends on
	// the environment your working in, whether you use
	// multithreading, etc
	int err;

	char* devid = "hw:0,0";	
	
	err = snd_rawmidi_open(&mHandleIn, NULL, devid, O_RDONLY);
	if (err)
	{
		char tmp[256];
		sprintf(tmp, "snd_rawmidi_open %d %d failed: %d\n",
		        mSettings.mCard.mId, mSettings.mDevice.mId, err);
		throw CMTGErr(tmp);
	}
	mStartTime = Time();
}

CMTGMIDIIn::~CMTGMIDIIn()
{
	snd_rawmidi_close(mHandleIn);
}

////Change CMTGMIDIIn because now blocks execution (add Target)
void CMTGMIDIIn::Read(void)
{
	TMIDIByte bytes[1024];
	TMIDIByte byte, cmbyte;
	int i = 1, n;
	while (
	    (n = snd_rawmidi_read(mHandleIn, &byte, 1) )  == 1
#ifdef WORKAROUND 
	    && byte != 0xFF
#endif
	)
	{
////		printf("read %d bytes", n);
////		printf("read %02x\n", byte);
		TMIDITime millisecs = Time();
		if (byte&0x80)
		{
			bytes[0] = byte;
			cmbyte = byte;
			i = 1;
		}
		else
		{
			bytes[0] = cmbyte;
			bytes[1] = byte;
			i = 2;
		}
		EMTGChnMsg cm = EMTGMIDIChnMsgToChnMsg(cmbyte);
		int nBytes;
		if (cm == kMTGSystem)
		{
			EMTGSysMsg sm = EMTGMIDIChnMsgToSysMsg(cmbyte);
			nBytes = sNBytesPerSysMsg[sm];
		}
		else
		{
			nBytes = sNBytesPerChnMsg[cm];
		}
		if (nBytes > 1)
		{
			int todo = nBytes - i;
			int cnt = 0;
			while (todo > 0)
			{
				int n = snd_rawmidi_read(mHandleIn, &bytes[i], 1);
				if (n == 1)
				{
#ifdef WORKAROUND
					if (bytes[i] == 0xFF)
					{
						n = 0;
					}
#endif

					todo -= n;
					i += n;
				}
				cnt++;   // american safety
				if (cnt > 10000) throw(CMTGErr("SAFETY LIMIT REACHED"));
			}
		}
		if (cm == kMTGNoteOn && bytes[2] == 0)
		{
			bytes[0] &= 0x8F;
		}
		if (mpTarget) mpTarget->Handle(millisecs - mStartTime, bytes, nBytes);
	}
	if (n < 0)
	{
		if (n==-11 && !mSettings.mBlockmode) {
			/* if we are in non-block mode, read returns -11 on some soundcards, for which we
			should not throw an error */
			return;
		}
		char tmp[256];
		sprintf(tmp, "midi read %d %d failed: %s\n",
		        mSettings.mCard.mId, mSettings.mDevice.mId, strerror(-n));
		throw CMTGErr(tmp);
	}
}

