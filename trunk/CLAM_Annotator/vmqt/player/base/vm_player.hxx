#ifndef __VMQT_PLAYER_H__
#define __VMQT_PLAYER_H__

#include <QThread>
#include "vm_data_types.hxx"

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

			void set_duration(double dur);
			void set_sambling_rate(double sr);
			void set_playing_flags(int flags);

			bool is_playing() const;

		signals:
			void playingTime(double);
			void stopTime(double, bool);

		public slots:
			void play();
			void pause();
			void stop();

			void timeBounds(double, double);

		protected:
			int             pl_playing_flags;
			volatile bool   pl_is_playing;
			volatile bool   pl_is_paused;
			double          pl_sampling_rate;
			double          pl_begin_time;
			CLAM::VM::Range pl_time_bounds;

			virtual void run()=0; // thread code here
		};
	}
}

#endif
