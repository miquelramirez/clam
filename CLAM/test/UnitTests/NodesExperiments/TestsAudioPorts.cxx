
#include <cppunit/extensions/HelperMacros.h>
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"
#include "Audio.hxx"

namespace CLAMTest {

class TestsAudioPorts ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsAudioPorts );

class TestsAudioPorts : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsAudioPorts );
	
	CPPUNIT_TEST( testCreateAudioOutPorts );
	CPPUNIT_TEST( testRegionOfAudioOutPort_hasCorrectSize );
	CPPUNIT_TEST( testAccessAudioOutPortDataReturnsAudioInsteadOfTData );
	CPPUNIT_TEST( testAudioPortsProduceAndConsume_whenInPortHasEqualSizeOfOne );
	CPPUNIT_TEST( testAudioPortsProduceAndConsume_whenInPortHasEqualSizeGreaterThanOne );
	CPPUNIT_TEST( testAudioPortsProduceAndConsume_afterConsuming );
	CPPUNIT_TEST( testAudioPortsProduceAndConsume_whenMoreThanOneReader );
	CPPUNIT_TEST( testAudioPortsProduceAndConsume_whenDifferentSizes );
	CPPUNIT_TEST_SUITE_END();

	void testCreateAudioOutPorts()
	{
		CLAM::AudioOutPort out;
	}

	void testRegionOfAudioOutPort_hasCorrectSize()
	{
		CLAM::AudioOutPort out;
		int frameSize = 64;
		out.SetAudioSize( frameSize );
		CPPUNIT_ASSERT_EQUAL( frameSize, out.GetAudioSize() );
	}

	void testAccessAudioOutPortDataReturnsAudioInsteadOfTData()
	{
		CLAM::AudioOutPort out;
		int frameSize = 64;
		out.SetAudioSize( frameSize );
		CLAM::Audio & myAudio = out[0];
	}

	void testAudioPortsProduceAndConsume_whenInPortHasEqualSizeOfOne()
	{
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int frameSize = 1;
		CLAM::TData data = 0.5;
		
		out.ConnectToConcreteIn(in);		
		out.SetAudioSize( frameSize );
		in.SetAudioSize( frameSize );
		
		CLAM::Audio & toProduce = out[0];
			toProduce.GetBuffer()[0] = data;
		out.Produce();
		const CLAM::Audio & toConsume = in[0];
		CPPUNIT_ASSERT_EQUAL( data, toConsume.GetBuffer()[0] );

	}
	
	void testAudioPortsProduceAndConsume_whenInPortHasEqualSizeGreaterThanOne()
	{
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int frameSize = 8;
		
		out.ConnectToConcreteIn(in);		
		out.SetAudioSize( frameSize );
		out.SetHop( frameSize );
		in.SetAudioSize( frameSize );
		
		CLAM::Audio & toProduce = out[0];
		for(int i=0;i<frameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		const CLAM::Audio & toConsume = in[0];
		
		for(int i=0;i<frameSize;i++)
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume.GetBuffer()[i] );
			
	}
	void testAudioPortsProduceAndConsume_afterConsuming()
	{	
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int frameSize = 4;
		
		out.ConnectToConcreteIn(in);		
		out.SetAudioSize( frameSize );
		out.SetHop( frameSize );
		in.SetAudioSize( frameSize );
		in.SetHop( frameSize );
		
		CLAM::Audio & toProduce = out[0];
		for(int i=0;i<frameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		in.Consume();

		CLAM::Audio & toProduce2 = out[0];
		for(int i=0;i<frameSize;i++)
			toProduce2.GetBuffer()[i] = i+frameSize;
		out.Produce();
		const CLAM::Audio & toConsume = in[0];
		
		for(int i=0;i<frameSize;i++)
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)(i+frameSize), toConsume.GetBuffer()[i] );

	}
	
	void testAudioPortsProduceAndConsume_whenDifferentSizes()
	{	
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int writerFrameSize = 4;
		int readerFrameSize = 6;
		
		out.ConnectToConcreteIn(in);		
		out.SetAudioSize( writerFrameSize );
		out.SetHop( writerFrameSize );
		in.SetAudioSize( readerFrameSize );
		in.SetHop( readerFrameSize );
		
		CLAM::Audio & toProduce = out[0];
		for(int i=0;i<writerFrameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		
		CPPUNIT_ASSERT_EQUAL( false, in.CanConsume() );
		
		CLAM::Audio & toProduce2 = out[0];
		for(int i=0;i<writerFrameSize;i++)
			toProduce2.GetBuffer()[i] = i+writerFrameSize;
		out.Produce();
		
		const CLAM::Audio & toConsume = in[0];
		for(int i=0;i<readerFrameSize;i++)
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume.GetBuffer()[i] );
	}
	
	void testAudioPortsProduceAndConsume_whenMoreThanOneReader()
	{	
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in1, in2;
		int frameSize = 4;
		
		out.ConnectToConcreteIn(in1);		
		out.ConnectToConcreteIn(in2);		
		out.SetAudioSize( frameSize );
		out.SetHop( frameSize );
		in1.SetAudioSize( frameSize );
		in1.SetHop( frameSize );
		in2.SetAudioSize( frameSize );
		in2.SetHop( frameSize );

		CLAM::Audio & toProduce = out[0];
		for(int i=0;i<frameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		const CLAM::Audio & toConsume1 = in1[0];
		const CLAM::Audio & toConsume2 = in2[0];
		
		for(int i=0;i<frameSize;i++)
		{
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume1.GetBuffer()[i] );
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume2.GetBuffer()[i] );
		}
	}
};

} // namespace CLAMTest
