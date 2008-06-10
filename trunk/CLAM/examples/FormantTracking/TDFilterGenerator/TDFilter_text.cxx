#include "TDFilter.hxx"
#include "TDFilterGen.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "ProcessingComposite.hxx"
//#include "Plot.hxx" // DGGPORT
#include <fstream>

using namespace CLAM;

class TDFilterExampleConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (TDFilterExampleConfig,7, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, AudioInFilename, FilenameIn);
	DYN_ATTRIBUTE (2, public, AudioOutFilename, FilenameOut);
	DYN_ATTRIBUTE (3, public, TData, SamplingRate);
	DYN_ATTRIBUTE (4, public, ETDFilterType, Type);
	DYN_ATTRIBUTE (5, public, DataArray, A);
	DYN_ATTRIBUTE (6, public, DataArray, B);
private:
	void DefaultInit();
};

void TDFilterExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();
//	SetFilenameIn("foo.wav");
//	SetFilenameOut( "out46.wav" );
}

class TDFilterExample: public ProcessingComposite
{
	TDFilterExampleConfig	mConfig;
	TSize					mSize;

	MonoAudioFileReader mFileIn;
	MonoAudioFileWriter mFileOut;
	Audio mInputData;
	Audio mOutputData;

	TDFilterGen		mTDFilterGen;
	TDFilter		mTDFilter;

	void AttachChildren();
	bool ConfigureChildren();
	bool ConfigureData();
	void ConfigureAudio();
	void ConfigureTDFilter();
	void ConfigureTDFilterGen();
	void ConfigureFileIO();
	const char* GetClassName() const {return "TDFilterExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c);
public:
	TDFilterExample(const TDFilterExampleConfig &cfg);
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do() ;
};

bool TDFilterExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();
	
	return true;
}

void TDFilterExample::AttachChildren()
{
	mFileIn.SetParent(this);
	mFileOut.SetParent(this);
	mTDFilter.SetParent(this);
	mTDFilterGen.SetParent(this);
}

bool TDFilterExample::ConfigureChildren()
{
	ConfigureFileIO();

	ConfigureTDFilter();

	ConfigureTDFilterGen();

	return true;
}

void TDFilterExample::ConfigureFileIO()
{
	MonoAudioFileReaderConfig fcfg;
	fcfg.SetSourceFile(mConfig.GetFilenameIn());
	mFileIn.Configure(fcfg);

	mSize = mFileIn.GetHeader().GetSamples();

	MonoAudioFileWriterConfig writerCfg;
	writerCfg.SetTargetFile(mConfig.GetFilenameOut());
	writerCfg.SetSampleRate(mFileIn.GetHeader().GetSampleRate() );
	mFileOut.Configure(writerCfg);

}

void TDFilterExample::ConfigureTDFilter()
{
	TDFilterConfig cfg;

	cfg.SetName( "TDFilter" );
	cfg.SetAudioSize( mSize );

	mTDFilter.Configure( cfg );
}

void TDFilterExample::ConfigureTDFilterGen()
{
	TDFilterGenConfig cfg;

	cfg.SetName( "TDFilter_generator" );
	cfg.SetType( mConfig.GetType() );
	cfg.SetSpectralRange( mConfig.GetSamplingRate() / 2. );

	if( mConfig.HasA() )
		cfg.SetACoefficients( mConfig.GetA() );

	if( mConfig.HasB() )
		cfg.SetBCoefficients( mConfig.GetB() );

	mTDFilterGen.Configure( cfg );
}

void TDFilterExample::ConfigureAudio()
{
	mInputData.SetSize(mSize);
	mInputData.SetSampleRate(mConfig.GetSamplingRate());
	
	mOutputData.SetSize( mSize);
	mOutputData.SetSampleRate( mConfig.GetSamplingRate() );
}

bool TDFilterExample::ConfigureData()
{
	ConfigureAudio();
	return true;
}

bool TDFilterExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);
	ConfigureChildren();
	ConfigureData();
	return true;
}

TDFilterExample::TDFilterExample(const TDFilterExampleConfig &cfg)
{
	AttachChildren();
	Configure(cfg);
}

bool TDFilterExample::Do(void)
{
	TDFilterKernel myTDFilterKernel;

	mFileIn.Do( mInputData );

//	CLAMVM::plot( mInputData, "Audio de entrada" );

	mTDFilterGen.Do( myTDFilterKernel );
	mTDFilter.Do( mInputData, myTDFilterKernel, mOutputData );

	//CLAMVM::plot( mOutputData, "Audio de salida" ); // DGGPORT


	mFileOut.Do( mOutputData );

	return true;
}
int main()
{
	try
	{
		float SampleRate = 44100.;
		int polesOrder = 10;
		int zerosOrder = 10;
		
		DataArray A, B;
		
		// DataArray configuration
		A.Resize( polesOrder + 1 );
		A.SetSize( polesOrder + 1 );
		B.Resize( zerosOrder + 1 );
		B.SetSize( zerosOrder + 1 );


		// Filter coefficients
		B[10] = 1;		A[10] =   0.0092341067;
		B[9]= 10;     	A[9] =    -0.1343381478;
		B[8] = 45;     	A[8] = 0.8925614801;
		B[7] = 120;     A[7] =  -3.5717077136;
		B[6] = 210;     A[6] = 9.5491733251 ;
		B[5] = 252;     A[5] = -17.8599406974;
		B[4] = 210;     A[4] = 23.7263412533;
		B[3] = 120;     A[3] = -22.1798128381;
		B[2] = 45;     	A[2] = 14.0254513805;
		B[1] = 10;     	A[1] = -5.4519604921;
		B[0] = 1;     	A[0] =  1.0;
		//Processing composite comfiguration
		TDFilterExampleConfig myAppConfig;

		myAppConfig.SetName( "My_TDFilter" );
		myAppConfig.SetFilenameIn( "c:\\noise.wav" );
		myAppConfig.SetFilenameOut( "c:\\out_46.wav" );
		myAppConfig.SetSamplingRate( SampleRate );
		myAppConfig.SetType( ETDFilterType::eARMA );
//		myAppConfig.SetType( ETDFilterType::eAutoregressive );
//		myAppConfig.SetType( ETDFilterType::eFIR );
		myAppConfig.SetA( A );
		myAppConfig.SetB( B );

		//Processing composite creation
		TDFilterExample myApp( myAppConfig );

		//End of processing compostie configuration
		myApp.Start();

		//Processing composite execution
		myApp.Do();

	}
	catch( CLAM::Err err )
	{
		err.Print();
	}
	return 0;
}
