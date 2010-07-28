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

#ifndef EnergyFeatures_INCLUDED
#define EnergyFeatures_INCLUDED


#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/TypeInfo.hxx>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cmath>

namespace CLAM
{

class EnergyFeatures : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	public: 
		DYNAMIC_TYPE_USING_INTERFACE (Config , 1, ProcessingConfig);
		DYN_ATTRIBUTE( 0, public, unsigned int, SourceID);
  
	protected:
    		void DefaultInit()
    		{
			AddAll();
			UpdateData();
			SetSourceID(0);
    		}
	};

protected:
	Config _config;		
	InPort<Audio> _in;
	OutControl<int> _sourceId;
	FloatOutControl _energyOutput;
	FloatOutControl _deltaEnergyOutput;
	OutControl<int> _frameCountOutput;
	float _energy;
	float _lastlogEnergy;
	float _frameCounter;
	int _framesBetweenOnsets;
	unsigned _sampleCount,_sampleRate;



public:
	EnergyFeatures(const Config& config = Config())
		: _in("Input", this)
		, _sourceId("ID number of Source",this)
		, _energyOutput("Energy Output",this)
		, _deltaEnergyOutput("delta Energy Output",this)
		, _frameCountOutput("Frames between Onsets",this)	
		, _energy(0.)
		, _lastlogEnergy(0) 
		, _frameCounter(0)
		, _framesBetweenOnsets(0)
		, _sampleCount(0)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(_config, c);
		_sampleRate = BackendSampleRate();

		return true;
	}

	const char* GetClassName() const {return "EnergyFeatures";}

	~EnergyFeatures() {}

	const ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		const unsigned int samplesPerFrame = _sampleRate/25;

		const CLAM::Audio& in = _in.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();				

		for (unsigned i=0; i<size; i++)
		{
			const CLAM::TData & current = inpointer[i];
			_sampleCount++;
			_energy += current*current;
			if (_sampleCount >= samplesPerFrame)
			{
				_energy=_energy/samplesPerFrame;
				if (_energy < 0.0001) _energy=0.0001;					
				float logEnergy =log(sqrt(_energy));
				float deltaEnergy=logEnergy - _lastlogEnergy;
				if (deltaEnergy >= 2){
					_framesBetweenOnsets=_frameCounter;
					_frameCounter=0;	
				}
				_sourceId.SendControl(_config.GetSourceID());				
				_energyOutput.SendControl(_energy);
				_deltaEnergyOutput.SendControl(deltaEnergy);
				_frameCountOutput.SendControl(_framesBetweenOnsets);
				//std::cout << _energy << " " << deltaEnergy << std::endl;
				_lastlogEnergy =logEnergy;
				_energy = 0.;
				_sampleCount=0;
				_frameCounter++;
			}
		}

		_in.Consume();
		return true;
	}
	
};

} //namespace

#endif
