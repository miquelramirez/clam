/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "SMSTransformationChain.hxx"

//We should avoid having to include all SMS Transformations here: factory needed
#include "SegmentTransformationConfig.hxx"
#include "SegmentSMSMorphConfig.hxx"
#include "SegmentSMSTimeStretchConfig.hxx"

namespace CLAM
{
	// SMSTransformationChaineeConfig method definition

	SMSTransformationChaineeConfig::~SMSTransformationChaineeConfig()
	{
		if(mpConcreteConfig) 
			delete mpConcreteConfig;
	}


	void SMSTransformationChaineeConfig::DefaultInit( )
	{
		AddAll();
		UpdateData();
		SetConcreteClassName("Unknown");
		mpConcreteConfig=NULL;
	}

	void SMSTransformationChaineeConfig::CopyInit(const SMSTransformationChaineeConfig& originalConfig)
	{
		AddAll();
		UpdateData();
		mpConcreteConfig=NULL;
		SetConcreteConfig(*(originalConfig.mpConcreteConfig));
	}

	void SMSTransformationChaineeConfig::StoreOn(Storage & s) const
	{
		ProcessingConfig::StoreOn(s);
		mpConcreteConfig->StoreOn(s);
	}
	
	void SMSTransformationChaineeConfig::LoadFrom(Storage& s)
	{
		ProcessingConfig::LoadFrom(s);
		mpConcreteConfig=InstantiateConcreteConfig();
		const_cast<ProcessingConfig*>(mpConcreteConfig)->LoadFrom(s);
	}

	ProcessingConfig* SMSTransformationChaineeConfig::InstantiateConcreteConfig(const std::string& type)
	{
		if(type=="SMSDummyTransformation"||type=="SMSFreqShift"||type=="SMSPitchShift"||
		   type=="SMSOddEvenHarmonicRatio"||type=="SMSSineFilter"||type=="SMSResidualGain"||
		   type=="SegmentSMSHarmonizer"||type=="SMSSinusoidalGain"||type=="SMSPitchDiscretization"||
		   type=="SMSSpectralShapeShift"||type=="SMSGenderChange"||
		   type=="SMSTransformationChainIO")
		{
			return new CLAM::SegmentTransformationConfig();
		}
		if (type=="SegmentSMSMorph") 
			return new CLAM::SegmentSMSMorphConfig();
		if (type=="SegmentSMSTimeStretch")
			return new CLAM::SegmentSMSTimeStretchConfig();

		// error let's investigate
		CLAM_ASSERT(type!="Unknown",
			"Before instantiating a concrete configuration, you have to set its class name");

		std::string error="SMSTransformationChaineeConfig::InstantiateConcreteConfig: "
			"Trying to instantiate a non-valid Configuration: ";
		error+=type;
		CLAM_ASSERT(false,error.c_str());
		return 0;
	}
	
	// SMSTransformationChainConfig method definition
	void SMSTransformationChainConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	void SMSTransformationChainConfig::AddConfiguration(const ProcessingConfig& newConcreteConfig,const std::string& className)
	{
		SMSTransformationChaineeConfig newChaineeConfig;
		newChaineeConfig.SetConcreteClassName(className);
		newChaineeConfig.SetConcreteConfig(newConcreteConfig);
			
		//We first resize and set to false by default, on array
		GetOnArray().Resize(GetnConfigurations()+1);
		GetOnArray().SetSize(GetnConfigurations()+1);
		GetOnArray()[GetnConfigurations()]=false;
		
		GetConfigurations().push_back(newChaineeConfig);
		
	}
	
}

