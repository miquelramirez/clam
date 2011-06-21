/*
 * Copyright (c) 2010 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef DOPPLERFRACTIONALDELAY_INCLUDED
#define DOPPLERFRACTIONALDELAY_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/InControl.hxx>

#include <vector>
#include <cmath>

namespace CLAM
{

/**
 *
 */
class DopplerFractionalDelay : public Processing
{

	class Implementation
	{
	private:
		typedef long long unsigned Index;
		typedef std::vector<TData> DelayBuffer;
	private:
		DelayBuffer _delayBuffer;
		Index _sampleRate;
		Index _delayBufferSize;
		Index _readIndex;
		Index _writeIndex;
		Index _modIndex;
		float _centerTap;
		float _width;
		float _freqMod;
		TData _pastModelayLine;
		float _pastDist;
		float _step;
		float _interpDist;
		bool _notInitialized;
	public:
		Implementation(unsigned sampleRate, unsigned maxDelayInSamples)
		: _delayBufferSize(1)
		, _pastModelayLine(0)
		, _pastDist(0)
		, _step(0)
		, _interpDist(0)
		, _notInitialized(true)
		{
			_sampleRate = sampleRate;
			_delayBuffer.resize(maxDelayInSamples * _sampleRate);
			_delayBufferSize = _delayBuffer.size();
			_writeIndex = (_delayBufferSize-1);
			_readIndex = (_delayBufferSize-1);
			std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);

		}
		void setDelay(float delaySamples)
		{
			CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
			Index delayInSamples = round(delaySamples);

			if (delayInSamples > _delayBufferSize)
				return;

			Index readIndex = (_writeIndex - delayInSamples) % _delayBufferSize;
			if (_readIndex % _delayBufferSize == readIndex)
				return;

			_readIndex = readIndex;

		}

		TData delayLine(TData x, float frac,Index D)
		{
			CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");
			Index writeindex = _writeIndex++ % _delayBufferSize;
			Index readindex = _readIndex++ % _delayBufferSize;

			_delayBuffer[writeindex] = x;

			if (readindex==0) return _delayBuffer.back() + (1-frac) * _delayBuffer[readindex]-(1-frac)*_pastModelayLine;
			return _delayBuffer[readindex-1] + (1-frac) * _delayBuffer[readindex] - (1-frac)*_pastModelayLine;

		}

		void run(unsigned bufferSize, const float * inpointer, float * outpointer, bool newControlArrived, float distance, float shiftGain)
		{
			const float C=340.0;
			const float FPS=25;
			const float shiftConstant=1000.0;
			if (newControlArrived)
			{
				if (_notInitialized)
				{
					_pastDist=distance;
					_interpDist=_pastDist;
					_notInitialized=false;
				}
				if (abs(distance-_pastDist)>10)
				{
					_step=0;
					_pastDist=distance;
					_interpDist=_pastDist;
				}
				else
				{
					_step=(distance-_pastDist)*FPS/_sampleRate;
					//if (distance > 0) _step=-_step;
					_pastDist=distance;
				}
			}


			for (unsigned i = 0; i < bufferSize; ++i)
			{
				//float delay=shiftGain + shiftGain*(_interpDist/C);
				float delay=shiftGain*shiftConstant*sqrt((_interpDist)*(_interpDist))/C;
				float D=floor(delay);
				float frac=delay-D;
				setDelay(D);
				outpointer[i] = delayLine(inpointer[i],frac,D);
				_pastModelayLine=outpointer[i];
				_interpDist+=_step;
			}
		}
	};
public:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config , 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, MaxDelayInSeconds);
		DYN_ATTRIBUTE (1, public, float, Distance);
		DYN_ATTRIBUTE (2, public, float, ShiftGain);


	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMaxDelayInSeconds(1.3653125);  //65535
			SetDistance(0);
			SetShiftGain(1);
		}
	};
//private: //TODO debugging
protected:
	Config _config;
	InPort<Audio> _in1;
	OutPort<Audio> _out1;
	FloatInControl _distance;
	FloatInControl _shiftGain;

	Implementation * _impl;

private:


public:
	DopplerFractionalDelay(const Config& config = Config())
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
		, _distance("relative distance in mts", this)
		, _shiftGain("freq shift scaler", this)
		, _impl(0)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		if (_impl) delete _impl;
		_impl = new Implementation(BackendSampleRate(), _config.GetMaxDelayInSeconds());

		_distance.DoControl(_config.GetDistance());
		_distance.SetBounds(0,500);
		_shiftGain.DoControl(_config.GetShiftGain());
		_shiftGain.SetBounds(1,20);  //////////////////////added to exagerate the effect/////////////////////////
		return true;
	}

	const char* GetClassName() const { return "DopplerFractionalDelay"; }

	virtual ~DopplerFractionalDelay() {}

	const ProcessingConfig & GetConfig() const { return _config; }


	bool Do()
	{
		bool newControlArrived = not _distance.HasBeenRead();

		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();
		unsigned bufferSize = in.GetSize();

		CLAM::Audio& out = _out1.GetData();
		out.SetSize(bufferSize);
		TData* outpointer = out.GetBuffer().GetPtr();

		TControlData distance = _distance.GetLastValue();
		float shiftGain=_shiftGain.GetLastValue();
		_impl->run(bufferSize, inpointer, outpointer, newControlArrived, distance, shiftGain);

		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif

