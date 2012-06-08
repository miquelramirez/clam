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

#include <CLAM/MediaTime.hxx>
#include <CLAM/Thread.hxx>
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>

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

			const bool& IsPlaying() const;

			void SetBounds(const MediaTime& time);

			void SetSlotPlayingTime(Slotv1<TData>& slot);
			void SetSlotStopPlaying(Slotv1<TData>& slot);

			bool IsRunning() const;
	
		protected:
			Thread mThread;
			MediaTime mTime;
			Signalv1<TData> mSigPlayingTime;
			Signalv1<TData> mSigStop;

			virtual void thread_code()=0;
				
			void HaveData(bool d);
			void SetPlaying(bool playing);
			void SetPaused(bool paused);

			const bool& IsPaused() const;
			const bool& IsStopped() const;
					
			const bool& HaveData() const;
								
			void SetBeginTime(const TData& begin);
			const TData& GetBeginTime() const;
				
		private:
			TData mBeginTime;
			bool mHaveData;
			bool mIsPlaying;
			bool mIsPaused;
			bool mIsStopped;

			void SetStopped(bool stopped);
				
		};
    }
}

#endif

