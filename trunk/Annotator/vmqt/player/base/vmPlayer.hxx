/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_PLAYER_H__
#define __VMQT_PLAYER_H__

#include <QtCore/QThread>
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class Player : public QThread
		{
			Q_OBJECT
		public:
			enum PlayStatus {Stoped, Paused, Playing};
			Player(QObject* parent=0);
			virtual ~Player();

			void SetDuration(double dur);
			void SetSamplingRate(double sr);
			void SetPlayingFlags(int flags);

			bool IsPlaying() const;

		signals:
			void playingTime(double);
			void stopTime(double, bool);

		public slots:
			void play();
			void pause();
			void stop();

			void timeBounds(double, double);

		protected:
			int           mPlayingFlags;
			volatile PlayStatus    mPlayStatus;
			double        mSamplingRate;
			double        mBeginTime;
			Range         mTimeBounds;

			virtual void run()=0; // thread code here
		};
	}
}

#endif

