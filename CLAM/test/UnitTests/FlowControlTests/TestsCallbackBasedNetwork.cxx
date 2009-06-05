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
#include <CLAM/NaiveFlowControl.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>

namespace CLAMTest {


class TestsCallbackBasedNetwork;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsCallbackBasedNetwork );


class DummyIsolated : public CLAM::Processing
{
	unsigned _next;
public:
	DummyIsolated(unsigned portSize)
		: _next(0)
	{
		SetExecState(Ready);
	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig &c) { return true; }
	const CLAM::ProcessingConfig & GetConfig() const
	{
		static CLAM::NullProcessingConfig config;
		return config;
	}
	const char * GetClassName() const { return "DummyIsolated";}
	bool Do() 
	{ 
		++_next;
		return true;
	}
	unsigned GetNext() const { return _next; }
};

class DummyGenerator : public CLAM::Processing
{
	CLAM::AudioOutPort _out;
	unsigned _next;
public:
	DummyGenerator(unsigned portSize)
		: _out("out", this)
		, _next(0)
	{
		_out.SetSize(portSize);
		_out.SetHop(portSize);

		SetExecState(Ready);

	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig &c) { return true; }
	const CLAM::ProcessingConfig & GetConfig() const
	{
		static CLAM::NullProcessingConfig config;
		return config;
	}
	const char * GetClassName() const { return "DummyGenerator";}
	
	bool Do()
	{
		CLAM::Audio & out = _out.GetAudio();
		for (unsigned i=0; i<out.GetSize(); i++)
		{
			out.GetBuffer()[i] = ++_next;
		}
		_out.Produce();
		return true;
	}
	unsigned GetNext() const { return _next; }
};
class DummyFilter : public CLAM::Processing
{
	CLAM::AudioInPort _in;
	CLAM::AudioOutPort _out;
	std::vector<float> _log;
	unsigned _iLog;
public:
	DummyFilter(unsigned portSize)
		: _in("in", this)
		, _out("out", this)
		, _log(204800, -1.0)
		, _iLog(0)
	{
		_in.SetSize(portSize);
		_in.SetHop(portSize);
		_out.SetSize(portSize);
		_out.SetHop(portSize);

		SetExecState(Ready);

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
		CLAM_ASSERT(in.GetSize()==out.GetSize(), "DummyFilter: sizes missmatch");
		for (unsigned i=0; i<in.GetSize(); i++)
		{
			out.GetBuffer()[i] = in.GetBuffer()[i];
			_log[_iLog++] = in.GetBuffer()[i];
		}
		_in.Consume();
		_out.Produce();
		return true;
	}
	const float * GetLog() const
	{
		return &(_log[0]);
	}
};

class TestsCallbackBasedNetwork : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestsCallbackBasedNetwork );

	CPPUNIT_TEST( testSourceAndSink );
	CPPUNIT_TEST( testUnconnectedSourceAndSink );
	CPPUNIT_TEST( testSourceFilterSink_sameSize );
	CPPUNIT_TEST( testSourceFilterSink_smallerSize );
	CPPUNIT_TEST( testSourceFilterSink_biggerSize );
	CPPUNIT_TEST( testSourceFilterSink_biggerNonDivisorSize );
	CPPUNIT_TEST( testSlowSinkLessBranch );
	CPPUNIT_TEST( testSlowSinkLessBranch_doNotAccomulate );
	CPPUNIT_TEST( testGeneratorProducesOnlyWhatSinkNeeds );
	CPPUNIT_TEST( testIsolatedGetsExecutedOnlyOnce );

	CPPUNIT_TEST_SUITE_END();
	float _inFloat[2048000];
	float _outFloat[2048000];
	CLAM::Network _network;
public:
	void setUp()
	{
		_network.AddFlowControl(new CLAM::NaiveFlowControl);
		for (unsigned i=0; i<204800; i++)
		{
			_inFloat[i]=i+1;
			_outFloat[i]=-1;
		}
	}
private:
	void assertSamplesTransferred(unsigned howMany, unsigned lag=0)
	{
		assertSamplesTransferredToBuffer(_outFloat, howMany, lag);
	}
	void assertSamplesTransferredToBuffer(const float * buffer, unsigned howMany, unsigned lag=0)
	{
		try {
			for (unsigned i=0; i<lag; i++)
				CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, buffer[i], 1e-14);
			for (unsigned i=0; i<howMany; i++)
				CPPUNIT_ASSERT_DOUBLES_EQUAL(_inFloat[i], buffer[i+lag], 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, buffer[howMany+lag], 1e-14);
		}
		catch (...)
		{
			std::cerr << std::endl;
			std::cerr << "Expected: ";
			for (unsigned i=0; i<lag; i++)
				std::cerr << "-1 ";
			for (unsigned i=0; i<howMany; i++)
				std::cerr << _inFloat[i] << " ";
			std::cerr << "-1 ";
			std::cerr << std::endl;
			std::cerr << "Actual:   ";
			for (unsigned i=0; i<howMany+lag; i++)
				std::cerr << buffer[i] << " ";
			std::cerr << buffer[howMany] << " ";
			std::cerr << std::endl;
			throw;
		}
	
	}
	void testSourceAndSink()
	{
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.ConnectPorts("Source.1", "Sink.1");
		_network.Start();
		source->SetExternalBuffer(_inFloat, 2, 0);
		sink->SetExternalBuffer(_outFloat, 2, 0);
		_network.Do();
		_network.Stop();
		assertSamplesTransferred(2);
	}
	void testUnconnectedSourceAndSink()
	{
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.Start();
		source->SetExternalBuffer(_inFloat, 2, 0);
		sink->SetExternalBuffer(_outFloat, 2, 0);
		_network.Do();
		_network.Stop();
		assertSamplesTransferred(0);
	}
	void testSourceFilterSink_sameSize()
	{
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(2);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Filter", filter);
		_network.ConnectPorts("Source.1", "Filter.in");
		_network.ConnectPorts("Filter.out", "Sink.1");
		_network.Start();
		source->SetExternalBuffer(_inFloat, 2, 0);
		sink->SetExternalBuffer(_outFloat, 2, 0);
		_network.Do();
		_network.Stop();
		assertSamplesTransferred(2);
	}
	void testSourceFilterSink_smallerSize()
	{
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(1);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Filter", filter);
		_network.ConnectPorts("Source.1", "Filter.in");
		_network.ConnectPorts("Filter.out", "Sink.1");
		_network.Start();
		source->SetExternalBuffer(_inFloat, 2, 0);
		sink->SetExternalBuffer(_outFloat, 2, 0);
		_network.Do();
		_network.Stop();
		assertSamplesTransferred(2);
	}
	void testSourceFilterSink_biggerSize()
	{
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(4);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Filter", filter);
		_network.ConnectPorts("Source.1", "Filter.in");
		_network.ConnectPorts("Filter.out", "Sink.1");
		_network.Start();
		source->SetExternalBuffer(_inFloat, 2, 0);
		sink->SetExternalBuffer(_outFloat, 2, 0);
		_network.Do();
		assertSamplesTransferred(0,2);
		source->SetExternalBuffer(_inFloat+2, 2, 0);
		sink->SetExternalBuffer(_outFloat+2, 2, 0);
		_network.Do();
		assertSamplesTransferred(2,2);
		source->SetExternalBuffer(_inFloat+4, 2, 0);
		sink->SetExternalBuffer(_outFloat+4, 2, 0);
		_network.Do();
		assertSamplesTransferred(4,2);
		source->SetExternalBuffer(_inFloat+6, 2, 0);
		sink->SetExternalBuffer(_outFloat+6, 2, 0);
		_network.Do();
		assertSamplesTransferred(6,2);
		_network.Stop();
	}
	void testSourceFilterSink_biggerNonDivisorSize()
	{
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(5);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Filter", filter);
		_network.ConnectPorts("Source.1", "Filter.in");
		_network.ConnectPorts("Filter.out", "Sink.1");
		_network.Start();
		source->SetExternalBuffer(_inFloat, 2, 0);
		sink->SetExternalBuffer(_outFloat, 2, 0);
		_network.Do();
		assertSamplesTransferred(0,2);
		source->SetExternalBuffer(_inFloat+2, 2, 0);
		sink->SetExternalBuffer(_outFloat+2, 2, 0);
		_network.Do();
		assertSamplesTransferred(0,4);
		source->SetExternalBuffer(_inFloat+4, 2, 0);
		sink->SetExternalBuffer(_outFloat+4, 2, 0);
		_network.Do();
		assertSamplesTransferred(2,4);
		source->SetExternalBuffer(_inFloat+6, 2, 0);
		sink->SetExternalBuffer(_outFloat+6, 2, 0);
		_network.Do();
		assertSamplesTransferred(4,4);
		_network.Stop();
	}
	void testSlowSinkLessBranch()
	{
		const unsigned dummyStep = 1;
		const unsigned callbackStep = 100;
		const unsigned iterations = 15;
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(dummyStep);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Filter", filter);
		_network.ConnectPorts("Source.1", "Filter.in");
		_network.ConnectPorts("Source.1", "Sink.1");
		_network.Start();
		for (int i=0; i<iterations; i++)
		{
			source->SetExternalBuffer(_inFloat+i*callbackStep, callbackStep, 0);
			sink->SetExternalBuffer(_outFloat+i*callbackStep, callbackStep, 0);
			_network.Do();
		}
		assertSamplesTransferred(iterations*callbackStep);
		_network.Stop();
	}
	void testSlowSinkLessBranch_doNotAccomulate()
	{
		const unsigned callbackStep = 100;
		CLAM::AudioSource * source = new CLAM::AudioSource;
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyFilter * filter = new DummyFilter(1);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Filter", filter);
		_network.ConnectPorts("Source.1", "Filter.in");
		_network.ConnectPorts("Source.1", "Sink.1");
		_network.Start();
		source->SetExternalBuffer(_inFloat, callbackStep, 0);
		sink->SetExternalBuffer(_outFloat, callbackStep, 0);
		_network.Do();
		assertSamplesTransferredToBuffer(filter->GetLog(), 100);
		_network.Stop();
	}
	void testGeneratorProducesOnlyWhatSinkNeeds()
	{
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		DummyGenerator * generator = new DummyGenerator(3);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("Generator", generator);
		_network.ConnectPorts("Generator.out", "Sink.1");
		_network.Start();
		sink->SetExternalBuffer(_outFloat, 4, 0);
		_network.Do();
		CPPUNIT_ASSERT_EQUAL(6u, generator->GetNext());
		assertSamplesTransferred(4);
		_network.Stop();
	}
	void testIsolatedGetsExecutedOnlyOnce()
	{
		CLAM::AudioSink * sink = new CLAM::AudioSink;
		CLAM::AudioSource * source = new CLAM::AudioSource;
		DummyIsolated * isolated = new DummyIsolated(1);
		_network.AddProcessing("Source", source);
		_network.AddProcessing("Sink", sink);
		_network.AddProcessing("IsolatedGenerator", isolated);
		_network.ConnectPorts("Source.1", "Sink.1");
		_network.Start();
		sink->SetExternalBuffer(_outFloat, 4, 0);
		source->SetExternalBuffer(_inFloat, 4, 0);
		_network.Do();
		CPPUNIT_ASSERT_EQUAL(1u, isolated->GetNext());
		assertSamplesTransferred(4);
		_network.Stop();
	}
};

} // namespace

