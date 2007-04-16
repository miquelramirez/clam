#include <cppunit/extensions/HelperMacros.h>

#include "CLAM_Math.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"

namespace CLAMTest
{

class AudioManagerTest;

CPPUNIT_TEST_SUITE_REGISTRATION( AudioManagerTest );

class AudioManagerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( AudioManagerTest );
	CPPUNIT_TEST( testOldAudioManagerExample );
	CPPUNIT_TEST( testOldAudioManagerExample );
	CPPUNIT_TEST( testOldAudioManagerExample );
	CPPUNIT_TEST( testAudioOut_withNoAudioManagerVariable );
	CPPUNIT_TEST( testAudioOut_withNoAudioManagerVariable );
	CPPUNIT_TEST( testAudioOut_withNoAudioManagerVariable );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	CLAM::TData testSignal1( long i)
	{
		static CLAM::TData f=0;
		f = f+0.02;
		if (f>2.* M_PI) f -= 2.*M_PI;

		return 0.5*std::sin( f + std::sin(2.*f)*10. );

	}
	CLAM::TData testSignal2( long i)
	{
		static CLAM::TData f=0;
		f = f+0.025;
		if (f>2.* M_PI) f -= 2.*M_PI;

		return 0.5*std::sin( f + std::sin(f)*20. );

	}



	void testOldAudioManagerExample()
	{
		const int sampleRate = 44100;
		const CLAM::TSize frameSize = 1024;

		CLAM::AudioManager theAudioManager( sampleRate, frameSize*5 );

		CLAM::AudioOut leftChannel;
		CLAM::AudioOut rightChannel;

		CLAM::AudioIOConfig  audioOutCfg;
		audioOutCfg.SetChannelID( 0 ); //TODO necessary?

		leftChannel.Configure( audioOutCfg );
		audioOutCfg.SetChannelID( 1 );
		rightChannel.Configure( audioOutCfg );

		// And we may now start them
//		theAudioManager.Start(); // TODO: what does? apparently is not necessary
		leftChannel.Start();
		rightChannel.Start();

		CLAM::Audio leftSamples;
		CLAM::Audio rightSamples;

		leftSamples.SetSize( frameSize );
		rightSamples.SetSize( frameSize );
//		leftSamples.SetSampleRate( sampleRate );
//		rightSamples.SetSampleRate( sampleRate );


		// And now let's start the loop
		long index=0;
		while ( index < 7000 )
		{
			// And now we may wrote to the audio device
			for(int i=0; i< frameSize; i++, index++)
			{
				leftSamples.GetBuffer()[i] = testSignal1(index);
				rightSamples.GetBuffer()[i] = testSignal2(index);

			}
			leftChannel.Do( leftSamples );
			rightChannel.Do( rightSamples );
		}
		// And finally, we stop the AudioOut's
		leftChannel.Stop();
		rightChannel.Stop();

		CPPUNIT_FAIL( "*** Expected Result ***, *** Given Result ***" );
	}

	void testAudioOut_withNoAudioManagerVariable()
	{
		CLAM::AudioOut leftChannel;
		CLAM::AudioOut rightChannel;
		const int frameSize = 1024;
		CLAM::AudioManager theAudioManager( 44100, frameSize*5 );

		CLAM::AudioIOConfig  audioOutCfg;
		audioOutCfg.SetChannelID( 0 ); //TODO necessary?

		leftChannel.Configure( audioOutCfg );
		audioOutCfg.SetChannelID( 1 );
		rightChannel.Configure( audioOutCfg );

		// And we may now start them
		theAudioManager.Start(); // TODO: what does? apparently is not necessary
		leftChannel.Start();
		rightChannel.Start();

		CLAM::Audio leftSamples;
		CLAM::Audio rightSamples;

		leftSamples.SetSize( frameSize );
		rightSamples.SetSize( frameSize );
//		leftSamples.SetSampleRate( sampleRate );
//		rightSamples.SetSampleRate( sampleRate );


		// And now let's start the loop
		long index=0;
		while ( index < 7000 )
		{

			// And now we may wrote to the audio device
			for(int i=0; i< frameSize; i++, index++)
			{
				leftSamples.GetBuffer()[i] = testSignal2(index);
				rightSamples.GetBuffer()[i] = testSignal1(index);

			}
			leftChannel.Do( leftSamples );
			rightChannel.Do( rightSamples );
		}
		// And finally, we stop the AudioOut's
		leftChannel.Stop();
		rightChannel.Stop();

		CPPUNIT_FAIL( "*** Expected Result ***, *** Given Result ***" );
	}

	// TODO chech asserts when missing device srcfile in project
	// TODO idem for library missing
};



} // namespace CLAMTest

