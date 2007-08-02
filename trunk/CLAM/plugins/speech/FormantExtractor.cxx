// Version beyond 1.1.0
#include "FormantExtractor.hxx"
#include "AbeLPModel.hxx"
#include "Polynomial.hxx"           //Abe 4-7-2007: moving the root solving from LPModel
#include "AbeLPC_Autocorrelation.hxx" // imports CLAM LPC_AutoCorrelation Processing declaration

#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/AudioIO.hxx>      // imports CLAM::AudioManager and CLAM::AudioIOConfig declarations
#include <CLAM/AudioManager.hxx>
#include <CLAM/AudioOut.hxx>     // imports CLAM::AudioOut declaration
#include <CLAM/AudioFile.hxx>    // imports CLAM::AudioFile declaration
#include <CLAM/MultiChannelAudioFileReader.hxx> // imports CLAM::MultiChannelAudioFileReader
#include <CLAM/Audio.hxx>        // imports the CLAM::Audio ProcessingData class interface
#include <CLAM/Err.hxx>          // imports CLAM::Err exception class declaration
#include <CLAM/Array.hxx>
#include <CLAM/FFT.hxx>           // imports CLAM::FFT Processing declaration
#include <CLAM/Spectrum.hxx>            // imports CLAM::Spectrum ProcessingData declaration
#include <CLAM/SpectrumConfig.hxx>      // imports CLAM::Spectrum auxiliar datatypes declaration
#include <CLAM/WindowGenerator.hxx>     // imports CLAM::WindowGenerator Processing declaration
#include <CLAM/AudioMultiplier.hxx>     // imports CLAM::AudioMultiplier Processing declaration

#include <CLAM/Array.hxx>    //Abe: temporary until I make a formant class
#include <CLAM/Complex.hxx>        //Abe 29-6-2007: for complex type
#include <cmath>
//#include <CLAM/CLAM_Math.hxx>    //Abe 29-6-2007: for complex type
//#include <CLAM/Spectrum.hxx>            // Abe to use <Complex>
#include <CLAM/ComplexToPolarCnv.hxx>

namespace CLAM
{
	namespace Hidden
	{
		static FactoryRegistrator<ProcessingFactory, FormantExtractor>  regFormantExtractor("FormantExtractor");
		static class FormantExtractorMetadata
		{
		public:
			FormantExtractorMetadata()
			{
				ProcessingFactory & factory = ProcessingFactory::GetInstance();
				factory.AddAttribute("FormantExtractor", "category", "CLAM");
				factory.AddAttribute("FormantExtractor", "description", "FormantExtractor");
			}
		} dummy;
	}
  
	FormantExtractor::FormantExtractor()
		: AudioIn("AudioIn", this)
		, FormantsOut("Spectral Peak Array", this)
	{
		Configure( Config() );
	}
  
	bool FormantExtractor::Do()
	{
		bool result = Do(AudioIn.GetData(), FormantsOut.GetData());
		AudioIn.Consume();
		FormantsOut.Produce();
		return result;
	}
  
  
	bool FormantExtractor::Do(Audio& in, SpectralPeakArray& out) {
		out.AddAll();
		out.UpdateData();
		out.InitIndices();
		if ( !AbleToExecute() )
			return false;
		const TData sampleRate = in.GetSampleRate();
		const TSize frameSize = 1024;

		AbeLPModel lpModel;
		const unsigned lpcOrder = 11;
		lpModel.UpdateModelOrder( lpcOrder );
		lpModel.SetSpectralRange( sampleRate / 2 );
		CLAM::LPCConfig lpcCfg;
		CLAM::AbeLPC_AutoCorrelation lpc;
		lpcCfg.SetOrder( lpcOrder );
		lpc.Configure( lpcCfg );

		SpecTypeFlags flags;
		flags.bMagPhase=1;
		flags.bComplex = 0;
		Spectrum lpSpectrum;
		lpSpectrum.SetSize( frameSize/2+1 );
		lpSpectrum.SetSpectralRange( sampleRate/2 );
		lpSpectrum.SetType( flags );

		Array <Complex> roots;
		Array <Polar> formants;

		lpc.Start();
		lpc.Do( in, lpModel );
		lpc.Stop();

		// get the roots of the lp equation (they are sympetric about the real axis of the z-plane
		roots = lpModel.ToRoots(lpModel.GetFilterCoefficients());

		out.SetnMaxPeaks(lpcOrder);
		out.SetnPeaks(0);
		out.SetScale(EScale::eLog);

		float spectralRange = sampleRate/2;
		unsigned nBins =1024;
		unsigned hopSize =512;
		IndexArray& outIndexBuffer=out.GetIndexArray();
		DataArray& outMagBuffer=out.GetMagBuffer();
		DataArray& outFreqBuffer=out.GetFreqBuffer();
		DataArray& outPhaseBuffer=out.GetPhaseBuffer();
		DataArray& outBinPosBuffer=out.GetBinPosBuffer();
		DataArray& outBinWidthBuffer=out.GetBinWidthBuffer();
		unsigned firstBin = 440*nBins/spectralRange;
		std::cout<<"LPC roots in Formant Extractor"<<std::endl;

		ComplexToPolarCnv_ convert;

		formants.Resize(roots.Size());
		formants.SetSize(roots.Size());
		convert.ToPolar(roots, formants);
		unsigned  f; //formant # starting from 0
		for(unsigned int i=0; i<formants.Size(); i++ )
		{
			//if angle is not zero, inside the unit circle, and imag non negative
			if(formants[i].Ang()>0 && formants[i].Mag()<1) {
				std::cout<<roots[i]<<std::endl;
				std::cout<<formants[i]<<" "<<formants[i].Ang()/(M_PI)*spectralRange<<", "<<formants[i].Mag()<<std::endl;
				//then, add to the out buffer
				outIndexBuffer.AddElem(f++);
				outFreqBuffer.AddElem(formants[i].Ang()/M_PI*spectralRange);
				outMagBuffer.AddElem(-20.-20*std::log(formants[i].Mag()));
				outPhaseBuffer.AddElem(formants[i].Ang()*hopSize);
				outBinPosBuffer.AddElem(formants[i].Ang()*nBins );
				outBinWidthBuffer.AddElem(formants[i].Ang()*nBins/2);
			}
		}

		out.SetIsIndexUpToDate(true);
		return true;

	}

}
