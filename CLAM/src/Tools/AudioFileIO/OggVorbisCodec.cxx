#include <cstdio>
#include <string>
#include <algorithm>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include "AudioFileFormats.hxx"
#include "AudioFile.hxx"
#include "OggVorbisCodec.hxx"
#include "OggVorbisAudioStream.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	OggVorbisCodec::OggVorbisCodec()
	{
	}

	OggVorbisCodec::~OggVorbisCodec()
	{
	}

	OggVorbisCodec& OggVorbisCodec::Instantiate()
	{
		static OggVorbisCodec theInstance;

		return theInstance;
	}

	bool OggVorbisCodec::IsReadable( std::string uri ) const
	{
		FILE*          fileHandle;
		OggVorbis_File vorbisFile;

		memset( &vorbisFile, 0, sizeof(OggVorbis_File) );

		fileHandle = fopen( uri.c_str(), "rb" );
		
		if (  !fileHandle || ferror(fileHandle) != 0  )
		{
			//:TODO: possible exception throwing
			//std::cerr << uri << std::endl;
			//std::cerr << "Open failed! " << strerror( errno ) << std::endl;
			return false;
		}

		int retval = ov_open( fileHandle, &vorbisFile, NULL, 0 );

		if (  retval < 0 )		
		{
			fclose( fileHandle );
			
			return false;
		}

		// MRJ: No need to close the fileHandle since libvorbisfile takes
		// its ownership if the ov_open call is successful
		ov_clear( &vorbisFile );

		return true;

	}

	bool OggVorbisCodec::IsWritable( std::string uri, const AudioFileHeader& hdr ) const
	{
		// MRJ: These values were documented ( sort of ) in
		// the Vorbis I specification document

		if ( (hdr.GetChannels() < 0) || ( hdr.GetChannels() > 255) )
			return false;
		if ( (hdr.GetSampleRate() < 8000.) || ( hdr.GetSampleRate() > 192000.) )
			return false;
		
		return true;	
	}

	Stream* OggVorbisCodec::GetStreamFor( const AudioFile& file )
	{
		return new OggVorbisAudioStream(file);
	}

	void OggVorbisCodec::RetrieveHeaderData( std::string uri, AudioFileHeader& hdr )
	{
		FILE*          fileHandle;
		OggVorbis_File vorbisFile;
		
		if ( ( fileHandle = fopen( uri.c_str(), "rb" ) ) == NULL )
		     return;

		if ( ov_open( fileHandle, &vorbisFile, NULL, 0 ) < 0 )		
		{
			fclose( fileHandle );			
			return;
		}

		vorbis_info* fileInfo = ov_info( &vorbisFile, -1 );
		
		if ( !fileInfo ) // File was encoded improperly
			return;

		hdr.AddSampleRate();
		hdr.AddChannels();
		hdr.AddFormat();
		hdr.AddEncoding();
		hdr.AddEndianess();
		hdr.AddLength();
		hdr.UpdateData();

		hdr.SetSampleRate( (TData)fileInfo->rate );
		hdr.SetChannels( (TSize)fileInfo->channels );
		hdr.SetLength( (TTime)ov_time_total( &vorbisFile, -1) * 1000. );
		hdr.SetFormat( EAudioFileFormat::eVorbisMk1 );
		hdr.SetEncoding( EAudioFileEncoding::eDefault );
		hdr.SetEndianess( EAudioFileEndianess::eDefault );

		// MRJ: No need to close the fileHandle since libvorbisfile takes
		// its ownership if the ov_open call is successful
		ov_clear( &vorbisFile );
	}

	void    OggVorbisCodec::RetrieveTextDescriptors( std::string uri, AudioTextDescriptors&  txtDesc )
	{
		FILE*          fileHandle;
		OggVorbis_File vorbisFile;
		
		if ( ( fileHandle = fopen( uri.c_str(), "rb" ) ) == NULL )
		     return;

		if ( ov_open( fileHandle, &vorbisFile, NULL, 0 ) < 0 )		
		{
			fclose( fileHandle );			
			return;
		}

		vorbis_info* fileInfo = ov_info( &vorbisFile, -1 );
		
		if ( !fileInfo ) // File was encoded improperly
			return;


		vorbis_comment* fileComments = ov_comment( &vorbisFile, -1 );

		if ( !fileComments ) // there were no comments in the file!
		{
			return;
		}
		
		int nComments = fileComments->comments;
		char** commentVector = fileComments->user_comments;
		int*   commentLenVector = fileComments->comment_lengths;

		for ( int i = 0; i < nComments; i++ )
		{
			// convert the current comment string into a std::string
			std::string currentComment;
			currentComment.assign( commentVector[i], 
					       commentVector[i]+commentLenVector[i] );
			
			std::string::iterator eqPos = std::find( currentComment.begin(),
								 currentComment.end(), '=' );

			if ( eqPos < currentComment.end() )
			{
				std::string fieldName;
				fieldName.assign( currentComment.begin(), eqPos );

				if( fieldName == "ARTIST" )
				{
					txtDesc.AddArtist();
					txtDesc.UpdateData();
					txtDesc.GetArtist().assign( eqPos+1, currentComment.end() );
				}
				else if ( fieldName == "TITLE" )
				{
					txtDesc.AddTitle();
					txtDesc.UpdateData();
					txtDesc.GetTitle().assign( eqPos+1, currentComment.end() );
				}
				else if ( fieldName == "ALBUM" )
				{
					txtDesc.AddAlbum();
					txtDesc.UpdateData();
					txtDesc.GetAlbum().assign( eqPos+1, currentComment.end() );
				}
				else if ( fieldName == "TRACKNUMBER" )
				{
					txtDesc.AddTrackNumber();
					txtDesc.UpdateData();
					txtDesc.GetTrackNumber().assign( eqPos+1, currentComment.end() );
				}
				else if ( fieldName == "PERFORMER" )
				{
					txtDesc.AddPerformer();
					txtDesc.UpdateData();
					txtDesc.GetPerformer().assign( eqPos+1, currentComment.end() );
				}
				else if ( fieldName == "COMPOSER" )
				{
					txtDesc.AddComposer();
					txtDesc.UpdateData();
					txtDesc.GetComposer().assign( eqPos+1, currentComment.end() );
				}
				else
				{
					std::string msg = fieldName;
					msg+= ":  Ignored comment field!";
					//CLAM_WARNING( false, msg.c_str() );
				}
			}
			
		}

		ov_clear( &vorbisFile );
	}
}

}
