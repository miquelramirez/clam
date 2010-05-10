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

#ifndef AIRABSORTION_INCLUDED
#define AIRABSORTION_INCLUDED

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
class AirAbsortion : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config , 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, Distance);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetDistance(0);
		}
	};
//private: //TODO debugging	
protected:
	Config _config;
	
	InPort<Audio> _in1;
	OutPort<Audio> _out1;
	float _sampleRate;
	float _xN,_xNmin1,_xNmin2,_yN,_yNmin1,_yNmin2;

protected:
	FloatInControl _distance;
			

public:
	AirAbsortion(const Config& config = Config()) 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)	
		,_distance("relative distance in mts", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		_sampleRate = BackendSampleRate();	

		_distance.DoControl(_config.GetDistance());
		_distance.SetBounds(0,500);
		_xN=0.0f;
		_xNmin1=0.0f;
		_xNmin2=0.0f;
		_yN=0.0f;
		_yNmin1=0.0f;
		_yNmin2=0.0f;

		return true;
	}
	
	const char* GetClassName() const { return "AirAbsortion"; }
	
	virtual ~AirAbsortion() {}
		
	const ProcessingConfig & GetConfig() const { return _config; }
	
	bool Do()
	{	
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out = _out1.GetData();
		out.SetSize(size);
		TData* outpointer = out.GetBuffer().GetPtr();	
	
		const float S=1.0f;
		const float freqHz=10000.0f;
		float distance = _distance.GetLastValue();			
		float dBgain=-0.05*distance;
		//std::cout << gain << "*************************************** " <<std::endl;
		float A  = pow(10,dBgain/40);
		float w0 = 2*M_PI*freqHz/_sampleRate;  
		//float alpha = sin(w0)/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 );		
		float two_sqrtA_alpha  =  sin(w0) * sqrt( (pow(A,2) + 1)*(1/S - 1) + 2*A );
		
		float b0 = A*( (A+1) + (A-1)*cos(w0) + two_sqrtA_alpha );
           	float b1 = -2*A*( (A-1) + (A+1)*cos(w0));
            	float b2 = A*( (A+1) + (A-1)*cos(w0) - two_sqrtA_alpha );
            	float a0 = (A+1) - (A-1)*cos(w0) + two_sqrtA_alpha;
            	float a1 = 2*((A-1) - (A+1)*cos(w0));
            	float a2 = (A+1) - (A-1)*cos(w0) - two_sqrtA_alpha;

		for (unsigned i = 0; i < size; ++i){
			
			_xNmin2=_xNmin1;
			_xNmin1=_xN;
			_xN=inpointer[i];

        		_yN = (b0/a0)*_xN + (b1/a0)*_xNmin1 + (b2/a0)*_xNmin2 - (a1/a0)*_yNmin1 - (a2/a0)*_yNmin2 ;

			_yNmin2=_yNmin1;
			_yNmin1=_yN;
			outpointer[i]=_yN; 
		}
		
		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

