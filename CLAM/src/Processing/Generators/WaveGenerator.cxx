/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "OSDefines.hxx"
#include "WaveGenerator.hxx"
#include "CLAM_Math.hxx"

namespace CLAM {

	void WaveGeneratorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetFrequency(440.0);
		SetAmplitude(1.0);
		SetPhase(0.0);
		SetSampleRate(44100);
		SetFrameSize(512);
		SetWaveType(EWaveType::eSine);
	}


	template<> 
	inline TData WaveFunctor<WaveGenerator::EWaveType_eSine>::operator()(TTime x,TData amplitude)
	{
		return amplitude * sin(x);
	}




	WaveGenerator::WaveGenerator()
		: Output("Output",this)
	{
		Configure(WaveGeneratorConfig());
	};

	WaveGenerator::WaveGenerator(const WaveGeneratorConfig &c)
		: Output("Output",this)
	{ 
		Configure(c);
	};

	bool WaveGenerator::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mAmplitude = mConfig.GetAmplitude();

		TData samples_per_period = mConfig.GetSampleRate() / mConfig.GetFrequency();

		mXDelta =  2.0 * M_PI / samples_per_period;

		mXPos = fmod( TData(mConfig.GetPhase()), TData(2 * M_PI) );

		mType = mConfig.GetWaveType();
		
		Output.SetSize(mConfig.GetFrameSize());

		return true;
	}

	WaveGenerator::~WaveGenerator()
	{
	}

	bool WaveGenerator::Do(Audio& out)
	{
		switch(mType)
		{
		case EWaveType::eSine:

			FillBuffer< EWaveType_eSine > ( out.GetBuffer(), *this );
			break;
		default:
			return false;
		}
		return true;
	}


	bool WaveGenerator::Do(void)
	{
		bool res = Do(Output.GetData());
		Output.Produce();
		return res;
	}


}

