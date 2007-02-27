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
#include <CLAM/PullFlowControl.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>

namespace CLAMTest {


class TestsCallbackBasedNetwork;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsCallbackBasedNetwork );

class DummyFilter : public CLAM::Processing
{
	CLAM::AudioInPort _in;
	CLAM::AudioOutPort _out;
public:
	DummyFilter(unsigned portSize)
		: _in("in", this)
		, _out("out", this)
	{
		_in.SetSize(portSize);
		_in.SetHop(portSize);
		_out.SetSize(portSize);
		_out.SetHop(portSize);

		mExecState=Ready; //PAU: don¡t understand. it should be done automatically by Configure

	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig &c)
	{
		return true;
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		static CLAM::NullProcessingConfig config;
		return config;
	}
	const char * GetClassName() const { return "DummyFilter";}
	
	bool Do()
	{
		const CLAM::Audio & in = _in.GetAudio();
		CLAM::Audio & out = _out.GetAudio();
		out.GetBuffer()[0]=0;
		out.GetBuffer()[1]=1;
		out.GetBuffer()[2]=2;
		_in.Consume();
		_out.Produce();
		return true;
	}
};

class TestsCallbackBasedNetwork : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestsCallbackBasedNetwork );
		
	//CPPUNIT_TEST( testSourceAndSink );
	CPPUNIT_TEST( testSourceFilterSink_sameSize );

	CPPUNIT_TEST_SUITE_END();
	float _inFloat[2048];
	float _outFloat[2048];
public:
	void setUp()
	{
		for (unsigned i=0; i<2048; i++)
		{
			_inFloat[i]=i;
			_outFloat[i]=-1;
		}
	}
private:
	void assertSamplesTransferred(unsigned howMany)
	{
		for (unsigned i=0; i<howMany; i++)
		{
			std::cout << "in " << _inFloat[i] << " out " << _outFloat[i] << std::flush;
			CPPUNIT_ASSERT_DOUBLES_EQUAL(_inFloat[i], _outFloat[i], 1e-14);
		}
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, _outFloat[howMany], 1e-14);
	}
	void testSourceAndSink()
	{
		CLAM::Network network;
		network.AddFlowControl(new CLAM::PullFlowControl);
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		network.AddProcessing("Source", source);
		network.AddProcessing("Sink", sink);
		network.ConnectPorts("Source.AudioOut", "Sink.AudioIn");
		network.Start();
		source->SetExternalBuffer(_inFloat, 2);
		sink->SetExternalBuffer(_outFloat, 2);
		network.Do();
		network.Stop();
		assertSamplesTransferred(2);
	}
	void testSourceFilterSink_sameSize()
	{
		CLAM::Network network;
		network.AddFlowControl(new CLAM::PullFlowControl);
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(2);
		filter->Start();
		network.AddProcessing("Source", source);
		network.AddProcessing("Sink", sink);
		network.AddProcessing("Filter", filter);
		network.ConnectPorts("Source.AudioOut", "Filter.in");
		network.ConnectPorts("Filter.out", "Sink.AudioIn");
		network.Start();
		source->SetExternalBuffer(_inFloat, 2);
		sink->SetExternalBuffer(_outFloat, 2);
		network.Do();
		network.Stop();
		assertSamplesTransferred(2);
	}
	// source -> sink
	
	// source-> filter -> sink
	// net.AddProcessing( Dummy(1,1)
};

} // namespace
