#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "AudioFile.hxx" // CLAM
#include "AudioFileHeader.hxx" // CLAM
#include "AudioFileFormats.hxx" // CLAM

#include "XMLStorage.hxx" // CLAM
#include <fstream>

namespace CLAMTest
{
	class AudioFileTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( AudioFileTest );

	class AudioFileTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( AudioFileTest );

		CPPUNIT_TEST( testOpenExisting_FileExists_and_Is_PCM );
		CPPUNIT_TEST( testOpenExisting_FileExists_and_Is_OggVorbis );
		CPPUNIT_TEST( testOpenExisting_FileExists_and_Is_Mpeg );
		CPPUNIT_TEST( testOpenExisting_FileExists_NotSpurious_Mpeg );
		
		CPPUNIT_TEST( testOpenExisting_FileDoesNotExist_UnrecognizedFormat );

		CPPUNIT_TEST( testIsReadable_with_strangeThings );

		CPPUNIT_TEST( testGetHeader_HeaderIsRight_PCM );
		CPPUNIT_TEST( testGetHeader_HeaderIsRight_OggVorbis );
		CPPUNIT_TEST( testGetHeader_HeaderIsRight_Mpeg );

		CPPUNIT_TEST( testGetHeader_NoHeaderWhenFileIsUnreadable );
		CPPUNIT_TEST( testGetHeader_NoHeaderWhenFileIsUnreadable_AfterOneSuccessful );

		CPPUNIT_TEST( testCreateNew_UserDefinesHeaderForWriting_PCM_RightCodecIsDeduced );
		CPPUNIT_TEST( testCreateNew_UserDefinesHeaderForWriting_Ogg_RightCodecIsDeduced );
		CPPUNIT_TEST( testCreateNew_WithoutSampleRate_Fails );
		CPPUNIT_TEST( testCreateNew_WithoutChannels_IsOK );
		CPPUNIT_TEST( testCreateNew_WithoutFormat_Fails );
	
		CPPUNIT_TEST( testCreateNew_SetValues_WAV );
		CPPUNIT_TEST( testCreateNew_SetValues_AIFF );
		CPPUNIT_TEST( testCreateNew_SetValues_OggVorbis );

		CPPUNIT_TEST( testCreateNew_SetValues_WAV_AreWritable );
		CPPUNIT_TEST( testCreateNew_SetValues_AIFF_AreWritable );
		CPPUNIT_TEST( testCreateNew_SetValues_OggVorbis_AreWritable );

		CPPUNIT_TEST( testIsWritable_ReturnsTrue_PCM_WithReasonableHeader );
		CPPUNIT_TEST( testIsWritable_ReturnsFalse_PCM_TooManyChannels );
		CPPUNIT_TEST( testIsWritable_ReturnsFalse_PCM_TooFewChannels );

		CPPUNIT_TEST( testTextDescriptorsExtraction_From_OggVorbis );
		CPPUNIT_TEST( testTextDescriptorsExtraction_From_Mpeg );

		CPPUNIT_TEST_SUITE_END();
	public:

		void setUp()
		{
			mPathToTestData = GetTestDataDirectory();
		}

		void tearDown()
		{
		}
	protected:

		std::string mPathToTestData;

	private: // test cases

		void testIsReadable_with_strangeThings()
		{
			CLAM::AudioFileSource file;
			
			file.OpenExisting( std::string( mPathToTestData + "Image.jpg" ) );

			CPPUNIT_ASSERT_EQUAL( false,
					      file.IsReadable() );
				
		}


		void testIsWritable_ReturnsTrue_PCM_WithReasonableHeader()
		{
			CLAM::AudioFileTarget file;


			CLAM::AudioFileHeader header;

			header.AddAll();
			header.UpdateData();
			header.SetChannels( 1 );
			header.SetSampleRate( 22050 );
			header.SetFormat( CLAM::EAudioFileFormat::eWAV );
			header.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			header.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			file.CreateNew( "NewFile.wav", header );
			CPPUNIT_ASSERT_EQUAL( true, file.IsWritable() );
		}

		void testIsWritable_ReturnsFalse_PCM_TooManyChannels()
		{
			CLAM::AudioFileTarget file;


			CLAM::AudioFileHeader header;

			header.AddAll();
			header.UpdateData();
			header.SetChannels( 291 );
			header.SetSampleRate( 22050 );
			header.SetFormat( CLAM::EAudioFileFormat::eWAV );
			header.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			header.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			file.CreateNew( "NewFile.wav", header );
			
			CPPUNIT_ASSERT_EQUAL( false, file.IsWritable() );		
		}

		void testIsWritable_ReturnsFalse_PCM_TooFewChannels()
		{
			CLAM::AudioFileTarget file;

			
			CLAM::AudioFileHeader header;

			header.AddAll();
			header.UpdateData();
			header.SetChannels( 0 );
			header.SetSampleRate( 22050 );
			header.SetFormat( CLAM::EAudioFileFormat::eWAV );
			header.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			header.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			file.CreateNew( "NewFile.wav", header  );
			
			CPPUNIT_ASSERT_EQUAL( false,
					      file.IsWritable() );		

		}


		void testOpenExisting_FileExists_and_Is_PCM()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string("Elvis.wav") );

			CPPUNIT_ASSERT_EQUAL( std::string("PCM"),
					      file.GetKind().GetString() );
		}

		void testOpenExisting_FileExists_and_Is_OggVorbis()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "Elvis.ogg" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("Ogg/Vorbis"),
					      file.GetKind().GetString() );			
		}

		void testOpenExisting_FileDoesNotExist_UnrecognizedFormat()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string("nikora") );

			CPPUNIT_ASSERT_EQUAL( std::string( "Unknown" ),
					      file.GetKind().GetString() );
		}

		void testGetHeader_HeaderIsRight_PCM()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "Elvis.wav" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( int(133888),
					      int(header.GetSamples()) );

			CPPUNIT_ASSERT_EQUAL( int(22050),
					      int(header.GetSampleRate()) );
			CPPUNIT_ASSERT_EQUAL( 1,
					      header.GetChannels() );
			CPPUNIT_ASSERT_EQUAL( int(6072.02),
					      int(header.GetLength()) );
			CPPUNIT_ASSERT_EQUAL( std::string("WAV"),
					      header.GetFormat().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string("signed 16-bit"),
					      header.GetEncoding().GetString() );
			
			CPPUNIT_ASSERT_EQUAL( std::string("Format Default"),
					      header.GetEndianess().GetString() );
					      
					      
		}

		void testGetHeader_HeaderIsRight_OggVorbis()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "Elvis.ogg" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( int(22050.),
					      int(header.GetSampleRate()) );
			CPPUNIT_ASSERT_EQUAL( 1,
					      header.GetChannels() );
			CPPUNIT_ASSERT_EQUAL( int(5427),
					      int(header.GetLength()) );
			CPPUNIT_ASSERT_EQUAL( std::string( "VorbisMk1" ),
					      header.GetFormat().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string( "Format Default" ),
					      header.GetEncoding().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string( "Format Default" ),
					      header.GetEndianess().GetString() );

		}

		void testGetHeader_NoHeaderWhenFileIsUnreadable()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( "momonga" );
			
			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( false,
					      header.HasSampleRate() );
		}

		void testGetHeader_NoHeaderWhenFileIsUnreadable_AfterOneSuccessful()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "Elvis.wav" ) );
	
			file.OpenExisting( "Momonga" );
			
			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( false,
					      header.HasSampleRate() );
		}

		void testCreateNew_UserDefinesHeaderForWriting_PCM_RightCodecIsDeduced()
		{
			CLAM::AudioFileTarget file;
			
			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddAll();
			fileHeader.UpdateData();
			fileHeader.SetSampleRate(44100.);
			fileHeader.SetChannels( 2 );
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "WAV" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "signed 24-bit" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );

			file.CreateNew( "newFile.wav", fileHeader );		

			CPPUNIT_ASSERT_EQUAL( std::string( "PCM" ),
					      file.GetKind().GetString() );
		}

		void testCreateNew_UserDefinesHeaderForWriting_Ogg_RightCodecIsDeduced()
		{
			CLAM::AudioFileTarget file;
			

			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddAll();
			fileHeader.UpdateData();
			fileHeader.SetSampleRate(44100.);
			fileHeader.SetChannels( 2 );
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "VorbisMk1" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "Format Default" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );

			file.CreateNew( "newFile.wav", fileHeader );		

			CPPUNIT_ASSERT_EQUAL( std::string( "Ogg/Vorbis" ),
					      file.GetKind().GetString() );

		}

		void testCreateNew_WithoutSampleRate_Fails()
		{
			CLAM::AudioFileTarget file;
			

			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddChannels();
			fileHeader.AddFormat();
			fileHeader.AddEncoding();
			fileHeader.AddEndianess();
			fileHeader.UpdateData();
			fileHeader.SetChannels( 2 );
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "VorbisMk1" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "Format Default" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );


			CPPUNIT_ASSERT_EQUAL( false, file.CreateNew( std::string( "newFile.wav" ), fileHeader ));

		}

		void testCreateNew_WithoutChannels_IsOK()
		{
			CLAM::AudioFileTarget file;
			
			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddSampleRate();
			fileHeader.AddFormat();
			fileHeader.AddEncoding();
			fileHeader.AddEndianess();
			fileHeader.UpdateData();
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "VorbisMk1" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "Format Default" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );

			CPPUNIT_ASSERT_EQUAL( true, file.CreateNew("newFile.wav", fileHeader) );

		}

		void testCreateNew_WithoutFormat_Fails()
		{
			CLAM::AudioFileTarget file;
			
			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddSampleRate();
			fileHeader.AddChannels();
			fileHeader.AddEncoding();
			fileHeader.AddEndianess();
			fileHeader.UpdateData();

			CPPUNIT_ASSERT_EQUAL( false, file.CreateNew("newFile.wav", fileHeader) );

		}

		void testCreateNew_SetValues_WAV()
		{
			CLAM::AudioFileTarget file;

			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "WAV" );

			CPPUNIT_ASSERT_EQUAL( true, file.CreateNew("newFile.wav", fileHeader) );
		}

		void testCreateNew_SetValues_AIFF()
		{
			CLAM::AudioFileTarget file;

			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "AIFF" );

			CPPUNIT_ASSERT_EQUAL( true, file.CreateNew("newFile.wav", fileHeader) );
		}

		void testCreateNew_SetValues_OggVorbis()
		{
			CLAM::AudioFileTarget file;

			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "VorbisMk1" );

			CPPUNIT_ASSERT_EQUAL( true, file.CreateNew("newFile.wav", fileHeader) );
		}

		void testCreateNew_SetValues_WAV_AreWritable()
		{
			CLAM::AudioFileTarget file;

			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "WAV" );

			file.CreateNew( "newFile.wav", fileHeader );
			
			CPPUNIT_ASSERT_EQUAL( true, file.IsWritable() );
		}


		void testCreateNew_SetValues_AIFF_AreWritable()
		{
			CLAM::AudioFileTarget file;

			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "AIFF" );

			file.CreateNew( "newFile.aiff", fileHeader );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.IsWritable() );
		}

		void testCreateNew_SetValues_OggVorbis_AreWritable()
		{
			CLAM::AudioFileTarget file;

			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "VorbisMk1" );

			file.CreateNew( "newFile.ogg", fileHeader );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.IsWritable() );
		}

		void testOpenExisting_FileExists_and_Is_Mpeg()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "trumpet.mp3" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("Mpeg Audio"),
					      file.GetKind().GetString() );			

		}


		void testOpenExisting_FileExists_NotSpurious_Mpeg()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "ElvisStereo.wav" ) );

			CPPUNIT_ASSERT( std::string("Mpeg Audio") != file.GetKind().GetString() );						
		}

		void testGetHeader_HeaderIsRight_Mpeg()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "trumpet.mp3" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( int(22050),
					      (int)header.GetSampleRate() );
			CPPUNIT_ASSERT_EQUAL( int(1),
					      (int)header.GetChannels() );
			CPPUNIT_ASSERT_EQUAL( int(2430),
					      (int)header.GetLength() );
			CPPUNIT_ASSERT_EQUAL( std::string("Mpeg Audio Layer 3"),
					      header.GetFormat().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string("Format Default"),
					      header.GetEncoding().GetString() );
			CPPUNIT_ASSERT_EQUAL( std::string("Format Default"),
					      header.GetEndianess().GetString() );

		}

		void testTextDescriptorsExtraction_From_OggVorbis()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + std::string( "Elvis.ogg" ) );

			const CLAM::AudioTextDescriptors& txtDesc = file.GetTextDescriptors();
			
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("Elvis Presley"),
					      txtDesc.GetArtist() );
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("My Love Complete"),
					      txtDesc.GetTitle() );
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("Unknown"),
					      txtDesc.GetAlbum() );
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("1"),
					      txtDesc.GetTrackNumber() );
		}
		
		void testTextDescriptorsExtraction_From_Mpeg()
		{
			CLAM::AudioFileSource file;
			file.OpenExisting( mPathToTestData + CLAM::Text( "trumpet.mp3" ) );

			const CLAM::AudioTextDescriptors& txtDesc = file.GetTextDescriptors();
			
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("The Trumpeteers"),
					      txtDesc.GetArtist() );
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("A Trumpet"),
					      txtDesc.GetTitle() );
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("Trumpets, Trumpets, Trumpets"),
					      txtDesc.GetAlbum() );
			CPPUNIT_ASSERT_EQUAL( CLAM::Text("1"),
					      txtDesc.GetTrackNumber() );
		}

	};
	
}

