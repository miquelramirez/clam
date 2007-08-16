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
#include <CLAM/Audio.hxx>
#include <CLAM/Segmentation.hxx>
#include <CLAM/Oscillator.hxx>
#include "EquidistantPointsFunction.hxx"

#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioManager.hxx>
#include <CLAM/AudioOut.hxx>
#include <CLAM/SimpleOscillator.hxx>


#include <QtCore/QThread>
#include "vmDataTypes.hxx"

#include "vmPlayer.hxx"


class AuralizationPlayer : public QThread
{
	Q_OBJECT
public:
	enum PlayStatus {Stoped, Paused, Playing};
	AuralizationPlayer(QObject* parent=0)
		: QThread(parent)
		, mPlayStatus(Stoped)
		, mSamplingRate(44100.0)
		, mBeginTime(0.0)
		, mTimeBounds(0.0,1.0)
		, mBPF(0)
		, mAudio0(0)
		, mAudio1(0)
		, mPitchBounds(min_ref,max_ref)
	{
		SetPlayingFlags(CLAM::VM::eUseOscillator);
	}
	virtual ~AuralizationPlayer()
	{
		if (mPlayStatus != Playing) return;
		mPlayStatus = Stoped;
		terminate();
		wait();
	}

	void SetDuration(double dur)
	{
		if(dur <= 0) return;
		mTimeBounds.min = 0.0;
		mTimeBounds.max = dur;
		mBeginTime = 0.0;
	}
	void SetSamplingRate(double sr)
	{
		mSamplingRate = sr;
	}
	void SetPlayingFlags(int flags)
	{
		mPlayingFlags = flags;
	}
	bool IsPlaying() const
	{
		return mPlayStatus == Playing;
	}

	void SetData(const CLAM::BPF& bpf) { mBPF = &bpf; }
	enum { LEFT=1, RIGHT=2};
	void SetAudioPtr(const CLAM::Audio* audio, unsigned channelMask=LEFT|RIGHT)
	{
		if (channelMask&1)
			mAudio0 = audio;
		if (channelMask&2)
			mAudio1 = audio;
	}

	void SetPitchBounds(double min, double max)
	{
		if(min >= max) return;
		mPitchBounds.min = min;
		mPitchBounds.max = max;
	}
signals:
	void playingTime(double);
	void stopTime(double, bool);

public slots:
	void play()
	{
		mPlayStatus = Playing;
		start();
	}
	void pause()
	{
		mPlayStatus = Paused;
	}
	void stop()
	{
		mPlayStatus = Stoped;
		wait();
	}

	void timeBounds(double min, double max)
	{
		if(min < 0) return;
		if(min >= max) return;
		mTimeBounds.min = min;
		mTimeBounds.max = max;
		mBeginTime = mTimeBounds.min;
	}

protected:
	int           mPlayingFlags;
	volatile PlayStatus    mPlayStatus;
	double        mSamplingRate;
	double        mBeginTime;
	CLAM::VM::Range         mTimeBounds;
	const CLAM::BPF*   mBPF;
	const CLAM::Audio* mAudio0;
	const CLAM::Audio* mAudio1;
	CLAM::VM::Range    mPitchBounds;

	static const double min_ref;
	static const double max_ref;

private:
	void run()
	{
		if(!mAudio0) SetPlayingFlags(CLAM::VM::eUseOscillator);
		if(mAudio0) mSamplingRate = mAudio0->GetSampleRate();
		else mSamplingRate=44100;

		unsigned frameSize = 4092;     
		CLAM::AudioManager manager(mSamplingRate,(int)frameSize);  

		manager.Start();
		
		CLAM::AudioOut channel0;   
		CLAM::AudioIOConfig audioOutCfg0;     
		audioOutCfg0.SetChannelID(0);    
		channel0.Configure(audioOutCfg0); 

		CLAM::AudioOut channel1;   
		CLAM::AudioIOConfig audioOutCfg1;     
		audioOutCfg1.SetChannelID(1);    
		channel1.Configure(audioOutCfg1); 
		
		channel0.Start();
		channel1.Start();

		CLAM::SimpleOscillatorConfig oscCfg;
		oscCfg.SetSamplingRate(mSamplingRate);
		oscCfg.SetAmplitude(0.6);
		CLAM::SimpleOscillator osc(oscCfg);
		
		CLAM::InControl& freqControl = osc.GetInControls().Get("Pitch");

		CLAM::Audio samplesAudio0;
		CLAM::Audio samplesAudio1;
		CLAM::Audio samplesBpf;
		samplesAudio0.SetSize(frameSize);
		samplesAudio1.SetSize(frameSize);
		samplesBpf.SetSize(frameSize);

		CLAM::Audio silence;
		silence.SetSize(frameSize);

		unsigned firstIndex = FirstIndex();
		unsigned k = firstIndex;

		unsigned start = unsigned(mBeginTime*mSamplingRate);
		unsigned nSamples = unsigned(mTimeBounds.max*mSamplingRate);
    
		unsigned leftIndex = start;        
		unsigned rightIndex = leftIndex+frameSize;

		osc.Start();

		for(unsigned i=start; i < nSamples; i+=frameSize)
		{
			if (mPlayStatus!=Playing) break;
			if(mBPF && mBPF->Size() && k+1<mBPF->Size() && mBPF->GetXValue(k+1)<= i/mSamplingRate) k++;

			if(mAudio0 && rightIndex < unsigned(mAudio0->GetSize()))
				mAudio0->GetAudioChunk(int(leftIndex),int(rightIndex),samplesAudio0);
			if(mAudio1 && rightIndex < unsigned(mAudio1->GetSize()))
				mAudio1->GetAudioChunk(int(leftIndex),int(rightIndex),samplesAudio1);

			if(mBPF && mBPF->Size() && k < mBPF->Size()) freqControl.DoControl(GetPitch(k));
			osc.Do(samplesBpf);
			if (mBPF && mBPF->Size() && leftIndex/mSamplingRate < mBPF->GetXValue(k))
			{
				channel0.Do(silence);
				channel1.Do(silence);
			}
			else if ((mPlayingFlags & CLAM::VM::eAudio) && (mPlayingFlags & CLAM::VM::eUseOscillator))
			{
				channel0.Do(samplesAudio0);
				channel1.Do(samplesBpf);
			}
			else if (mPlayingFlags & CLAM::VM::eAudio && !(mPlayingFlags & CLAM::VM::eUseOscillator))
			{
				channel0.Do(samplesAudio0);
				channel1.Do(samplesAudio1);
			}
			else
			{
				channel0.Do(samplesBpf);
				channel1.Do(samplesBpf);
			}
			emit playingTime(double(leftIndex)/mSamplingRate);
			leftIndex += frameSize;
			rightIndex += frameSize;
		}
		osc.Stop();
		channel0.Stop();
		channel1.Stop();

		if (mPlayStatus==Playing) mPlayStatus=Stoped;
		mBeginTime = (mPlayStatus==Paused) ? double(leftIndex)/mSamplingRate : mTimeBounds.min;
		emit stopTime(double(leftIndex)/mSamplingRate,mPlayStatus==Paused);
	}

	unsigned FirstIndex()
	{
		if (!mBPF ) return 0;
		unsigned nPoints = mBPF->Size();
		if(nPoints <= 1) return 0;

		if(mBeginTime <= mBPF->GetXValue(0)) return 0;
		if(mBeginTime >= mBPF->GetXValue(nPoints-1)) return nPoints-1;

		unsigned index = 0;
		unsigned left_index = 0;
		unsigned right_index = nPoints-1;
		while(left_index <= right_index)
		{
			const unsigned currentIndex = (left_index+right_index)/2;
			if(currentIndex >= nPoints-1)
			{
				index = currentIndex;
				break;
			}
			if(mBeginTime >= double(mBPF->GetXValue(currentIndex)) && 
			   mBeginTime <= double(mBPF->GetXValue(currentIndex+1)))
			{
				index = currentIndex;
				break;
			}
			if(mBeginTime < double(mBPF->GetXValue(currentIndex)))
			{
				right_index = currentIndex-1;
			}
			else if(mBeginTime > double(mBPF->GetXValue(currentIndex)))
			{
				left_index = currentIndex+1;
			}
		}
		return index;
	}

	double GetPitch(unsigned index)
	{
		double value = mBPF->GetValueFromIndex(index);
//		if(value >= min_ref && value <= max_ref) return value;
		if(value < mPitchBounds.min) value = mPitchBounds.min;
		else if(value > mPitchBounds.max) value = mPitchBounds.max;
		return (value-mPitchBounds.min)*(max_ref-min_ref)/mPitchBounds.Span()+min_ref;
	}

};


class Auralizer
{
public:
	CLAM::Audio mClick; ///< A vector of audios to produce click
	CLAM::Audio mOnsetAuralizationAudio; ///< Current audio with segmentation marks inserted
	const CLAM::Audio * _currentAudio;
	AuralizationPlayer* mPlayer;
	Auralizer(QObject * parent)
		: _currentAudio(0)
	{
		mPlayer = new AuralizationPlayer(parent);
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
		mPlayer->SetAudioPtr(_currentAudio);
		if (playOnsets)
			mPlayer->SetAudioPtr(&(mOnsetAuralizationAudio), AuralizationPlayer::RIGHT);
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

