#include <qlayout.h>
#include "QtSingleBPFPlayer.hxx"
#include "QtSingleBPFPlayerExt.hxx"

namespace CLAM
{
	namespace VM
	{
		QtSingleBPFPlayerExt::QtSingleBPFPlayerExt(QWidget* parent)
			: QWidget(parent)
		{
			mSlotPlayingTime.Wrap(this,&QtSingleBPFPlayerExt::PlayingTime);
			mSlotStopPlaying.Wrap(this,&QtSingleBPFPlayerExt::StopPlaying);
			InitWidget();
		}

		QtSingleBPFPlayerExt::~QtSingleBPFPlayerExt()
		{
		}

		void QtSingleBPFPlayerExt::SetData(const BPF& bpf)
		{
			((QtSingleBPFPlayer*)mPlayer)->SetData(bpf);
		}

		void QtSingleBPFPlayerExt::SetAudioPtr(const Audio* audio, int chn)
		{
			((QtSingleBPFPlayer*)mPlayer)->SetAudioPtr(audio,chn);
		}

		void QtSingleBPFPlayerExt::SetDuration(TData duration)
		{
			((QtSingleBPFPlayer*)mPlayer)->SetDuration(duration);
		}

		void QtSingleBPFPlayerExt::SetSampleRate(TData sr)
		{
			((QtSingleBPFPlayer*)mPlayer)->SetSampleRate(sr);
		}

		void QtSingleBPFPlayerExt::setRegionTime(float begin, float end)
		{
			MediaTime time;
			time.SetBegin(TData(begin));
			time.SetEnd(TData(end));
			((QtSingleBPFPlayer*)mPlayer)->SetPlaySegment(time);
		}

		void QtSingleBPFPlayerExt::updateYValue(int index, float value)
		{
			((QtSingleBPFPlayer*)mPlayer)->Update(TIndex(index),TData(value));
		}

		void QtSingleBPFPlayerExt::closeEvent(QCloseEvent* e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void QtSingleBPFPlayerExt::InitWidget()
		{
			mPlayer = new QtSingleBPFPlayer(this);
			((QtSingleBPFPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTime);
			((QtSingleBPFPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlaying);
			AddToPlayList();
			QBoxLayout* layout = new QHBoxLayout(this);
			layout->addWidget(mPlayer);
		}

		void QtSingleBPFPlayerExt::PlayingTime(TData time)
		{
			emit playingTime(float(time));
		}

		void QtSingleBPFPlayerExt::StopPlaying(TData time)
		{
			emit stopPlaying(float(time));
		}
	}
}

// END

