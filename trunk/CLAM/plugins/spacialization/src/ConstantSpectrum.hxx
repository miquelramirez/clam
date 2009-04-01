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

#ifndef ConstantSpectrum_hxx
#define ConstantSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MagPhaseSpectrum.hxx"
#include "MyFFT.hxx"
#include "Complex2MagPhaseSpectrum.hxx"
#include <iostream>
namespace CLAM
{


/**
 This processing takes the first FrameSize samples of an audio file
 and takes it as constant spectrum on the output.
 This processing can be used, for example, to filter an incoming stream with
 a filter represented in a wave file as a time domain impulse response.
 If the impulse response is long and you want to achieve low latency,
 consider to use ImpulseResponse objects and LowLatencyConvolution objects.
 @param AudioFile [Control] The file name of the input file
 @param FrameSize [Control] The number of samples to compute the FFT on.
 @param[out] "Complex Spectrum" [Port] The spectrum in ComplexSpectrum representation
 @param[out] "MagPhase Spectrum" [Port] The spectrum in MagPhaseSpectrum representation
 @todo Doing zero padding when the FrameSize is shorter
 @ingroup NewSpectralProcessing
*/
class ConstantSpectrum : public Processing
{ 
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, AudioInFilename, AudioFile);
		DYN_ATTRIBUTE( 1, public, int, FrameSize);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetAudioFile("");
			SetFrameSize(65536/2);
		};
	};

	OutPort<ComplexSpectrum> mOutComplex;
	OutPort<MagPhaseSpectrum> mOut;
	MagPhaseSpectrum mSpectrum;
	ComplexSpectrum mComplexSpectrum;
	Config mConfig;
public:
	const char* GetClassName() const { return "ConstantSpectrum"; }
	ConstantSpectrum(const Config& config = Config()) 
		: mOutComplex("Complex Spectrum", this) 
		, mOut("MagPhase Spectrum", this) 
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		if (!FillSpectrum()) return false;
		return true; 
	}

	bool FillSpectrum()
	{

		MonoAudioFileReaderConfig readerConfig;
		readerConfig.SetSourceFile(mConfig.GetAudioFile());
		MonoAudioFileReader reader(readerConfig);
		if (!reader.IsConfigured())
		{
			AddConfigErrorMessage(reader.GetConfigErrorMessage());
			return false;
		}
		const unsigned nSamples = reader.GetHeader().GetSamples();
		std::cout << "FillConstantSpectrum: NSamples: " << nSamples << std::endl;
		
		AudioWindowingConfig windowerConfig;
		windowerConfig.SetSamplingRate(44100);
		windowerConfig.SetHopSize(mConfig.GetFrameSize()); // 0 would work, just to keep the parameter relation on a normal analysis
		windowerConfig.SetWindowSize(mConfig.GetFrameSize()+1);
		windowerConfig.SetFFTSize(mConfig.GetFrameSize()*2);
		windowerConfig.SetDoHalfWindowShift(false);
		windowerConfig.SetWindowType(EWindowType::eNone);
		AudioWindowing windower(windowerConfig);
		if (not windower.IsConfigured())
		{
			AddConfigErrorMessage(windower.GetConfigErrorMessage());
			return false;
		}

		MyFFT::Config fftConfig; 
		fftConfig.SetAudioSize(mConfig.GetFrameSize()*2);
		MyFFT fft(fftConfig);
		if (not fft.IsConfigured())
		{
			AddConfigErrorMessage(fft.GetConfigErrorMessage());
			return false;
		}
		Complex2MagPhaseSpectrum toMagPhase;

		ConnectPorts(reader,0,windower,0);
		ConnectPorts(windower,0,fft,0);
		ConnectPorts(fft,0,toMagPhase,0);
		InPort<MagPhaseSpectrum> fetcher;
		toMagPhase.GetOutPort(0).ConnectToIn(fetcher);
		InPort<ComplexSpectrum> complexFetcher;
		fft.GetOutPort(0).ConnectToIn(complexFetcher);

		std::cout << "Reader" << std::endl;
		reader.Start();
		std::cout << "Windower" << std::endl;
		windower.Start();
		std::cout << "fft" << std::endl;
		fft.Start();
		std::cout << "MagPhase" << std::endl;
		toMagPhase.Start();
		std::cout << "Started" << std::endl;
		do 
		{
			reader.Do();
		} while (not windower.CanConsumeAndProduce() );
		windower.Do();
		fft.Do();
		toMagPhase.Do();

		mSpectrum = fetcher.GetData();
		mComplexSpectrum = complexFetcher.GetData();

		reader.Stop();
		windower.Stop();
		fft.Stop();
		toMagPhase.Stop();
		return true;
	}
 
	bool Do()
	{
		MagPhaseSpectrum & spectrum = mOut.GetData();
		ComplexSpectrum & complexSpectrum = mOutComplex.GetData();
		if (mSpectrum.magnitudes.size() != !spectrum.magnitudes.size())
			spectrum = mSpectrum;
		if (mComplexSpectrum.bins.size() != !complexSpectrum.bins.size())
			complexSpectrum = mComplexSpectrum;

		// Tell the ports this is done
		mOut.Produce();
		mOutComplex.Produce();
		return true;
	}

};

}

#endif // ConstantSpectrum_hxx
