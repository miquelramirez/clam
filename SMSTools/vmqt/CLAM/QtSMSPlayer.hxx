#ifndef __QTSMSPLAYER__
#define __QTSMSPLAYER__

#include <CLAM/Segment.hxx>
#include <CLAM/QtMultiPlayer.hxx>

namespace CLAM
{
	namespace VM
	{
		class QtSMSPlayer : public QtMultiPlayer
		{
			Q_OBJECT
		public:
			QtSMSPlayer(QWidget* parent=0);
			~QtSMSPlayer();

			void SetData(std::vector<const Audio*> data, bool setTime);
			void SetData(const Segment& segment);
			
			void SetLeftChannelMuted(bool b);
			void SetRightChannelMuted(bool b);

			void SetCurrentPlayer(int playerID);

			void Flush();

		public slots:
			void play();

		private:
			bool mHasAudio;
			bool mHasFundamental;

			enum { AUDIO=0, FUNDAMENTAL=1, PLAY=2 };

			int LookIfAvailable();
		};
	}
}

#endif

