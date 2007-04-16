
#include <cppunit/extensions/HelperMacros.h>
#include "AudioOutPort.hxx" // CLAM
#include "AudioInPort.hxx" // CLAM
#include "Audio.hxx" // CLAM
#include "AudioOutPortPublisher.hxx" // CLAM
#include "AudioInPortPublisher.hxx" // CLAM

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
	CPPUNIT_TEST( testAudioInPortPublisher_PublishInPort_withIncorrectInPort );
	CPPUNIT_TEST( testAudioInPortPublisher_PublishInPort_withProperInPort );
	CPPUNIT_TEST_SUITE_END();

	void testCreateAudioOutPorts()
	{
		CLAM::AudioOutPort out;
	}

	void testRegionOfAudioOutPort_hasCorrectSize()
	{
		CLAM::AudioOutPort out;
		int frameSize = 64;
		out.SetSize( frameSize );
		CPPUNIT_ASSERT_EQUAL( frameSize, out.GetSize() );
	}

	void testAccessAudioOutPortDataReturnsAudioInsteadOfTData()
	{
		CLAM::AudioOutPort out;
		int frameSize = 64;
		out.SetSize( frameSize );
		CLAM::Audio & myAudio = out.GetAudio();
	}

	void testAudioPortsProduceAndConsume_whenInPortHasEqualSizeOfOne()
	{
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int frameSize = 1;
		CLAM::TData data = 0.5;
		
		out.ConnectToConcreteIn(in);		
		out.SetSize( frameSize );
		in.SetSize( frameSize );
		
		CLAM::Audio & toProduce = out.GetAudio();
			toProduce.GetBuffer()[0] = data;
		out.Produce();
		const CLAM::Audio & toConsume = in.GetAudio();
		CPPUNIT_ASSERT_EQUAL( data, toConsume.GetBuffer()[0] );

	}
	
	void testAudioPortsProduceAndConsume_whenInPortHasEqualSizeGreaterThanOne()
	{
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int frameSize = 8;
		
		out.ConnectToConcreteIn(in);		
		out.SetSize( frameSize );
		out.SetHop( frameSize );
		in.SetSize( frameSize );
		
		CLAM::Audio & toProduce = out.GetAudio();
		for(int i=0;i<frameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		const CLAM::Audio & toConsume = in.GetAudio();
		
		for(int i=0;i<frameSize;i++)
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume.GetBuffer()[i] );
			
	}
	void testAudioPortsProduceAndConsume_afterConsuming()
	{	
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		int frameSize = 4;
		
		out.ConnectToConcreteIn(in);		
		out.SetSize( frameSize );
		out.SetHop( frameSize );
		in.SetSize( frameSize );
		in.SetHop( frameSize );
		
		CLAM::Audio & toProduce = out.GetAudio();
		for(int i=0;i<frameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		in.Consume();

		CLAM::Audio & toProduce2 = out.GetAudio();
		for(int i=0;i<frameSize;i++)
			toProduce2.GetBuffer()[i] = i+frameSize;
		out.Produce();
		const CLAM::Audio & toConsume = in.GetAudio();
		
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
		out.SetSize( writerFrameSize );
		out.SetHop( writerFrameSize );
		in.SetSize( readerFrameSize );
		in.SetHop( readerFrameSize );
		
		CLAM::Audio & toProduce = out.GetAudio();
		for(int i=0;i<writerFrameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		
		CPPUNIT_ASSERT_EQUAL( false, in.CanConsume() );
		
		CLAM::Audio & toProduce2 = out.GetAudio();
		for(int i=0;i<writerFrameSize;i++)
			toProduce2.GetBuffer()[i] = i+writerFrameSize;
		out.Produce();
		
		const CLAM::Audio & toConsume = in.GetAudio();
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
		out.SetSize( frameSize );
		out.SetHop( frameSize );
		in1.SetSize( frameSize );
		in1.SetHop( frameSize );
		in2.SetSize( frameSize );
		in2.SetHop( frameSize );

		CLAM::Audio & toProduce = out.GetAudio();
		for(int i=0;i<frameSize;i++)
			toProduce.GetBuffer()[i] = i;
		out.Produce();
		const CLAM::Audio & toConsume1 = in1.GetAudio();
		const CLAM::Audio & toConsume2 = in2.GetAudio();
		
		for(int i=0;i<frameSize;i++)
		{
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume1.GetBuffer()[i] );
			CPPUNIT_ASSERT_EQUAL( (CLAM::TData)i, toConsume2.GetBuffer()[i] );
		}
	}
	
	void testAudioInPortPublisher_PublishInPort_withIncorrectInPort()
	{
		CLAM::InPort<int> in;
		CLAM::AudioInPortPublisher inPublisher;

		try
		{
			inPublisher.PublishInPort( in );
			CPPUNIT_FAIL( "Assertion should fail" ); 
		}
		catch( CLAM::ErrAssertionFailed & )
		{
		}
	}
	
	void testAudioInPortPublisher_PublishInPort_withProperInPort()
	{
		CLAM::AudioOutPort out;
		CLAM::AudioInPort in;
		CLAM::AudioInPortPublisher inPublisher;
		inPublisher.PublishInPort( in );

		CLAM::TData data(4.0);

		out.ConnectToIn(inPublisher);
		out.GetAudio().GetBuffer()[0] = data;
		out.Produce();
			
		CPPUNIT_ASSERT_EQUAL( data, in.GetAudio().GetBuffer()[0] );
	}
	
	void testAudioOutPortPublisher_PublishOutPort_withProperOutPort()
	{
		
		CLAM::OutPort<CLAM::TData> out;
		CLAM::AudioInPort in;
		CLAM::AudioOutPortPublisher outPublisher;
		outPublisher.PublishOutPort( out );

		CLAM::TData data(4.0);

		outPublisher.ConnectToIn(in);
		out.GetData() = data;

		CPPUNIT_ASSERT_EQUAL( data, outPublisher.GetAudio().GetBuffer()[0] );		
		out.Produce();
		CPPUNIT_ASSERT_EQUAL( data, in.GetAudio().GetBuffer()[0] );
	}

};

} // namespace CLAMTest

