#include "ProcessingChain.hxx"

//We should avoid having to include all SMS Transformations here: factory needed
#include "SMSTransformationConfig.hxx"
#include "SMSMorphConfig.hxx"
#include "SMSTimeStretchConfig.hxx"

namespace CLAM
{
	// ProcessingChaineeConfig method definition

	ProcessingChaineeConfig::~ProcessingChaineeConfig()
	{
		if(mpConcreteConfig) 
			delete mpConcreteConfig;
	}


	void ProcessingChaineeConfig::DefaultInit( )
	{
		AddAll();
		UpdateData();
		SetConcreteClassName("Unknown");
		mpConcreteConfig=NULL;
	}

	void ProcessingChaineeConfig::CopyInit(const ProcessingChaineeConfig& originalConfig)
	{
		AddAll();
		UpdateData();
		mpConcreteConfig=NULL;
		SetConcreteConfig(*(originalConfig.mpConcreteConfig));
	}

	void ProcessingChaineeConfig::StoreOn(Storage & s) const
	{
		ProcessingConfig::StoreOn(s);
		mpConcreteConfig->StoreOn(s);
	}
	
	void ProcessingChaineeConfig::LoadFrom(Storage& s)
	{
		ProcessingConfig::LoadFrom(s);
		mpConcreteConfig=InstantiateConcreteConfig();
		const_cast<ProcessingConfig*>(mpConcreteConfig)->LoadFrom(s);
	}

	ProcessingConfig* ProcessingChaineeConfig::InstantiateConcreteConfig(const std::string& type)
	{
		if(type=="SMSDummyTransformation"||type=="SMSFreqShift"||type=="SMSPitchShift"||
		   type=="SMSOddEvenHarmonicRatio"||type=="SMSSineFilter"||type=="SMSResidualGain"||
		   type=="SMSHarmonizer"||type=="SMSSinusoidalGain"||type=="SMSPitchDiscretization"||
		   type=="SMSSpectralShapeShift"||type=="SMSGenderChange"||
		   type=="SMSTransformationChainIO")
		{
			return new CLAM::SMSTransformationConfig();
		}
		if (type=="SMSMorph") 
			return new CLAM::SMSMorphConfig();
		if (type=="SMSTimeStretch")
			return new CLAM::SMSTimeStretchConfig();

		// error let's investigate
		CLAM_ASSERT(type!="Unknown",
			"Before instantiating a concrete configuration, you have to set its class name");

		std::string error="ProcessingChaineeConfig::InstantiateConcreteConfig: "
			"Trying to instantiate a non-valid Configuration: ";
		error+=type;
		CLAM_ASSERT(false,error.c_str());
	}
	
	// ProcessingChainConfig method definition
	void ProcessingChainConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	ProcessingChainConfig::~ProcessingChainConfig()
	{
	}
	
	void ProcessingChainConfig::AddConfiguration(const ProcessingConfig& newConcreteConfig,const std::string& className)
	{
		ProcessingChaineeConfig newChaineeConfig;
		newChaineeConfig.SetConcreteClassName(className);
		newChaineeConfig.SetConcreteConfig(newConcreteConfig);
			
		//We first resize and set to false by default, on array
		GetOnArray().Resize(GetnConfigurations()+1);
		GetOnArray().SetSize(GetnConfigurations()+1);
		GetOnArray()[GetnConfigurations()]=false;
		
		GetConfigurations().push_back(newChaineeConfig);
		
	}
	
}
