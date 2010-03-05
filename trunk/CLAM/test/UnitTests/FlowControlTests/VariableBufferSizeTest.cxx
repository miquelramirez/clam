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

#include <cppunit/extensions/HelperMacros.h>
#include "Processing.hxx" // CLAM
#include "Network.hxx" // CLAM
#include "BasicFlowControl.hxx" // CLAM
#include <string>
#include "BaseLoggable.hxx"
#include "InPort.hxx" // CLAM
#include "OutPort.hxx" // CLAM
#include "InControl.hxx" // CLAM
#include "OutControl.hxx" // CLAM
#include "DummyProcessingData.hxx"

#include "Oscillator.hxx" // CLAM
#include "AudioMultiplier.hxx" // CLAM
#include "MonoAudioFileReader.hxx" // CLAM
#include "AudioSink.hxx" // CLAM
#include "AudioSource.hxx" // CLAM
#include "AudioBufferSink.hxx" // CLAM
#include "AudioBufferSource.hxx" // CLAM
#include "AudioOut.hxx" // CLAM
#include "AudioIn.hxx" // CLAM

namespace CLAMTest {

class FlowChangingTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FlowChangingTest );


class FlowChangingTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FlowChangingTest );
		
	CPPUNIT_TEST( testAudioBufferSourcePropagatesAudioSize );
	CPPUNIT_TEST( testAudioBufferSource_keepsAllocatedSize_changingToLowerSize );
	CPPUNIT_TEST( testAudioBufferSource_keepsAllocatedSize_changingToBiggerSize );


	CPPUNIT_TEST_SUITE_END();

	class DummyProcessing : public CLAM::Processing
	{
		CLAM::InPort<CLAM::Audio> _in;
		unsigned _size;
	public:
		const char* GetClassName() const
		{
			return "DummyProcessing";
		}
		bool Do()
		{
			_size=_in.GetData().GetBuffer().AllocatedSize();
			_in.Consume();
			return false;
		}

		unsigned getAllocatedSize() const
		{	
			return _size;
		}

		DummyProcessing() 
			: _in("Dummy In", this)
			, _size(0)
		{
			Configure(Config());
		}
		

	};

	void testAudioBufferSourcePropagatesAudioSize()
	{
		float audio[30] = {};
		CLAM::AudioBufferSource source;
		DummyProcessing dummy;

		CLAM::ConnectPorts(source, 0, dummy, 0);
		source.Start();
		dummy.Start();
		source.SetExternalBuffer(audio, 30, 0);
		source.Do();
		dummy.Do();
		CPPUNIT_ASSERT_EQUAL(30u, dummy.getAllocatedSize());
	}

	void testAudioBufferSource_keepsAllocatedSize_changingToLowerSize()
	{
		float audio[30] = {};
		CLAM::AudioBufferSource source;
		DummyProcessing dummy;

		CLAM::ConnectPorts(source, 0, dummy, 0);
		source.Start();
		dummy.Start();
		source.SetExternalBuffer(audio, 30, 0);
		source.Do();
		dummy.Do();
		source.SetExternalBuffer(audio, 20, 0);
		source.Do();
		dummy.Do();
		CPPUNIT_ASSERT_EQUAL(30u, dummy.getAllocatedSize());
	}

	void testAudioBufferSource_keepsAllocatedSize_changingToBiggerSize()
	{
		float audio[30] = {};
		CLAM::AudioBufferSource source;
		DummyProcessing dummy;

		CLAM::ConnectPorts(source, 0, dummy, 0);
		source.Start();
		dummy.Start();
		source.SetExternalBuffer(audio, 20, 0);
		source.Do();
		dummy.Do();
		source.SetExternalBuffer(audio, 30, 0);
		source.Do();
		dummy.Do();
		CPPUNIT_ASSERT_EQUAL(30u, dummy.getAllocatedSize());
	}
};
   
} // namespace 

