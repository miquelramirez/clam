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

#include "SndPcm.hxx"
#include <stdarg.h>

#include <stdarg.h>

void SndPcm::cat_error(const char* fmt,...)
{
	int len = strlen(error_str);
	int rem = 1024 - len;
	if (rem>0) {
		va_list ap;
		va_start(ap,fmt);
		vsnprintf(error_str+len,rem,fmt,ap);
		va_end(ap);
	}
}

	
SndPcm::SndPcm(int irate,int ichannels,int ilatency,
	const char* pdevice,const char* cdevice)
{
	format = SND_PCM_FORMAT_S16_LE;
	rate = irate;
	channels = ichannels;
	latency_min = ilatency;
	latency_max = ilatency*2;
	block = 0;
	tick_time = 0;
	tick_time_ok = 0;
	error_str[0]=0;
	int err;

	if ((err = snd_pcm_open(&phandle, pdevice, SND_PCM_STREAM_PLAYBACK, block ? 0 : SND_PCM_NONBLOCK)) < 0) {
		cat_error("SndPcm::SndPcm(...): Playback open error: %s\n", snd_strerror(err));
		throw SndPcmError(error_str);
	}
	if ((err = snd_pcm_open(&chandle, cdevice, SND_PCM_STREAM_CAPTURE, block ? 0 : SND_PCM_NONBLOCK)) < 0) {
		cat_error("SndPcm::SndPcm(...): Record open error: %s\n", snd_strerror(err));
		throw SndPcmError(error_str);
	}

	latency = latency_min - 4;
	if (setparams(phandle, chandle, &latency) < 0)
		throw SndPcmError(error_str);
}			

SndPcm::~SndPcm()
{
	snd_pcm_hw_free(phandle);
	snd_pcm_hw_free(chandle);
	snd_pcm_close(phandle);
	snd_pcm_close(chandle);
}

void SndPcm::Start(void)
{
	int err;
	char buffer[1024];
	
	int nSilentBlockSamples = snd_pcm_bytes_to_samples(chandle,1024);
	int nSilentBlockFrames = snd_pcm_bytes_to_frames(chandle,1024);
	
	if ((err = snd_pcm_link(chandle, phandle)) < 0) {
		cat_error("Streams link error: %s\n", snd_strerror(err));
		throw SndPcmError(error_str);
	}
	if (snd_pcm_format_set_silence(format, buffer, nSilentBlockSamples) < 0) {
		cat_error("silence error\n");
		throw SndPcmError(error_str);
	}
	int n = latency*2; /* write two silent buffers */
	while (n)
	{
		int m = n;
		if (m>nSilentBlockFrames) m = nSilentBlockFrames;
		if (writebuf(phandle, buffer, m) < 0) {
			cat_error("write error\n");
			throw SndPcmError(error_str);
		}
		n -= m;
	}

	if ((err = snd_pcm_start(chandle)) < 0) {
		cat_error("Go error: %s\n", snd_strerror(err));
		throw SndPcmError(error_str);
	}
}

void SndPcm::Stop(void)
{
	snd_pcm_drop(chandle);
	snd_pcm_nonblock(phandle, 0);
	snd_pcm_drain(phandle);
	snd_pcm_nonblock(phandle, !block ? 1 : 0);
	snd_pcm_unlink(chandle);
}

void SndPcm::RecoverXRun(short* data)
{
	int err;
	
	snd_pcm_drop(chandle);
	snd_pcm_drop(phandle);

	putchar('.');

	latency = latency_min - 4;
	if (setparams(phandle, chandle, &latency) < 0)
		throw SndPcmError(error_str);

	if (writebuf(phandle,(char*) data, latency) < 0) {
		cat_error("write error\n");
		throw SndPcmError(error_str);
	}
	if (writebuf(phandle,(char*) data, latency) < 0) {
		cat_error("write error\n");
		throw SndPcmError(error_str);
	}

	if ((err = snd_pcm_start(chandle)) < 0) {
		cat_error("Go error: %s\n", snd_strerror(err));
		throw SndPcmError(error_str);
	}

}


void SndPcm::Poll(void)
{
	snd_pcm_wait(chandle, 1000);
}
