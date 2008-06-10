/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "FormantTracker.hxx"
//#include "Plots.hxx" // DGGPORT
#include "MonoAudioFileReader.hxx"
#include <fstream>

using namespace CLAM;

class FormantTrackerExampleConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FormantTrackerExampleConfig, 12, ProcessingConfig);

	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, Filename, Filename);
	DYN_ATTRIBUTE (2, public, Filename, DatFilename );
	DYN_ATTRIBUTE (3, public, TSize, NCandidates);
	DYN_ATTRIBUTE (4, public, TData, Fmax);
	DYN_ATTRIBUTE (5, public, TData, Fmin);
	DYN_ATTRIBUTE (6, public, TSize, HopSize);
	DYN_ATTRIBUTE (7, public, TData, SemitoneRange);
	DYN_ATTRIBUTE (8, public, TData, AmplitudeRange);
	DYN_ATTRIBUTE (9, public, TSize, NToTrack);
	DYN_ATTRIBUTE (10, public, TData, MsWinSize);
	DYN_ATTRIBUTE (11, public, EWindowType, WindowType);
private:
	void DefaultInit();
};

void FormantTrackerExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetFilename("foo.wav");
	SetDatFilename( "formants.dat" );
	SetSemitoneRange( TData(.2599) );  // 4 semitones
	SetAmplitudeRange( 2. );

}

class FormantTrackerExample: public ProcessingComposite
{
	FormantTrackerExampleConfig mConfig;
	TSize                mSize;
	TSize                mNCandidates;
	TData                mSamplingRate;

	MonoAudioFileReader  mFileIn;
	Audio                mInputData;

	FormantTracker       mFormantTracker;

	void AttachChildren(void);
	bool ConfigureChildren(void);
	bool ConfigureData(void);
	void ConfigureAudio(void);
	void ConfigureFormantTracker();
	void ConfigureFileIO(void);

	const char* GetClassName() const {return "FormantTrackerExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c);

public:

	FormantTrackerExample();
	FormantTrackerExample( const FormantTrackerExampleConfig &cfg );
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do();
};

bool FormantTrackerExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();
	
	return true;
}

void FormantTrackerExample::AttachChildren(void)
{
	mFileIn.SetParent(this);
	mFormantTracker.SetParent(this);
}

bool FormantTrackerExample::ConfigureChildren(void)
{
	ConfigureFileIO();
	ConfigureFormantTracker();

	return true;
}

void FormantTrackerExample::ConfigureFormantTracker()
{
/*	TData downFactor = floor( mSamplingRate / ( 2 * mConfig.GetFmax() ) );

	TData winSize = mConfig.GetMSWinSize()*1e-3*mSamplingRate/downFactor;

	mNFrames = floor( mSize / downFactor / mConfig.GetHopSize() );*/

	FormantTrackerConfig Fcfg;

	Fcfg.SetFmax( mConfig.GetFmax() );
	Fcfg.SetFmin( mConfig.GetFmin() );
	Fcfg.SetHopSize( mConfig.GetHopSize() );
	Fcfg.SetNCandidates( mConfig.GetNCandidates() );
	Fcfg.SetSamplingRate( mSamplingRate );
	Fcfg.SetNToTrack( mConfig.GetNToTrack() );
	Fcfg.SetSemitoneRange( mConfig.GetSemitoneRange() );
	Fcfg.SetAmplitudeRange( mConfig.GetAmplitudeRange() );
	Fcfg.SetWindowType( mConfig.GetWindowType() );
	Fcfg.SetMsWinSize( mConfig.GetMsWinSize() );
	Fcfg.SetAudioSize( mSize );

	mFormantTracker.Configure( Fcfg );
}

void FormantTrackerExample::ConfigureFileIO(void)
{
	MonoAudioFileReaderConfig fcfg;
	fcfg.SetSourceFile(mConfig.GetFilename());
	mFileIn.Configure(fcfg);

	mSize = mFileIn.GetHeader().GetSamples();
	mSamplingRate = mFileIn.GetHeader().GetSampleRate();
}

void FormantTrackerExample::ConfigureAudio(void)
{
	mInputData.SetSize(mSize);
	mInputData.SetSampleRate(mSamplingRate);
}

bool FormantTrackerExample::ConfigureData(void)
{
	ConfigureAudio();
	return true;
}

bool FormantTrackerExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	ConfigureChildren();
	ConfigureData();
	return true;
}


FormantTrackerExample::FormantTrackerExample()
{
	AttachChildren();
}

FormantTrackerExample::FormantTrackerExample(const FormantTrackerExampleConfig& cfg)
{
	AttachChildren();
	Configure(cfg);
}

bool FormantTrackerExample::Do()
{
	Array< SpectralPeakArray > formantTracks;

	formantTracks.Resize(mFormantTracker.GetNFrames());
	formantTracks.SetSize(mFormantTracker.GetNFrames());

	for ( int i = 0; i < mFormantTracker.GetNFrames(); i++ )
	{
		formantTracks[i].AddMagBuffer();
		formantTracks[i].AddFreqBuffer();
		formantTracks[i].AddPhaseBuffer();
		formantTracks[i].AddIndexArray();
		formantTracks[i].UpdateData();
	}

	mFileIn.Do( mInputData );

	//CLAMVM::plot( mInputData, "Audio de entrada" );

	mFormantTracker.Do(mInputData, formantTracks );
	// DGGPORT
/*
	CLAMVM::SinTracksPlot plot( "myPlot" );
	plot.SetLabel( "Formant evolution" );
	plot.SetPosition( 100, 100 );
	plot.SetSize( 640, 480 );
	plot.SetYRange( 0, mInputData.GetSampleRate()/2 );
	plot.SetData( formantTracks, mInputData.GetSampleRate() );

	CLAMVM::SystemPlots::DisplayAll();
*/
	std::string filename = mConfig.GetDatFilename();

	std::ofstream formants( filename.c_str() );

	TTime currentCenterTime = mConfig.GetMsWinSize()/4;
	TTime deltaCenterTime = mConfig.GetMsWinSize()/4;

	for ( int i = 0; i < formantTracks.Size(); i++ )
	{
		if ( formantTracks[i].GetFreqBuffer().Size() > 0 )
		{
			formants << currentCenterTime/1000.<< "," << formantTracks[i].GetFreqBuffer()[0]
				 << "," << formantTracks[i].GetFreqBuffer()[1] << std::endl;
			currentCenterTime += deltaCenterTime;
		}
	}

	formants.close();

	return true;
}

int main( int argc, char** argv )
{
	try{

		if ( argc < 3 )
		{
			std::cerr << "ERROR: no input file specified!" << std::endl;
			std::cerr << "Usage: ./FormantTracker <input wav file> <output .dat file>" << std::endl;
			exit(-1);
		}

		int nToTrack = 2;
		int NCandidates = 10;//2*nToTrack;
		float fMax = 1500+(nToTrack-1)*1500;
		float fMin = 40.;
		int winSize = 142;
		float msWinSize = 22.5; // window size in miliseconds
		int hopSize = winSize/4.0;
		
		FormantTrackerExampleConfig myAppConfig;

		// Processing composite configuration
		myAppConfig.SetFilename( argv[1] );
		myAppConfig.SetDatFilename( argv[2] );
		myAppConfig.SetNCandidates( NCandidates );
		myAppConfig.SetFmax( fMax );
		myAppConfig.SetFmin( fMin );
		myAppConfig.SetHopSize( hopSize );
		myAppConfig.SetMsWinSize( msWinSize );
		myAppConfig.SetNToTrack(nToTrack);
		myAppConfig.SetWindowType(EWindowType::eHamming);
	
		// Processing composite creation
		FormantTrackerExample myApp;
		
		myApp.Configure( myAppConfig );

		// End of Processing composite configuration
		myApp.Start();

		// Processing composite execution
		std::cout << "Running object" << std::endl;
		myApp.Do();

	}
	catch( CLAM::Err e )
	{
		e.Print();
	}

  return 0;
}

