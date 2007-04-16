/*
DON'T EDIT THIS FILE BY HAND, IT IS GENERATED 
BY THE SCRIPT SndPcmGen.pl FROM
alsa-lib/test/latency.c
SndPcm.cxx.in.head
SndPcm.cxx.in.tail
SndPcm.hxx.in.head
SndPcm.hxx.in.tail
*/

/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
/*
 * The functions which follow are taken from the latency test included
 * in the ALSA source distribution, with the following copyright note:
 *
 *  Latency test program
 *
 *     Author: Jaroslav Kysela <perex@suse.cz>
 *
 *     Author of bandpass filter sweep effect:
 *	       Maarten de Boer <mdeboer@iua.upf.es>
 *
 *  This small demo program can be used for measuring latency between
 *  capture and playback. This latency is measured from driver (diff when
 *  playback and capture was started). Scheduler is set to SCHED_RR.
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
int SndPcm::setparams_stream(snd_pcm_t *handle,
		     snd_pcm_hw_params_t *params,
		     const char *id)
{
	int err;

	err = snd_pcm_hw_params_any(handle, params);
	if (err < 0) {
		cat_error("Broken configuration for %s PCM: no configurations available: %s\n", snd_strerror(err), id);
		return err;
	}
	err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		cat_error("Access type not available for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_set_format(handle, params, format);
	if (err < 0) {
		cat_error("Sample format not available for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_set_channels(handle, params, channels);
	if (err < 0) {
		cat_error("Channels count (%i) not available for %s: %s\n", channels, id, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_set_rate_near(handle, params, rate, 0);
	if (err < 0) {
		cat_error("Rate %iHz not available for %s: %s\n", rate, id, snd_strerror(err));
		return err;
	}
	if (err != rate) {
		cat_error("Rate doesn't match (requested %iHz, get %iHz)\n", rate, err);
		return -EINVAL;
	}
	return 0;
}

int SndPcm::setparams_bufsize(snd_pcm_t *handle,
		      snd_pcm_hw_params_t *params,
		      snd_pcm_hw_params_t *tparams,
		      snd_pcm_uframes_t bufsize,
		      const char *id)
{
	int err;
	snd_pcm_uframes_t periodsize;

	snd_pcm_hw_params_copy(params, tparams);
	err = snd_pcm_hw_params_set_buffer_size_near(handle, params, bufsize * 2);
	if (err < 0) {
		cat_error("Unable to set buffer size %li for %s: %s\n", bufsize * 2, id, snd_strerror(err));
		return err;
	}
	periodsize = snd_pcm_hw_params_get_buffer_size(params) / 2;
	err = snd_pcm_hw_params_set_period_size_near(handle, params, periodsize, 0);
	if (err < 0) {
		cat_error("Unable to set period size %li for %s: %s\n", periodsize, id, snd_strerror(err));
		return err;
	}
	return 0;
}

int SndPcm::setparams_set(snd_pcm_t *handle,
		  snd_pcm_hw_params_t *params,
		  snd_pcm_sw_params_t *swparams,
		  const char *id)
{
	int err, val, sleep_min = 0;

	err = snd_pcm_hw_params(handle, params);
	if (err < 0) {
		cat_error("Unable to set hw params for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	err = snd_pcm_sw_params_current(handle, swparams);
	if (err < 0) {
		cat_error("Unable to determine current swparams for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	err = snd_pcm_sw_params_set_start_threshold(handle, swparams, 0x7fffffff);
	if (err < 0) {
		cat_error("Unable to set start threshold mode for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	tick_time_ok = 0;
	if (tick_time > 0) {
		int time, ttime;
		time = snd_pcm_hw_params_get_period_time(params, NULL);
		ttime = snd_pcm_hw_params_get_tick_time(params, NULL);
		if (time < ttime) {
			cat_error("Skipping to set minimal sleep: period time < tick time\n");
		} else if (ttime <= 0) {
			cat_error("Skipping to set minimal sleep: tick time <= 0 (%i)\n", ttime);
		} else {
			sleep_min = tick_time / ttime;
			if (sleep_min <= 0)
				sleep_min = 1;
			err = snd_pcm_sw_params_set_sleep_min(handle, swparams, sleep_min);
			if (err < 0) {
				cat_error("Unable to set minimal sleep %i for %s: %s\n", sleep_min, id, snd_strerror(err));
				return err;
			}
			tick_time_ok = sleep_min * ttime;
		}
	}
	val = !block ? 4 : snd_pcm_hw_params_get_period_size(params, NULL);
	if (tick_time_ok > 0)
		val = 16;
	val = snd_pcm_hw_params_get_period_size(params, NULL);
	err = snd_pcm_sw_params_set_avail_min(handle, swparams, val);
	if (err < 0) {
		cat_error("Unable to set avail min for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	val = !block ? 4 : 1;
	err = snd_pcm_sw_params_set_xfer_align(handle, swparams, val);
	if (err < 0) {
		cat_error("Unable to set transfer align for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	err = snd_pcm_sw_params(handle, swparams);
	if (err < 0) {
		cat_error("Unable to set sw params for %s: %s\n", id, snd_strerror(err));
		return err;
	}
	return 0;
}

int SndPcm::setparams(snd_pcm_t *phandle, snd_pcm_t *chandle, int *bufsize)
{
	int err, last_bufsize = *bufsize;
	snd_pcm_hw_params_t *pt_params, *ct_params;	/* templates with rate, format and channels */
	snd_pcm_hw_params_t *p_params, *c_params;
	snd_pcm_sw_params_t *p_swparams, *c_swparams;
	snd_pcm_sframes_t size;

	snd_pcm_hw_params_alloca(&p_params);
	snd_pcm_hw_params_alloca(&c_params);
	snd_pcm_hw_params_alloca(&pt_params);
	snd_pcm_hw_params_alloca(&ct_params);
	snd_pcm_sw_params_alloca(&p_swparams);
	snd_pcm_sw_params_alloca(&c_swparams);
	if ((err = setparams_stream(phandle, pt_params, "playback")) < 0) {
		cat_error("Unable to set parameters for playback stream: %s\n", snd_strerror(err));
		return -1;;
	}
	if ((err = setparams_stream(chandle, ct_params, "capture")) < 0) {
		cat_error("Unable to set parameters for playback stream: %s\n", snd_strerror(err));
		return -1;;
	}

      __again:
      	if (last_bufsize == *bufsize)
		*bufsize += 4;
	last_bufsize = *bufsize;
	if (*bufsize > latency_max)
		return -1;
	if ((err = setparams_bufsize(phandle, p_params, pt_params, *bufsize, "playback")) < 0) {
		cat_error("Unable to set sw parameters for playback stream: %s\n", snd_strerror(err));
		return -1;;
	}
	if ((err = setparams_bufsize(chandle, c_params, ct_params, *bufsize, "capture")) < 0) {
		cat_error("Unable to set sw parameters for playback stream: %s\n", snd_strerror(err));
		return -1;;
	}

	size = snd_pcm_hw_params_get_period_size(p_params, NULL);
	if (size > *bufsize)
		*bufsize = size;
	size = snd_pcm_hw_params_get_period_size(c_params, NULL);
	if (size > *bufsize)
		*bufsize = size;
	if (snd_pcm_hw_params_get_period_time(p_params, NULL) !=
	    snd_pcm_hw_params_get_period_time(c_params, NULL))
		goto __again;
	if (snd_pcm_hw_params_get_period_size(p_params, NULL) * 2 < snd_pcm_hw_params_get_buffer_size(p_params))
		goto __again;
	if (snd_pcm_hw_params_get_period_size(c_params, NULL) * 2 < snd_pcm_hw_params_get_buffer_size(c_params))
		goto __again;

	if ((err = setparams_set(phandle, p_params, p_swparams, "playback")) < 0) {
		cat_error("Unable to set sw parameters for playback stream: %s\n", snd_strerror(err));
		return -1;;
	}
	if ((err = setparams_set(chandle, c_params, c_swparams, "capture")) < 0) {
		cat_error("Unable to set sw parameters for playback stream: %s\n", snd_strerror(err));
		return -1;;
	}

	if ((err = snd_pcm_prepare(phandle)) < 0) {
		cat_error("Prepare error: %s\n", snd_strerror(err));
		return -1;;
	}



	fflush(stdout);
	return 0;
}

long SndPcm::readbuf(snd_pcm_t *handle, char *buf, long len)
{
	long r;

	if (!block) {
		do {
			r = snd_pcm_readi(handle, buf, len);
		} while (r == -EAGAIN);
		if (r > 0) {



		}
		// cat_error("read = %li\n", r);
	} else {
		int frame_bytes = (snd_pcm_format_width(format) / 8) * channels;
		do {
			r = snd_pcm_readi(handle, buf, len);
			if (r > 0) {
				buf += r * frame_bytes;
				len -= r;



			}
			// cat_error("r = %li, len = %li\n", r, len);
		} while (r >= 1 && len > 0);
	}
	// showstat(handle, 0);
	return r;
}

long SndPcm::writebuf(snd_pcm_t *handle, char *buf, long len)
{
	long r;

	while (len > 0) {
		r = snd_pcm_writei(handle, buf, len);
		if (r == -EAGAIN)
			continue;
		// cat_error("write = %li\n", r);
		if (r < 0)
			return r;
		// showstat(handle, 0);
		buf += r * 4;
		len -= r;

	}
	return 0;
}

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

#ifdef TESTSNDPCM

#include <sched.h>

void setscheduler(void)
{
	struct sched_param sched_param;

	if (sched_getparam(0, &sched_param) < 0) {
		printf("Scheduler getparam failed...\n");
		return;
	}
	sched_param.sched_priority = sched_get_priority_max(SCHED_RR);
	if (!sched_setscheduler(0, SCHED_RR, &sched_param)) {
		printf("Scheduler set to Round Robin with priority %i...\n", sched_param.sched_priority);
		fflush(stdout);
		return;
	}
	printf("!!!Scheduler set to Round Robin with priority %i FAILED!!!\n", sched_param.sched_priority);
}

main()
{
	short buf[1024];
	try
	{
		SndPcm sndpcm(44099,2,"plughw:0,0","plughw:0,0");
		
		setscheduler();

		sndpcm.Start();
		
		for (int i=0;i<1000;i++)
		{
			sndpcm.Poll();
			sndpcm.ReadBuf(buf);
			sndpcm.WriteBuf(buf);
		}
	}
	catch (SndPcmError e)
	{
		printf(e.str);
	}
}

#endif

