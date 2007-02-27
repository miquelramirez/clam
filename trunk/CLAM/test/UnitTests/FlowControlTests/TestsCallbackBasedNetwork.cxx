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
#include <CLAM/Network.hxx>
#include <CLAM/PushFlowControl.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>

namespace CLAMTest {


class TestsCallbackBasedNetwork;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsCallbackBasedNetwork );



class TestsCallbackBasedNetwork : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestsCallbackBasedNetwork );
		
	CPPUNIT_TEST( testSourceAndSink );

	CPPUNIT_TEST_SUITE_END();
	float _inFloatBuffer[2048];
	float _outFloatBuffer[2048];
	void setUp()
	{
		for (unsigned i=0; i<2048; i++)
		{
			_inFloatBuffer[i]=i;
			_outFloatBuffer[i]=-1;
		}
	}
	void testSourceAndSink()
	{
		CLAM::Network network;
		network.AddFlowControl(new CLAM::PushFlowControl);
		CLAM::Processing * source = new CLAM::AudioSource;
		CLAM::Processing * sink = new CLAM::AudioSink;
		network.AddProcessing("Source", source);
		network.AddProcessing("Sink", sink);
		network.ConnectPorts("Source.AudioOut", "Sink.AudioIn");
		network.Start();
		source.SetExternalBuffer(_inFloatBuffer, 2);
		sink.SetExternalBuffer(_outFloatBuffer, 2);
		network.Do();
		network.Stop();
	}
	// source -> sink
	
	// source-> filter -> sink
	// net.AddProcessing( Dummy(1,1)
};

} // namespace
