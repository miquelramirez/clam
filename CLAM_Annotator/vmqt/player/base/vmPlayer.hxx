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
			volatile bool mIsPlaying;
			volatile bool mIsPaused;
			double        mSamplingRate;
			double        mBeginTime;
			Range         mTimeBounds;

			virtual void run()=0; // thread code here
		};
	}
}

#endif
