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

#ifndef Auralizer_hxx
#define Auralizer_hxx


#include <QtCore/QThread>
#include <CLAM/BPF.hxx>

namespace CLAM
{
	class Audio;
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

		class BPFPlayer : public Player
		{
			Q_OBJECT
		public:
			BPFPlayer(QObject* parent=0);
			~BPFPlayer();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio, unsigned channelMask=1|2); // TODO: Make this be enums
			void SetPitchBounds(double min, double max);

		private:
			const BPF*   mBPF;
			const Audio* mAudio0;
			const Audio* mAudio1;
			Range        mPitchBounds;

			void run();

			unsigned FirstIndex();
			double GetPitch(unsigned index);
		};
	}
}


class Auralizer
{
public:
	CLAM::Audio mClick; ///< A vector of audios to produce click
	CLAM::Audio mOnsetAuralizationAudio; ///< Current audio with segmentation marks inserted
	const CLAM::Audio * _currentAudio;
	CLAM::VM::BPFPlayer* mPlayer;
	Auralizer(QWidget * parent)
		: _currentAudio(0)
	{
		mPlayer = new CLAM::VM::BPFPlayer(parent);
		initClick();
	}
	void play() { mPlayer->play(); }
	void stop() { mPlayer->stop(); }
	void pause() { mPlayer->pause(); }
	bool isPlaying() { return mPlayer->IsPlaying(); }
	void seekTo(double miliseconds, double endMiliseconds)
	{
		mPlayer->timeBounds(miliseconds, endMiliseconds);
	}
	void setOptions(bool playOnsets, bool playLLDs)
	{
		CLAM_ASSERT(_currentAudio, "Auralizer::setOptions without audio");
//		unsigned int LEFT_CHANNEL = 1;
		unsigned int RIGHT_CHANNEL = 2;
		mPlayer->SetAudioPtr(_currentAudio);
		if (playOnsets)
			mPlayer->SetAudioPtr(&(mOnsetAuralizationAudio), RIGHT_CHANNEL);
		mPlayer->SetPlayingFlags( CLAM::VM::eAudio | (playLLDs?CLAM::VM::eUseOscillator:0));
	}
	void setAudio(const CLAM::Audio & audio)
	{
		_currentAudio= & audio;
		mPlayer->SetAudioPtr(_currentAudio);
		mPlayer->SetDuration(double(audio.GetDuration())/1000.0);
	}
	void setSegmentation(const CLAM::Segmentation * segmentation)
	{
		if (!segmentation) return;
		CLAM_ASSERT(_currentAudio, "Auralizer::setOptions without audio");
		const std::vector<double> & marks = segmentation->onsets();
		int nMarks = marks.size();
		mOnsetAuralizationAudio.SetSize(0);
		mOnsetAuralizationAudio.SetSize(_currentAudio->GetSize());
		mOnsetAuralizationAudio.SetSampleRate(_currentAudio->GetSampleRate());
		int size = mOnsetAuralizationAudio.GetSize();
		for (int i=0; i<nMarks; i++)
		{
			int samplePosition = Round(marks[i]*_currentAudio->GetSampleRate());
			if(samplePosition<size)
				mOnsetAuralizationAudio.SetAudioChunk(samplePosition,mClick);
		}
	}
	void setLLD(const CLAM::EquidistantPointsFunction & epf)
	{
		mPlayer->SetData(epf.GetBPF());

		double minValue = epf.getMin();
		double maxValue = epf.getMax();
		// TODO: Move this margin to the widget
		double span = maxValue-minValue;
		minValue -= span*0.1;
		maxValue += span*0.1;
		mPlayer->SetPitchBounds(minValue, maxValue);
	}
private:
	void initClick()
	{
		mClick.SetSize(1000);

		CLAM::OscillatorConfig cfg;
		cfg.SetFrequency(1000);
		cfg.SetAmplitude(1);
		cfg.SetSamplingRate(44100);
		CLAM::Oscillator osc(cfg);
		osc.Start();
		osc.Do(mClick);
		osc.Stop();
	}
};



#endif//Auralizer_hxx

