/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef ImpulseResponseInterpolator_hxx
#define ImpulseResponseInterpolator_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"
#include "ComplexSpectrum.hxx"
#include "ComplexSpectrumMixer.hxx"
#include <algorithm>
#include <cmath>
#include <limits>

namespace CLAM
{


/**
 * Does the interpolation of two ImpulseResponse objects.
 */
class ImpulseResponseInterpolator : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);
	    DYN_ATTRIBUTE( 1, public, int, MaxNFrames);
	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
		  SetMaxNFrames(1000);
	    };
	};
private:
	InControl _position;
	double _lastPosition;
	InPort< std::vector<ComplexSpectrum>* > _input1;
	InPort< std::vector<ComplexSpectrum>* > _input2;
	OutPort< std::vector<ComplexSpectrum>* > _output;
	std::vector<ComplexSpectrum> _responseSpectrums;
	Config _config;
public:
	const char* GetClassName() const { return "ImpulseResponseInterpolator"; }
	ImpulseResponseInterpolator(const Config& config = Config()) 
		: _position("Position", this)
		, _input1("Input1", this)
		, _input2("Input2", this)
		, _output("Output", this)
	{
		Configure( config );
		_position.SetBounds(0,1);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		ConfigureInterpolatedResponseSpectrums();
		_position.DoControl(_position.DefaultValue());
		_lastPosition = std::numeric_limits<CLAM::TControlData>::max();

		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	void ConfigureInterpolatedResponseSpectrums()
	{
		const unsigned nBlocks = _config.GetMaxNFrames();
		_responseSpectrums.resize(nBlocks);
		for (unsigned i=0; i<nBlocks; i++)
		{
			ComplexSpectrum & spectrum = _responseSpectrums[i];
			spectrum.bins.assign(_config.GetFrameSize()+1,std::complex<CLAM::TData>());
		}
	}
	void InterpolateResponseSpectrums()
	{
		if (std::abs( _position.GetLastValue() - _lastPosition) < 0.001 )
			return;
		std::cout << "ImpulseResponseInterpolator: position changed"<<std::endl;
		_lastPosition = _position.GetLastValue();

		const std::vector<ComplexSpectrum> & input1 = *_input1.GetData();
		const std::vector<ComplexSpectrum> & input2 = *_input2.GetData();

		ComplexSpectrum zeroSpectrum;
		zeroSpectrum.spectralRange=input1[0].spectralRange;
		zeroSpectrum.bins.assign(_config.GetFrameSize()+1,std::complex<CLAM::TData>());
		std::cout 
			<< input1.size() << " "
			<< input2.size() << " "
			<< _config.GetMaxNFrames() << " "
			<< input1[0].bins.size() <<  " "
			<< input2[0].bins.size() <<  " "
			<< _config.GetFrameSize() << " "
			<< std::endl;
		ComplexSpectrumMixer mixer;
		mixer.GetInControl("Gain1").DoControl(_lastPosition);
		mixer.GetInControl("Gain2").DoControl(1-_lastPosition);
		for (unsigned i=0; i<_responseSpectrums.size(); i++)
		{
			const ComplexSpectrum & spectrum1 = i<input1.size() ? input1[i] : zeroSpectrum;
			const ComplexSpectrum & spectrum2 = i<input2.size() ? input2[i] : zeroSpectrum;
			mixer.Do( spectrum1, spectrum2, _responseSpectrums[i] );
		}
	}

	bool Do()
	{
		InterpolateResponseSpectrums();
		_output.GetData()= &_responseSpectrums;
		_output.Produce();
		return true;
	}
};


} // namespace CLAM

#endif // ImpulseResponseInterpolator_hxx

