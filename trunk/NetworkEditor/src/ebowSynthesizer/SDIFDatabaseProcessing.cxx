#include "SDIFDatabaseProcessing.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, SDIFDatabaseProcessing> regSDIFDatabaseProcessing("SDIFDatabaseProcessing");
}

SDIFDatabaseProcessing::SDIFDatabaseProcessing()
  :	mPitchControl(SDIFDATABASEPROCESSING_FUNDAMENTALFREQUENCY_CONTROL_INPORT_NAME, this),
	mAmplitudeControl(SDIFDATABASEPROCESSING_AMPLITUDE_CONTROL_INPORT_NAME, this),
	mBrightnessControl(SDIFDATABASEPROCESSING_SPECTRALCENTROID_CONTROL_INPORT_NAME, this),
	mOutputSpectralPeaks(SDIFDATABASEPROCESSING_SPECTRALPEAKS_OUTPORT_NAME, this),
	mOutputResidualSpectrum(SDIFDATABASEPROCESSING_RESIDUALSPECTRUM_OUTPORT_NAME, this),
	mSDIFDatabase()
{
	Configure(SDIFDatabaseConfig());
}

SDIFDatabaseProcessing::SDIFDatabaseProcessing(const SDIFDatabaseConfig &c)
 :	mPitchControl(SDIFDATABASEPROCESSING_FUNDAMENTALFREQUENCY_CONTROL_INPORT_NAME, this),
	mAmplitudeControl(SDIFDATABASEPROCESSING_AMPLITUDE_CONTROL_INPORT_NAME, this),
	mBrightnessControl(SDIFDATABASEPROCESSING_SPECTRALCENTROID_CONTROL_INPORT_NAME, this),
	mOutputSpectralPeaks(SDIFDATABASEPROCESSING_SPECTRALPEAKS_OUTPORT_NAME, this),
	mOutputResidualSpectrum(SDIFDATABASEPROCESSING_RESIDUALSPECTRUM_OUTPORT_NAME, this),
	mSDIFDatabase()
{
	Configure(c);
}

SDIFDatabaseProcessing::~SDIFDatabaseProcessing()
{
}

bool SDIFDatabaseProcessing::ConcreteConfigure(const ProcessingConfig& cfgObject)
{
	CopyAsConcreteConfig( mConfig, cfgObject );

	mPitchControl.SetBounds(20,20000);
	mAmplitudeControl.SetBounds(0,1);
	mBrightnessControl.SetBounds(0,1);

	//if (c.GetFilename()
	if ( !mConfig.HasFilename() )
	{
		AddConfigErrorMessage("The provided config object lacked the field 'SourceFile'");
		return false;
	}

	const std::string & location = mConfig.GetFilename();
	// TODO this works in C++?
	if ( location == "")
	{
		AddConfigErrorMessage("No file selected");
		return false;
	}

	std::string programPath(location.c_str());
	bool result = mSDIFDatabase.LoadProgram(programPath);

	// TODO replace this as soon as possible with a control parser that
	// recognizes new voices
	mVoiceId = mSDIFDatabase.CreateVoice();

	return result;
}

bool SDIFDatabaseProcessing::Do(void)
{

	float pitch = mPitchControl.GetLastValue();
	float amplitude = mAmplitudeControl.GetLastValue();
	float brightness = mBrightnessControl.GetLastValue();

	//VoiceId id
	Frame* aFramePtr = mSDIFDatabase.SelectFrameForVoice(mVoiceId, pitch, amplitude);

	// Copy SpectralPeakArray data
	SpectralPeakArray& sourceSpectralPeaks = aFramePtr->GetSpectralPeakArray();
	SpectralPeakArray& targetSpectralPeaks = mOutputSpectralPeaks.GetData();
	targetSpectralPeaks.AddAll();
	targetSpectralPeaks.UpdateData();
	targetSpectralPeaks.CopyMembers(sourceSpectralPeaks);
	
	// Copy ResidualSpectrum data
	// (I would have moved this code into Spectrum as a CopyMembers method
	//  but that class is so convoluted that it would be a massive headache
	//  to make a general copy members class.)
	Spectrum& sourceResidualSpectrum = aFramePtr->GetResidualSpec();
	Spectrum& targetResidualSpectrum = mOutputResidualSpectrum.GetData();
	CopyResidualSpectrumsMembers(sourceResidualSpectrum, targetResidualSpectrum);

	mOutputSpectralPeaks.Produce();
	mOutputResidualSpectrum.Produce();
	return true;
}

void SDIFDatabaseProcessing::CopyResidualSpectrumsMembers(Spectrum& sourceResidualSpectrum,
															Spectrum& targetResidualSpectrum)
{
	targetResidualSpectrum.AddAll();
	targetResidualSpectrum.UpdateData();

	int size = sourceResidualSpectrum.GetSize();
	targetResidualSpectrum.SetSize( size );
	Array<Complex>& srcComplexBuffer = sourceResidualSpectrum.GetComplexArray();
	Array<Complex>& targetComplexBuffer = targetResidualSpectrum.GetComplexArray();
	for (int r=0; r < size; r++)
	{
		targetComplexBuffer[r] = srcComplexBuffer[r];
	}
}

bool SDIFDatabaseProcessing::ConcreteStart()
{
	return true;
}

bool SDIFDatabaseProcessing::ConcreteStop()
{
	return true;
}


} // end namespace CLAM



