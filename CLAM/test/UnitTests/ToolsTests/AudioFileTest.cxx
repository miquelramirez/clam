#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "AudioFile.hxx"
#include "AudioFileHeader.hxx"
#include "AudioFileFormats.hxx"

#include "XMLStorage.hxx"
#include <fstream>

namespace CLAMTest
{
	class AudioFileTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( AudioFileTest );

	class AudioFileTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( AudioFileTest );

		CPPUNIT_TEST( testSetLocation_FileExists_and_Is_PCM );
		CPPUNIT_TEST( testSetLocation_FileExists_and_Is_OggVorbis );
		CPPUNIT_TEST( testSetLocation_FileExists_and_Is_Mpeg );
		CPPUNIT_TEST( testSetLocation_FileExists_NotSpurious_Mpeg );
		
		CPPUNIT_TEST( testSetLocation_FileDoesNotExist_UnrecognizedFormat );

		CPPUNIT_TEST( testGetHeader_HeaderIsRight_PCM );
		CPPUNIT_TEST( testGetHeader_HeaderIsRight_OggVorbis );
		CPPUNIT_TEST( testGetHeader_HeaderIsRight_Mpeg );

		CPPUNIT_TEST( testGetHeader_NoHeaderWhenFileIsUnreadable );
		CPPUNIT_TEST( testGetHeader_NoHeaderWhenFileIsUnreadable_AfterOneSuccessful );

		CPPUNIT_TEST( testSetHeader_UserDefinesHeaderForWriting_PCM_RightCodecIsDeduced );
		CPPUNIT_TEST( testSetHeader_UserDefinesHeaderForWriting_Ogg_RightCodecIsDeduced );
		CPPUNIT_TEST( testSetHeader_WithoutSampleRate_Fails );
		CPPUNIT_TEST( testSetHeader_WithoutChannels_IsOK );
		CPPUNIT_TEST( testSetHeader_WithoutFormat_Fails );
	
		CPPUNIT_TEST( testSetHeader_SetValues_WAV );
		CPPUNIT_TEST( testSetHeader_SetValues_AIFF );
		CPPUNIT_TEST( testSetHeader_SetValues_OggVorbis );

		CPPUNIT_TEST( testSetHeader_SetValues_WAV_AreWritable );
		CPPUNIT_TEST( testSetHeader_SetValues_AIFF_AreWritable );
		CPPUNIT_TEST( testSetHeader_SetValues_OggVorbis_AreWritable );

		CPPUNIT_TEST( testIsWritable_ReturnsTrue_PCM_WithReasonableHeader );
		CPPUNIT_TEST( testIsWritable_ReturnsFalse_PCM_TooManyChannels );
		CPPUNIT_TEST( testIsWritable_ReturnsFalse_PCM_TooFewChannels );

		CPPUNIT_TEST( testTextDescriptorsExtraction_From_OggVorbis );
		CPPUNIT_TEST( testTextDescriptorsExtraction_From_Mpeg );

		CPPUNIT_TEST_SUITE_END();
	public:

		void setUp()
		{
			char* pathToTestData = getenv( "CLAM_TEST_DATA" );

			if ( !pathToTestData )
				mPathToTestData = "../../../../CLAM-TestData/";
			else
				mPathToTestData = pathToTestData;

		}

		void tearDown()
		{
		}
	protected:

		std::string mPathToTestData;

	private: // test cases


		void testIsWritable_ReturnsTrue_PCM_WithReasonableHeader()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "NewFile.wav" ) );

			CLAM::AudioFileHeader header;

			header.AddAll();
			header.UpdateData();
			header.SetChannels( 1 );
			header.SetSampleRate( 22050 );
			header.SetFormat( CLAM::EAudioFileFormat::eWAV );
			header.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			header.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			file.SetHeader( header );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.IsWritable() );
		}

		void testIsWritable_ReturnsFalse_PCM_TooManyChannels()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "NewFile.wav" ) );

			CLAM::AudioFileHeader header;

			header.AddAll();
			header.UpdateData();
			header.SetChannels( 291 );
			header.SetSampleRate( 22050 );
			header.SetFormat( CLAM::EAudioFileFormat::eWAV );
			header.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			header.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			file.SetHeader( header );

			CPPUNIT_ASSERT_EQUAL( false,
					      file.IsWritable() );		
		}

		void testIsWritable_ReturnsFalse_PCM_TooFewChannels()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "NewFile.wav" ) );
			
			CLAM::AudioFileHeader header;

			header.AddAll();
			header.UpdateData();
			header.SetChannels( 0 );
			header.SetSampleRate( 22050 );
			header.SetFormat( CLAM::EAudioFileFormat::eWAV );
			header.SetEncoding( CLAM::EAudioFileEncoding::ePCM_24 );
			header.SetEndianess( CLAM::EAudioFileEndianess::eDefault );

			file.SetHeader( header );
			
			CPPUNIT_ASSERT_EQUAL( false,
					      file.IsWritable() );		

		}


		void testSetLocation_FileExists_and_Is_PCM()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("PCM"),
					      file.GetKind().GetString() );
		}

		void testSetLocation_FileExists_and_Is_OggVorbis()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.ogg" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("Ogg/Vorbis"),
					      file.GetKind().GetString() );			
		}

		void testSetLocation_FileDoesNotExist_UnrecognizedFormat()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string("nikora") );

			CPPUNIT_ASSERT_EQUAL( std::string( "Unknown" ),
					      file.GetKind().GetString() );
		}

		void testGetHeader_HeaderIsRight_PCM()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

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
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.ogg" ) );

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
			CLAM::AudioFile file;
			file.SetLocation( "momonga" );
			
			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( false,
					      header.HasSampleRate() );
		}

		void testGetHeader_NoHeaderWhenFileIsUnreadable_AfterOneSuccessful()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
	
			file.SetLocation( "Momonga" );
			
			const CLAM::AudioFileHeader& header = file.GetHeader();

			CPPUNIT_ASSERT_EQUAL( false,
					      header.HasSampleRate() );
		}

		void testSetHeader_UserDefinesHeaderForWriting_PCM_RightCodecIsDeduced()
		{
			CLAM::AudioFile file;
			
			file.SetLocation( std::string( "newFile.wav" ) );

			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddAll();
			fileHeader.UpdateData();
			fileHeader.SetSampleRate(44100.);
			fileHeader.SetChannels( 2 );
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "WAV" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "signed 24-bit" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );

			file.SetHeader( fileHeader );		

			CPPUNIT_ASSERT_EQUAL( std::string( "PCM" ),
					      file.GetKind().GetString() );
		}

		void testSetHeader_UserDefinesHeaderForWriting_Ogg_RightCodecIsDeduced()
		{
			CLAM::AudioFile file;
			
			file.SetLocation( std::string( "newFile.wav" ) );

			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddAll();
			fileHeader.UpdateData();
			fileHeader.SetSampleRate(44100.);
			fileHeader.SetChannels( 2 );
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "VorbisMk1" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "Format Default" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );

			file.SetHeader( fileHeader );		

			CPPUNIT_ASSERT_EQUAL( std::string( "Ogg/Vorbis" ),
					      file.GetKind().GetString() );

		}

		void testSetHeader_WithoutSampleRate_Fails()
		{
			CLAM::AudioFile file;
			
			file.SetLocation( std::string( "newFile.wav" ) );

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


			CPPUNIT_ASSERT_EQUAL( false,
					      file.SetHeader(fileHeader) );

		}

		void testSetHeader_WithoutChannels_IsOK()
		{
			CLAM::AudioFile file;
			
			file.SetLocation( std::string( "newFile.wav" ) );

			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddSampleRate();
			fileHeader.AddFormat();
			fileHeader.AddEncoding();
			fileHeader.AddEndianess();
			fileHeader.UpdateData();
			fileHeader.SetFormat( CLAM::EAudioFileFormat( "VorbisMk1" ) );
			fileHeader.SetEncoding( CLAM::EAudioFileEncoding( "Format Default" ) );
			fileHeader.SetEndianess( CLAM::EAudioFileEndianess( "Format Default" ) );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.SetHeader(fileHeader) );

		}

		void testSetHeader_WithoutFormat_Fails()
		{
			CLAM::AudioFile file;
			
			file.SetLocation( std::string( "newFile.wav" ) );

			CLAM::AudioFileHeader fileHeader;

			fileHeader.AddSampleRate();
			fileHeader.AddChannels();
			fileHeader.AddEncoding();
			fileHeader.AddEndianess();
			fileHeader.UpdateData();

			CPPUNIT_ASSERT_EQUAL( false,
					      file.SetHeader(fileHeader) );

		}

		void testSetHeader_SetValues_WAV()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "newFile.wav" ) );
			
			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "WAV" );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.SetHeader(fileHeader) );
		}

		void testSetHeader_SetValues_AIFF()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "newFile.wav" ) );
			
			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "AIFF" );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.SetHeader(fileHeader) );
		}

		void testSetHeader_SetValues_OggVorbis()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "newFile.wav" ) );
			
			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "VorbisMk1" );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.SetHeader(fileHeader) );
		}

		void testSetHeader_SetValues_WAV_AreWritable()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "newFile.wav" ) );
			
			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "WAV" );

			file.SetHeader( fileHeader );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.IsWritable() );
		}


		void testSetHeader_SetValues_AIFF_AreWritable()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "newFile.aiff" ) );
			
			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "AIFF" );

			file.SetHeader( fileHeader );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.IsWritable() );
		}

		void testSetHeader_SetValues_OggVorbis_AreWritable()
		{
			CLAM::AudioFile file;

			file.SetLocation( std::string( "newFile.ogg" ) );
			
			CLAM::AudioFileHeader fileHeader;
			
			fileHeader.SetValues( 44100., 2, "VorbisMk1" );

			file.SetHeader( fileHeader );

			CPPUNIT_ASSERT_EQUAL( true,
					      file.IsWritable() );
		}

		void testSetLocation_FileExists_and_Is_Mpeg()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "trumpet.mp3" ) );

			CPPUNIT_ASSERT_EQUAL( std::string("Mpeg Audio"),
					      file.GetKind().GetString() );			

		}


		void testSetLocation_FileExists_NotSpurious_Mpeg()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "ElvisStereo.wav" ) );

			CPPUNIT_ASSERT( std::string("Mpeg Audio") != file.GetKind().GetString() );						
		}

		void testGetHeader_HeaderIsRight_Mpeg()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "trumpet.mp3" ) );

			const CLAM::AudioFileHeader& header = file.GetHeader();

			
			/*
			std::cout << std::endl;
			std::cout << header.GetSampleRate() << std::endl;
			std::cout << header.GetChannels() << std::endl;
			std::cout << header.GetLength() << std::endl;
			std::cout << header.GetFormat().GetString() << std::endl;
			std::cout << header.GetEncoding().GetString() << std::endl;
			std::cout << header.GetEndianess().GetString() << std::endl;
			std::cout << std::endl;
			*/


			CPPUNIT_ASSERT_EQUAL( int(22050),
					      (int)header.GetSampleRate() );
			CPPUNIT_ASSERT_EQUAL( int(1),
					      (int)header.GetChannels() );
			CPPUNIT_ASSERT_EQUAL( int(2298),
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
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.ogg" ) );

			const CLAM::AudioTextDescriptors& txtDesc = file.GetTextDescriptors();
			
			/**
			std::cout << std::endl;
			if ( txtDesc.HasArtist() )
				std::cout << "Artist: " << txtDesc.GetArtist() << std::endl;
			if ( txtDesc.HasTitle() )
				std::cout << "Title: " << txtDesc.GetTitle() << std::endl;
			if ( txtDesc.HasAlbum() )
				std::cout << "Album: " << txtDesc.GetAlbum() << std::endl;
			if ( txtDesc.HasTrackNumber() )
				std::cout << "Track Number: " << txtDesc.GetTrackNumber() << std::endl;
			if ( txtDesc.HasComposer() )
				std::cout << "Composer: " << txtDesc.GetComposer() << std::endl;
			if ( txtDesc.HasPerformer() )
				std::cout << "Performer: " << txtDesc.GetPerformer() << std::endl;
			std::cout << std::endl;
			*/

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
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + CLAM::Text( "trumpet.mp3" ) );

			std::ofstream outputFile( "AudioFile_0001.xml" );
			outputFile << "<?xml version=\"1.0\" ?>" << std::endl;

			CLAM::XMLStorage::Dump( file, "AudioFile", outputFile );

			outputFile.close();

			CLAM::AudioFile file2;

			CLAM::XMLStorage::Restore( file2, "AudioFile_0001.xml" );

			std::ofstream outputFile2( "CopyOf_AudioFile_0001.xml" );
			outputFile2 << "<?xml version=\"1.0\" ?>" << std::endl;
			
			CLAM::XMLStorage::Dump( file2, file2.GetClassName(), outputFile2 );
			outputFile2.close();
			
			const CLAM::AudioTextDescriptors& txtDesc = file.GetTextDescriptors();
			
			/*
			std::cout << std::endl;
			if ( txtDesc.HasArtist() )
				std::cout << "Artist: " << txtDesc.GetArtist() << std::endl;
			if ( txtDesc.HasTitle() )
				std::cout << "Title: " << txtDesc.GetTitle() << std::endl;
			if ( txtDesc.HasAlbum() )
				std::cout << "Album: " << txtDesc.GetAlbum() << std::endl;
			if ( txtDesc.HasTrackNumber() )
				std::cout << "Track Number: " << txtDesc.GetTrackNumber() << std::endl;
			if ( txtDesc.HasComposer() )
				std::cout << "Composer: " << txtDesc.GetComposer() << std::endl;
			if ( txtDesc.HasPerformer() )
				std::cout << "Performer: " << txtDesc.GetPerformer() << std::endl;
			std::cout << std::endl;
			*/
			
			

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
