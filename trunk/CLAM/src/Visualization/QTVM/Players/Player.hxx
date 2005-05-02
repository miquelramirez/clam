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

#ifndef __PLAYER__
#define __PLAYER__

#include "MediaTime.hxx"
#include "Thread.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

using SigSlot::Signalv1;
using SigSlot::Slotv1;

namespace CLAM
{
    namespace VM
    {
	class Player 
	{
	public:
	    Player();
	    virtual ~Player();
				
	    void Play();
	    void Pause();
	    void Stop();

	    bool IsPlaying();

	    void SetBounds(const MediaTime& time);

	    void SetSlotPlayingTime(Slotv1<TData>& slot);
	    void SetSlotStopPlaying(Slotv1<TData>& slot);
	
	protected:
	    Thread _thread;
	    MediaTime _time;
	    Signalv1<TData> mSigPlayingTime;
	    Signalv1<TData> mSigStop;

	    virtual void thread_code()=0;
				
	    void HaveData(bool d);
	    void SetPlaying(bool playing);
	    void SetPaused(bool paused);

	    bool IsPaused();
	    bool IsStopped();
					
	    bool HaveData();
								
	    void SetBeginTime(const TData& begin);
	    TData GetBeginTime() const;
				
	private:
	    TData _beginTime;
	    bool _haveData;
	    bool _isPlaying;
	    bool _isPaused;
	    bool _isStopped;

	    void SetStopped(bool stopped);
				
	};
    }
}

#endif

