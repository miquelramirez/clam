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

#include "FormantExtractor.hxx"
#include "AudioFileIn.hxx"
#include <iostream>

using namespace CLAM;
using std::cout;

class FormantExtrExampleConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FormantExtrExampleConfig, 6, ProcessingConfig);

	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, std::string, Filename);
	DYN_ATTRIBUTE (2, public, TSize, NCandidates);
	DYN_ATTRIBUTE (3, public, TData, Fmax);
	DYN_ATTRIBUTE (4, public, TData, Fmin);
	DYN_ATTRIBUTE (5, public, TSize, HopSize);
private:
	void DefaultInit();
};

void FormantExtrExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetFilename("foo.wav");
}

class FormantExtrExample: public ProcessingComposite
{
	FormantExtrExampleConfig mConfig;
	TSize					 mSize;
	TData					 mSamplingRate;

	AudioFileIn				 mFileIn;
	Audio					 mInputData;

	FormantExtractor		 mFormantExtractor;

	void AttachChildren(void);
	bool ConfigureChildren(void);
	bool ConfigureData(void);
	void ConfigureAudio(void);
	void ConfigureFormantExtr(void);
	void ConfigureFileIO(void);

	const char* GetClassName() const {return "FormantExtrExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c);

public:

	FormantExtrExample( const FormantExtrExampleConfig &cfg );
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do();
};

bool FormantExtrExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();
	
	return true;
}

void FormantExtrExample::AttachChildren(void)
{
	mFileIn.SetParent(this);
	mFormantExtractor.SetParent(this);
}

bool FormantExtrExample::ConfigureChildren(void)
{
	ConfigureFileIO();

	ConfigureFormantExtr();

	return true;
}

void FormantExtrExample::ConfigureFormantExtr(void)
{
	FormantExtractorConfig Fcfg;

	Fcfg.SetName("FormantExtractor");
	Fcfg.SetFmax( mConfig.GetFmax() );
	Fcfg.SetFmin( mConfig.GetFmin() );
	Fcfg.SetHopSize( mConfig.GetHopSize() );
	Fcfg.SetNCandidates( mConfig.GetNCandidates() );
	Fcfg.SetSamplingRate( mSamplingRate );

	mFormantExtractor.Configure( Fcfg );
}

void FormantExtrExample::ConfigureFileIO(void)
{
	AudioFileConfig fcfg;
	fcfg.SetFilename(mConfig.GetFilename());
	fcfg.SetFiletype(EAudioFileType::eWave);
	fcfg.SetChannels(1);
	mFileIn.Configure(fcfg);

	mSize = mFileIn.Size();
	mSamplingRate = mFileIn.SampleRate();
}

void FormantExtrExample::ConfigureAudio(void)
{
	mInputData.SetSize(mSize);
	mInputData.SetSampleRate(mSamplingRate);
}

bool FormantExtrExample::ConfigureData(void)
{
	ConfigureAudio();
	return true;
}

bool FormantExtrExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);
	ConfigureData();
	return true;
}

FormantExtrExample::FormantExtrExample(const FormantExtrExampleConfig& cfg)
{
	AttachChildren();
	Configure(cfg);
}

bool FormantExtrExample::Do()
{
	TData downFactor = Round( mSamplingRate / ( 2 * mConfig.GetFmax() ) );
	TData downSize = Round( mSize / downFactor );

	TData tf = Round( downSize / mConfig.GetHopSize() );
	//MatrixTmpl< Formant > candidates( mConfig.GetNCandidates(), tf );
	Candidates candidates;

	mFileIn.Do( mInputData );

	//CLAMVM::plot( mInputData, "Audio de entrada" );


	mFormantExtractor.Do( mInputData, candidates );

	for (unsigned int i=0; i<tf; i++)
		{
			for (unsigned int j=0; j<mConfig.GetNCandidates(); j++)
				{
					std::cout.width(10L);
					std::cout << "Amp: " << candidates.GetMagMatrix()(j,i) << " ";
					std::cout << "Freq: " << candidates.GetFreqMatrix()(j,i) << " ";
					std::cout.fill();
				} 
			std::cout << std::endl;
		}

	return true;
}

int main()
{
	try
	{
		int NCandidates = 4;
		float fMax = 5000.;
		float fMin = 40.;
		int hopSize = 256;
		
		FormantExtrExampleConfig myAppConfig;

		// Processing composite configuration
		myAppConfig.SetName( "My_FormantExtractor" );
		myAppConfig.SetFilename( "foo.wav" );
		myAppConfig.SetNCandidates( NCandidates );
		myAppConfig.SetFmax( fMax );
		myAppConfig.SetFmin( fMin );
		myAppConfig.SetHopSize( hopSize );
	
		// Processing composite creation
		FormantExtrExample myApp( myAppConfig );

		// End of Processing composite configuration
		myApp.Start();

		// Processing composite execution
		std::cout << "Running object" << std::endl;
		myApp.Do();

	//	myApp.Stop();
	}
	catch( CLAM::Err e )
	{
		e.Print();
	}

	return 0;
}
