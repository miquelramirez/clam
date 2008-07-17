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

#include "Frame.hxx"
#include "Segment.hxx"
#include "Fundamental.hxx"
#include "SpectrumConfig.hxx"
#include "SMSAnalysisCore.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden 
{
	static const char * metadata[] = {
		"key", "SMSAnalysisCore",
		"category", "Analysis",
		"description", "SMSAnalysisCore",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SMSAnalysisCore> reg = metadata;
}

	
SMSAnalysisCore::SMSAnalysisCore()
	: mInputAudio("Input Audio",this ),
	mOutputResSpectrum("Residual Branch Spectrum",this ),
	mOutputSinSpectrum("Sinusoidal Branch Spectrum",this ),
	mOutputSpectralPeaks("Sinusoidal Peaks",this ),
	mOutputFundamental("Fundamental",this ),
	mOutputSubstractedSpectrum("Residual Spectrum", this )
{
	AttachChildren();
	ConnectAndPublishPorts();
	Configure(SMSAnalysisConfig());
}

SMSAnalysisCore::SMSAnalysisCore(SMSAnalysisConfig& cfg)
	: mInputAudio("Input Audio",this ),
	mOutputResSpectrum("Residual Branch Spectrum",this ),
	mOutputSinSpectrum("Sinusoidal Branch Spectrum",this ),
	mOutputSpectralPeaks("Sinusoidal Peaks",this ),
	mOutputFundamental("Fundamental",this ),
	mOutputSubstractedSpectrum("Residual Spectrum", this )
{
	AttachChildren();
	ConnectAndPublishPorts();
	Configure(cfg);
}



SMSAnalysisCore::~SMSAnalysisCore()
{
}

bool SMSAnalysisCore::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool SMSAnalysisCore::ConfigureChildren()
{
	mSinSpectralAnalysis.Configure(mConfig.GetSinSpectralAnalysis());
	mResSpectralAnalysis.Configure(mConfig.GetResSpectralAnalysis());
	mPeakDetect.Configure(mConfig.GetPeakDetect());
	mFundDetect.Configure(mConfig.GetFundFreqDetect());
	mSinTracking.Configure(mConfig.GetSinTracking());
	mSynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum());

	/*Initializing and configuring member circular buffers*/
		
	TSize frameSize=mConfig.GetHopSize();
	TSize sinWindowSize=mConfig.GetSinWindowSize();
	TSize resWindowSize=mConfig.GetResWindowSize();

	mSinSpectralAnalysis.GetInPort("Input").SetSize( sinWindowSize - 1 );
	mSinSpectralAnalysis.GetInPort("Input").SetHop( frameSize );
	mResSpectralAnalysis.GetInPort("Input").SetSize( resWindowSize - 1 );
	mResSpectralAnalysis.GetInPort("Input").SetHop( frameSize );

	return true;

}

void SMSAnalysisCore::ConfigureData()
{
	// XR: TODO - Get Sampling Rate from input audio?
	//Unused variable: TData samplingRate=mConfig.GetSamplingRate();

	Spectrum resSpec;
	Spectrum sinSpec;

	// Spectrum used for temporary residual analysis
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1); // s.AudioFrameSize is the size of the generated frames
	scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
	resSpec.Configure(scfg);
	
	
	// Spectrum used for temporary sinusoidal analysis
	scfg.SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	sinSpec.Configure(scfg);
	
	/* Now we set prototype of SpectrumSubstracter: we want to substract two spectrums: 
	the first on in MagPhase format, the second in Complex format and get the result back
	in Mag Phase.*/

 	SpectrumConfig Scfg; 
 	SpecTypeFlags sflags;
   	sflags.bComplex = 1;
 	sflags.bPolar = 0;
 	sflags.bMagPhase = 0;
 	sflags.bMagPhaseBPF = 0;
 	Scfg.SetType(sflags);
 	Scfg.SetSize(resSpec.GetSize());
 	Scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
 	Spectrum tmpSpecIn(Scfg);    

	mSpecSubstracter.SetPrototypes(resSpec,tmpSpecIn,resSpec);
}

void SMSAnalysisCore::AttachChildren()
{
	mSinSpectralAnalysis.SetParent(this);
	mResSpectralAnalysis.SetParent(this);
	mPeakDetect.SetParent(this);
	mFundDetect.SetParent(this);
	mSinTracking.SetParent(this);
	mSynthSineSpectrum.SetParent( this );
	mSpecSubstracter.SetParent(this);
}

bool SMSAnalysisCore::ConcreteStart()
{
	CLAM_ASSERT( mSinSpectralAnalysis.GetInPort("Input").GetVisuallyConnectedOutPort(), 
			"SMSAnalysisCore::ConcreteStart in port 'Input' needs an attached out port, to start");

	// TODO: port sizes negitiation should be managed by the flow control
	if (mSinSpectralAnalysis.GetInPort("Input").GetVisuallyConnectedOutPort()->GetSize()%2!=0)
	{
		
		int sinSize = mSinSpectralAnalysis.GetInPort("Input").GetSize();
		mSinSpectralAnalysis.GetInPort("Input").GetVisuallyConnectedOutPort()->SetSize( sinSize );
		std::cout << "SMSAnalysisCore::ConcreteStart() Alert!! setting size mSinSpec... : "<< sinSize <<"\n";

		mSinSpectralAnalysis.GetInPort("Input").GetVisuallyConnectedOutPort()->SetHop( sinSize ); 

//		mInputAudio.GetVisuallyConnectedOutPort()->SetSize( mInputAudio.GetSize() );
//		mInputAudio.GetVisuallyConnectedOutPort()->SetHop( mInputAudio.GetSize() );
	}

//	mSinSpectralAnalysis.GetInPort("Input").GetVisuallyConnectedOutPort()->CenterEvenRegions();
	mInputAudio.GetVisuallyConnectedOutPort()->CenterEvenRegions();
		
	return ProcessingComposite::ConcreteStart();
}

bool SMSAnalysisCore::Do()
{
	if( mSinSpectralAnalysis.CanConsumeAndProduce() && mResSpectralAnalysis.CanConsumeAndProduce()  )
	{
		mSinSpectralAnalysis.Do();
		mResSpectralAnalysis.Do();
		
		CLAM_DEBUG_ASSERT( mPeakDetect.CanConsumeAndProduce(), 
			"SMSAnalysisCore::Do() mPeakDetect should have data feeded");
		mPeakDetect.Do();
	
		CLAM_DEBUG_ASSERT( mFundDetect.CanConsumeAndProduce(), 
			"SMSAnalysisCore::Do() mFundDetect should have data feeded");
		mFundDetect.Do();

		CLAM_DEBUG_ASSERT( mSinTracking.CanConsumeAndProduce(), 
			"SMSAnalysisCore::Do() mSinTracking should have data feeded");
		mSinTracking.Do();
	
		CLAM_DEBUG_ASSERT( mSynthSineSpectrum.CanConsumeAndProduce(), 
			"SMSAnalysisCore::Do() mSynthSineSpectrum should have data feeded");
		mSynthSineSpectrum.Do();

		CLAM_DEBUG_ASSERT( mSpecSubstracter.CanConsumeAndProduce(), 
			"SMSAnalysisCore::Do() specSubstracter should have data feeded");
		mSpecSubstracter.Do();

		return true;
	}
	
	return false;
}

void SMSAnalysisCore::ConnectAndPublishPorts()
{
	mInputAudio.PublishInPort( mSinSpectralAnalysis.GetInPort("Input"));
	mInputAudio.PublishInPort( mResSpectralAnalysis.GetInPort("Input"));
		
	mSinSpectralAnalysis.GetOutPort("Output").ConnectToIn( mPeakDetect.GetInPort("Input spectrum"));
	mPeakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( mFundDetect.GetInPort("Input"));
	mPeakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( mSinTracking.GetInPort( "Input" ));
	mFundDetect.GetOutControl( "Fund Freq Value").AddLink( mSinTracking.GetInControl( "Fund Freq Value" ));
	mSinTracking.GetOutPort( "Output").ConnectToIn( mSynthSineSpectrum.GetInPort( "Input" ));

	mResSpectralAnalysis.GetOutPort("Output").ConnectToIn( mSpecSubstracter.GetInPort( "Input 1"));
	mSynthSineSpectrum.GetOutPort("Output").ConnectToIn( mSpecSubstracter.GetInPort( "Input 2"));

	mOutputResSpectrum.PublishOutPort( mResSpectralAnalysis.GetOutPort("Output") );
	mOutputSinSpectrum.PublishOutPort( mSinSpectralAnalysis.GetOutPort("Output") );
	mOutputSpectralPeaks.PublishOutPort( mSinTracking.GetOutPort( "Output") );
	mOutputFundamental.PublishOutPort( mFundDetect.GetOutPort( "Output" ) );
	mOutputSubstractedSpectrum.PublishOutPort( mSpecSubstracter.GetOutPort( "Output") );
}

} // namespace CLAM


