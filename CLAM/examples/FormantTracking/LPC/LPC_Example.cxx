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

#include<iostream>
#include "OSDefines.hxx"
#include "AudioFileIn.hxx"
#include "AudioManager.hxx"

#include "LPC.hxx"
#include "XMLStorage.hxx"

using namespace CLAM;

class LPCExampleConfig : public ProcessingConfig {
public:

		DYNAMIC_TYPE_USING_INTERFACE (LPCExampleConfig, 3, ProcessingConfig);

		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Filename);
		DYN_ATTRIBUTE (2, public, int, Order);
private:
	void DefaultInit();
};


void LPCExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();
}


class LPCExample : public ProcessingComposite {
	LPCExampleConfig mConfig;
	int mSize;
	int mOrder;

	AudioFileIn mFileIn;
	Audio mInputData;

	LPC	  mLPC;

	void AttachChildren();
	bool ConfigureChildren();
	bool ConfigureData();
	void ConfigureAudio(Audio&);

	const char* GetClassName() const {return "LPCExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c);
public:
	LPCExample(const LPCExampleConfig &cfg);
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do() { return true; }
	bool Do( DataArray& A, DataArray& K, TData& E );
};

bool LPCExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();

	try {
		AudioManager::Current().Start();
	}
	catch (Err) {
		throw(ErrProcessingObj("Could not start AudioManager",this));
	}
	
	return true;
}




void LPCExample::AttachChildren()
{
	mFileIn.SetParent(this);
	mLPC.SetParent(this);
}


bool LPCExample::ConfigureChildren()
{
	AudioFileConfig fcfg;
	fcfg.SetFilename(mConfig.GetFilename());
	fcfg.SetFiletype(EAudioFileType::eWave);
	CLAM_DEBUG_ASSERT(mSize>0, "no positive frame size");
	fcfg.SetFrameSize(mSize);
	fcfg.SetChannels(1);
	mFileIn.Configure(fcfg);

	//mSize = mFileIn.Size();

	LPCConfig LPC_cfg;
	LPC_cfg.SetName( "LPC" );
	LPC_cfg.SetOrder( mConfig.GetOrder() );

	mLPC.Configure( LPC_cfg );

	return true;
}

void LPCExample::ConfigureAudio(Audio& a)
{
	a.SetSize(mSize);
	a.SetSampleRate(AudioManager::Current().SampleRate());
}


bool LPCExample::ConfigureData()
{
	ConfigureAudio(mInputData);
	return true;
}

bool LPCExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);
	ConfigureChildren();
	ConfigureData();
	return true;
}

LPCExample::LPCExample(const LPCExampleConfig &cfg)
	: mSize(512)
{
	AttachChildren();
	Configure(cfg);
}

bool LPCExample::Do( DataArray& A, DataArray& K, TData& E )
{
		mFileIn.Do(mInputData);
		mLPC.Do( mInputData, A, K, E);

	return true;
}

int main( int argc, char* argv[] )
{
	try{
		int i , Size = 512;
		int LPC_order = 11;
		float SampleRate = 44100.0;

		AudioManager manager(SampleRate, Size);
//		manager.SetInternalBuffersNumber( 8 ); // DGGPORT
		manager.Start();

		CLAM::DataArray A;	// filter coefficients
		CLAM::DataArray K;	// reflection coefficients
		CLAM::TData E;	// average of mean squared error of the filter

		// Polynomial configuration
		A.Resize( LPC_order );
		A.SetSize( LPC_order );
				
		// DataArray configuration
		K.Resize( LPC_order );
		K.SetSize( LPC_order );

		// Processing composite configuration
		LPCExampleConfig myAppConfig;
		myAppConfig.SetName( "My_LPC" );
		myAppConfig.SetFilename( "C:\\aeiou_low.wav" );
		myAppConfig.SetOrder( LPC_order );

		// Processing composite creation
		LPCExample myApp( myAppConfig );

		// End of Processing composite configuration
		myApp.Start();

		// Processing composite execution
		std::cout << "Running object" << std::endl;
		myApp.Do( A, K, E );

		CLAM::XMLStorage::Dump( A, "ACoefficients", "LPC_exampleA.xml" );
		CLAM::XMLStorage::Dump( K, "BCoefficients", "LPC_exampleB.xml" );

	}
	catch( CLAM::Err err )
	{
		err.Print();
	}

	return 0;
}
