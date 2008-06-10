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

#include "DownSampling.hxx"
#include "ProcessingComposite.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
//#include "Plot.hxx" // DGGPORT

using namespace CLAM;

class DownSamplingExampleConfig : public ProcessingConfig {
public:

		DYNAMIC_TYPE_USING_INTERFACE (DownSamplingExampleConfig, 5, ProcessingConfig);

		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, FilenameIn);
		DYN_ATTRIBUTE (2, public, std::string, FilenameOut);
		DYN_ATTRIBUTE (3, public, TData, DownSamplingRate);
		DYN_ATTRIBUTE (4, public, TData, SamplingRate);

private:
	void DefaultInit();
};


void DownSamplingExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();
}


class DownSamplingExample : public ProcessingComposite {
	DownSamplingExampleConfig mConfig;
	TSize mSizeIn;
	TSize mSizeOut;

	MonoAudioFileReader mFileIn;
	MonoAudioFileWriter mFileOut;
	Audio mInputData;
	Audio mOutputData;

	DownSampling	  mDownSampling;

	void AttachChildren();
	bool ConfigureChildren();
	bool ConfigureData();
	void ConfigureAudio();

	const char* GetClassName() const {return "DownSamplingExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c);
public:
	DownSamplingExample(const DownSamplingExampleConfig &cfg);
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do();
};

bool DownSamplingExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();

	return true;
}




void DownSamplingExample::AttachChildren()
{
	mFileIn.SetParent(this);
	mFileOut.SetParent(this);
	mDownSampling.SetParent(this);
}


bool DownSamplingExample::ConfigureChildren()
{
	MonoAudioFileReaderConfig inConfig;
	inConfig.SetSourceFile(mConfig.GetFilenameIn());
	mFileIn.Configure(inConfig);
	mSizeIn = mFileIn.GetHeader().GetSamples();
	mSizeOut = Round( mSizeIn / 
						Round( mConfig.GetSamplingRate() / mConfig.GetDownSamplingRate() ) );

	MonoAudioFileWriterConfig outConfig;
	outConfig.SetTargetFile(mConfig.GetFilenameOut());
	outConfig.SetSampleRate(mConfig.GetDownSamplingRate() );
	mFileOut.Configure(outConfig);


	DownSamplingConfig DownSampling_cfg;
	DownSampling_cfg.SetName( "DownSampling" );
	DownSampling_cfg.SetDownSamplingRate( mConfig.GetDownSamplingRate() );

	mDownSampling.Configure( DownSampling_cfg );

	return true;
}

void DownSamplingExample::ConfigureAudio()
{
	mInputData.SetSize(mSizeIn);
	mInputData.SetSampleRate(mConfig.GetSamplingRate());

	mOutputData.SetSize( mSizeOut);
	mOutputData.SetSampleRate( mConfig.GetDownSamplingRate() );
}


bool DownSamplingExample::ConfigureData()
{
	ConfigureAudio();
	return true;
}

bool DownSamplingExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);
	ConfigureData();
	return true;
}

DownSamplingExample::DownSamplingExample(const DownSamplingExampleConfig &cfg)
{
	AttachChildren();
	Configure(cfg);
}

bool DownSamplingExample::Do()
{
		mFileIn.Do(mInputData);
		mDownSampling.Do( mInputData,mOutputData);
//		CLAMVM::plot( mOutputData, "Audio de salida" ); //DGGPORT
		mFileOut.Do(mOutputData);

	  return true;
}

int main()
{
	try {
		TData SamplingRate = 44100.;
		TData downSR = 10000.;

		DownSamplingExampleConfig cfg;
		cfg.SetName("DownSampling_Example");
		cfg.SetSamplingRate(SamplingRate );
		cfg.SetDownSamplingRate( downSR );
		cfg.SetFilenameIn("foo.wav");
		cfg.SetFilenameOut("C:\\out_46.wav");

		DownSamplingExample app(cfg);

		app.Start();

		app.Do();

		return 0;

	}
	catch (Err &e) {
		e.Print();
		return 1;
	}
}
