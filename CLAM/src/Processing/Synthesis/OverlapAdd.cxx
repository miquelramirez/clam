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

#include "OverlapAdd.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden 
{
	static const char * metadata[] = {
		"key", "OverlapAdd",
		"category", "Synthesis",
		"description", "OverlapAdd",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, OverlapAdd> regOverlapAdd("OverlapAdd");
	static FactoryRegistrator<ProcessingFactory, OverlapAdd> reg = metadata;
}

/* processing object method implementations */

OverlapAdd::OverlapAdd(const OverlapAddConfig &c)
	: mInput( "Audio Input", this ),
	  mOutput( "Audio Output", this )
{
	Configure(c);
}

OverlapAdd::~OverlapAdd()
{
}


/* configure the processing object according to the config object */

bool OverlapAdd::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);


	int frameSize=mConfig.GetFrameSize();

	mInput.SetSize( frameSize*2 );
	mInput.SetHop( frameSize*2 );

	mOutput.SetSize( frameSize );
	mOutput.SetHop( frameSize );

	mTmp.SetSize( frameSize*2 +1  );

	for(int i=0;i<mTmp.GetSize();i++)
		mTmp.GetBuffer()[i]=0.0f;
	
	return true;
}

/* the supervised Do() function */

bool OverlapAdd::Do(void)
{
	bool result = Do( mInput.GetAudio(), mOutput.GetAudio() );
	mInput.Consume();
	mOutput.Produce();
	return result;
}

bool OverlapAdd::Do( const Audio &in, Audio & out)
{
	//xamat: testing
  /*	std::cout<<"***OverlapAdd Configuration: "<<std::endl;
	std::cout<<"FrameSize: "<<mConfig.GetFrameSize()<<std::endl;
	std::cout<<"Input Audio Size: "<<in.GetSize()<<std::endl;
	std::cout<<"Output Audio Size: "<<out.GetSize()<<std::endl;
  */
  // TODO: refactor
	int halfSize = in.GetSize()/2;
	CLAM_DEBUG_ASSERT( out.GetSize() == halfSize, "OverlapAdd::Do - Audio Out size must be half the input size" );
	CLAM_DEBUG_ASSERT( mConfig.GetFrameSize() == halfSize, "OverlapAdd::Do - Config FrameSize must be half the input size" );
	
	CLAM::DataArray & inBuffer = in.GetBuffer();
	CLAM::DataArray & outBuffer = out.GetBuffer();
	CLAM::DataArray & tmpBuffer = mTmp.GetBuffer();
	const TSize outSize = out.GetSize();
	const TSize inSize = in.GetSize();
	
	
	for( int i=0;i<halfSize;i++)
	{
		tmpBuffer[i] = tmpBuffer[i+halfSize] + inBuffer[i];
	}
	
	for( int i=halfSize;i<inSize;i++)
	{
		tmpBuffer[i] = inBuffer[i];
	}
	
	for( int i=0;i<outSize;i++)
	{
		outBuffer[i] = tmpBuffer[i];
	}
	
	return true;
} 


} // namespace CLAM

