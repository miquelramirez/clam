#include "TDFilterGen.hxx"
#include "LPC.hxx"
#include "TDFilter.hxx"
#include "DownSampling.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "CLAM_Math.hxx"
#include "XMLStorage.hxx"

using namespace CLAM;

class TDFilterGenExampleConfig : public ProcessingConfig {
public:

		DYNAMIC_TYPE_USING_INTERFACE (TDFilterGenExampleConfig, 6, ProcessingConfig);

		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, AudioInFilename, FilenameIn);
		DYN_ATTRIBUTE (2, public, AudioOutFilename, FilenameOut);
		DYN_ATTRIBUTE (3, public, TData, DownSamplingRate);
		DYN_ATTRIBUTE (4, public, TData, SamplingRate);
		DYN_ATTRIBUTE (5, public, int, Order);
private:
	void DefaultInit();
};


void TDFilterGenExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetFilenameIn("foo.wav");
	SetFilenameOut( "out.wav" );
}


class TDFilterGenExample : public ProcessingComposite {
	TDFilterGenExampleConfig mConfig;
	TSize mSizeIn;
	TSize mDownSize;
	int mOrder;
	TData mNewSamplingRate;

	MonoAudioFileReader mFileIn;
	MonoAudioFileWriter mFileOut;
	Audio mInputData;
	Audio mDownInputData;
	Audio mOutputData;

	LPC				mLPC;
	DownSampling	mDownSampling;
	TDFilterGen		mTDFilterGen;
	TDFilter		mTDFilter;

	void AttachChildren();
	bool ConfigureChildren();
	bool ConfigureData();
	bool ConfigureTDFilterGen( const DataArray &a );
	bool ConfigureTDFilter();
	bool ConfigureLPC();
	bool ConfigureDownSampling();
	bool ConfigureFileIO();

	const char* GetClassName() const {return "TDFilterGenExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c);
public:
	TDFilterGenExample(const TDFilterGenExampleConfig &cfg);
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do() { return true; }
	bool Do( TDFilterKernel &F, DataArray& K, TData& E );
};

bool TDFilterGenExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();
	
	return true;
}




void TDFilterGenExample::AttachChildren()
{
	mFileIn.SetParent(this);
	mFileOut.SetParent(this);
	mLPC.SetParent(this);
	mDownSampling.SetParent(this);
	mTDFilter.SetParent(this);
	mTDFilterGen.SetParent(this);
}


bool TDFilterGenExample::ConfigureChildren()
{
	if (not ConfigureFileIO()) return false;
	if (not ConfigureLPC()) return false;
	if (not ConfigureDownSampling()) return false;
	if (not ConfigureTDFilter()) return false;
	return true;
}

bool TDFilterGenExample::ConfigureFileIO()
{
	MonoAudioFileReaderConfig readerCfg;
	readerCfg.SetSourceFile(mConfig.GetFilenameIn());
	mFileIn.Configure(readerCfg);
//	mSizeIn = mFileIn.Size();
	mDownSize = Round( mSizeIn / 
						 Round( mConfig.GetSamplingRate() / mConfig.GetDownSamplingRate() ) );

	mNewSamplingRate = mConfig.GetSamplingRate() / mDownSize;
	
	MonoAudioFileWriterConfig writerCfg;
	writerCfg.SetTargetFile(mConfig.GetFilenameOut());
	writerCfg.SetSampleRate(mNewSamplingRate );
	return mFileOut.Configure(writerCfg);

}

bool TDFilterGenExample::ConfigureLPC()
{
	LPCConfig LPC_cfg;
	LPC_cfg.SetName( "LPC" );
	LPC_cfg.SetOrder( mConfig.GetOrder() );

	return mLPC.Configure( LPC_cfg );
}

bool TDFilterGenExample::ConfigureTDFilter()
{
	TDFilterConfig cfg;

	cfg.SetName( "TDFilter" );
	cfg.SetAudioSize( mDownSize );

	return mTDFilter.Configure( cfg );
}

bool TDFilterGenExample::ConfigureDownSampling()
{
	DownSamplingConfig	downSampling_cfg;
	downSampling_cfg.SetName( "DownSampling" );
	downSampling_cfg.SetDownSamplingRate( mConfig.GetDownSamplingRate() );

	return mDownSampling.Configure( downSampling_cfg );
}


bool TDFilterGenExample::ConfigureTDFilterGen( const DataArray &a )
{
	TDFilterGenConfig cfg;

	cfg.SetName( "TDFilter_generator" );
	cfg.SetType( ETDFilterType::eAutoregressive );
	cfg.SetSpectralRange( mNewSamplingRate );
	cfg.SetACoefficients( a );

	if( mTDFilterGen.IsRunning() ) mTDFilterGen.Stop();

	return mTDFilterGen.Configure( cfg );
}

bool TDFilterGenExample::ConfigureData()
{
	mInputData.SetSize(mSizeIn);
	mInputData.SetSampleRate(mConfig.GetSamplingRate());

	mDownInputData.SetSize( mDownSize);
	mDownInputData.SetSampleRate( mConfig.GetDownSamplingRate() );

	mOutputData.SetSize( mDownSize);
	mOutputData.SetSampleRate( mConfig.GetDownSamplingRate() );
	return true;
}

bool TDFilterGenExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);
	if (not ConfigureChildren()) return false;
	ConfigureData();
	return true;
}

TDFilterGenExample::TDFilterGenExample(const TDFilterGenExampleConfig &cfg)
	: mSizeIn(512)
{
	AttachChildren();
	Configure(cfg);
}

bool TDFilterGenExample::Do( TDFilterKernel &F, DataArray& K, TData& E )
{
	DataArray A;

	A.Resize( K.Size() );
	A.SetSize( K.Size() );

	mFileIn.Do(mInputData);
	mDownSampling.Do( mInputData, mDownInputData );
	mNewSamplingRate = mDownInputData.GetSampleRate();
	mLPC.Do( mDownInputData, A, K, E);

	ConfigureTDFilterGen( A );

	mTDFilterGen.Do( F );

	mTDFilter.Do( mDownInputData, F, mOutputData );

	mFileOut.Do( mOutputData );

	return true;
}

int main( int argc, char* argv[] )
{
	try{
		int Size = 512;
		int LPC_order = 11;
		float SampleRate = 44100.0;
		float DownSR = 10000.; 

		CLAM::DataArray A;	// filter coefficients
		CLAM::DataArray K;	// reflection coefficients
		CLAM::TData E;	// average of mean squared error of the filter
		CLAM::TDFilterKernel myTDFilter;

		// DataArray configuration
		A.Resize( LPC_order );
		A.SetSize( LPC_order );
		K.Resize( LPC_order );
		K.SetSize( LPC_order );

		//TDFilter configuration
		//myTDFilter.SetSamplingRate( DownSR );

		// Processing composite configuration
		TDFilterGenExampleConfig myAppConfig;
		myAppConfig.SetName( "My_TDFilter" );
		myAppConfig.SetFilenameIn( "foo.wav" );
		myAppConfig.SetOrder( LPC_order );
		myAppConfig.SetSamplingRate( SampleRate );
		myAppConfig.SetDownSamplingRate( DownSR );

		// Processing composite creation
		TDFilterGenExample myApp( myAppConfig );

		// End of Processing composite configuration
		myApp.Start();

		// Processing composite execution
		std::cout << "Running object" << std::endl;
		myApp.Do( myTDFilter, K, E );

		CLAM::XMLStorage::Dump( myTDFilter.A(), "ACoefficients", "A.xml" );
		CLAM::XMLStorage::Dump( myTDFilter.GetFreqPoles(), "Frequencies", "Freqs.xml" );
		CLAM::XMLStorage::Dump( myTDFilter.GetAmpPoles(), "Amplitudes", "Amps.xml" );
		CLAM::XMLStorage::Dump( myTDFilter.Poles(), "ComplexRoots", "CRoots.xml" );

	}
	catch( CLAM::Err err )
	{
		err.Print();
	}

	return 0;
}
