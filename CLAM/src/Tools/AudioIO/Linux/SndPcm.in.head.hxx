/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <alsa/asoundlib.h>
#include <Err.hxx>

class SndPcmError : public CLAM::Err
{
public:
	SndPcmError(const char* _str)
	:Err(_str){

	}
};

class SndPcm
{
public:
	snd_pcm_format_t format;
	int rate;
	int channels;
	int latency;
	int latency_min;		/* in frames / 2 */
	int latency_max;		/* in frames / 2 */
	int block;					/* block mode */
	int tick_time;			/* disabled, otherwise in us */
	int tick_time_ok;

	snd_pcm_t *phandle, *chandle;

	char error_str[1024];
	void cat_error(const char* fmt,...);

	void ReadBuf(short* data)
	{
		int res = readbuf(chandle,(char*) data,latency);
		if (res>=0) return;
		if (res == -EPIPE)
//			throw(SndPcmError("SndPcm::ReadBuf() Buffer Overrun!\n"));
			RecoverXRun(data);
	}
	void WriteBuf(short* data)
	{
		int res = writebuf(phandle,(char*) data,latency);
		if (res>=0) return;
		if (res == -EPIPE)
//			throw(SndPcmError("SndPcm::WriteBuf() Buffer Underrun!\n"));
			RecoverXRun(data);
	}
	void ReadBuf(short* data,int len)
	{
		int res = readbuf(chandle,(char*) data,len);
		if (res>=0) return;
		if (res == -EPIPE)
//			throw(SndPcmError("SndPcm::ReadBuf() Buffer Overrun!\n"));
			RecoverXRun(data);
	}
	void WriteBuf(short* data,int len)
	{
		int res = writebuf(phandle,(char*) data,len);
		if (res>=0) return;
		if (res == -EPIPE)
//			throw(SndPcmError("SndPcm::WriteBuf() Buffer Underrun!\n"));
			RecoverXRun(data);
	}
	SndPcm(int irate,int ichannels,int ilatency,
		const char* pdevice,const char* cdevice);
	~SndPcm();

	void Start(void);
	void Stop(void);
	void RecoverXRun(short* data);
	
	void Poll(void);

private:
