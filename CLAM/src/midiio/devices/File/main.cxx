/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "MIDIEvent.hxx"
#include "MIDITrack.hxx"
#include "MIDISong.hxx"
#include "MIDIReader.hxx"
#include "MIDISongPlayer.hxx"


using namespace MIDI;

#include "MIDITempo.hxx"

#include <math.h>
#include <stdio.h>

class SimpleSynth
{
/* a very very simple synthesizer */
private:
	struct note
	{
		int chn;
		int key;
		int vel;
		float phase;
		float amp;
		float env;
	};

	note notes[256]; /* max 256 notes simultanious */
	int  nnotes;
	
	float dphase[128]; /* table to convert midi key to phase increment */

	FILE* file;

	int findnote(int chn,int key)
	/* find an running (vel!=0) note with chn and key */
	{
		int i = 0;
		while (i<nnotes)
		{
			if (notes[i].chn==chn && notes[i].key==key && notes[i].vel!=0) return i;
			i++;
		}
		return i;
	}
	
public:
	SimpleSynth()
	{
		nnotes = 0;
		for (int i=0;i<128;i++)
		{
			float f = pow( 2. , ( float(i) - 69. ) / 12. ) * 440.;
			dphase[i] = (M_PI*2.)*f / 44100.;
  	}
		file = fopen("output.raw","wb");
	}
	
	~SimpleSynth()
	{
		fclose(file);
	}

	void setnote(int chn,int key,int vel) /* turn not on/off */
	{
		if (vel==0)
		{
			/* turn off */
			int i = findnote(chn,key);
			if (i!=nnotes) {
				notes[i].vel = 0;
			}
			return;
		}
		/* turn on: add a new note */
		notes[nnotes].chn = chn;
		notes[nnotes].key = key;
		notes[nnotes].vel = vel;
		notes[nnotes].phase = 0;
		notes[nnotes].env = 0;
		notes[nnotes].amp = float(vel)/128.;
		nnotes++;
	}
	
	void synthesize(void)
	{
		short buf[44];
		for (int j=0;j<44;j++)
		{
			buf[j] = 0;
			float out = 0;
			int i;
			for (i=0;i<nnotes;i++)
			{
				if (notes[i].vel)
				{
					if (notes[i].env<1.) notes[i].env+=0.002; /* attack */
				}else{
					if (notes[i].env>0.) notes[i].env-=0.001; /* release */
				}
				
				/* a simple FM synthesizer */
				out += sin(notes[i].phase+sin(notes[i].phase)*2.)
					*notes[i].env*notes[i].amp;
					
				notes[i].phase += dphase[notes[i].key];
				if (notes[i].phase>2.*M_PI) notes[i].phase -= 2.*M_PI;
			}
			
			/* remove "dead" notes: notes that have finished the release */
			for (i=0;i<nnotes;)
			{
				if (notes[i].vel==0 && notes[i].env<0.001)
				{
					nnotes--;
					notes[i] = notes[nnotes];
				}else{
					i++;
				}
			}
			
			buf[j] = (short)(out*2000.);
		}
		fwrite(buf,2,44,file);
	}
};

int main(int argc,char** argv)
{
	/* an example of reading a midi file */
	Reader r(argc>1 ? argv[1] : "test.mid");
	Song s;
	r.Read(s);
	
	Tempo t(&s);

#ifdef DOPRINT
	fprintf(stderr,"song has %d tracks\n",s.Tracks());
#endif
	
	int trackId;
	Event ev;
	
	SongPlayer sp(&s); /* to traverse the song's events */
	
	int last = 0;

	SimpleSynth synth; /* a simple synthesizer */

	while (sp.GetEvent(ev,trackId))
	{
#ifdef DOPRINT
		fprintf(stderr,"%d %d %d %02x %02x %02x\n",
		ev.mTicks,
		t.TicksToTime(ev.mTicks),
		trackId,
		ev.mMessage[0],
		ev.mMessage[1],
		ev.mMessage[2]);
#endif
		if ((ev[0] & 0xF0)==0x90 || (ev[0] & 0xF0)==0x80) // note on / note off
		{
			int now = t.TicksToTime(ev.GetTicks());
			while (last<now)
			{
				synth.synthesize();
				last++;
			}
			{
				synth.setnote(ev[0]&0x0F, ev[1], (ev[0] & 0xF0)==0x90 ? ev[2] : 0);
			}
		}
	}

	for (int k=0;k<1000;k++) synth.synthesize();

	return 0;
}

