#include "SMS_Errors.hxx"

namespace SMS
{
	Error::Error()
	{
	}

	Error::Error( const Error& obj )
	{
		mMessageString = obj.mMessageString;
	}

	Error::~Error()
	{
	}

	std::string& Error::GetMessageString()
	{
		return mMessageString;
	}

	void Error::SetMessageString( std::string msgString )
	{
		mMessageString = msgString;
	}

	const std::string& Error::WhatHappened()
	{
		return mMessageString;
	}
	
	FileError::FileError()
	{
		std::string unspecified = "A not specified file ";
		SetOffendingFilename( unspecified );
	}

	void FileError::SetOffendingFilename( const char* filename )
	{
		std::string tmp( filename );
		
		SetOffendingFilename( tmp );
	}

	void FileError::SetOffendingFilename( const std::string& filename )
	{
		mOffendingFilename = filename;
		
		ComposeGenericErrorMessage();
	}

        const std::string& FileError::GetOffendingFilename() const
	{
		return mOffendingFilename;
	}

	void FileError::ComposeGenericErrorMessage()
	{
		SetMessageString( GetOffendingFilename() + " has caused an error!\n" );
	}

	void UnavailableFile::ComposeGenericErrorMessage()
	{
		SetMessageString( GetOffendingFilename()
				  + " does not exist or is unavailable to the application process" );
	}

	void UnsupportedFileFormat::ComposeGenericErrorMessage()
	{
		SetMessageString( GetOffendingFilename()
				  + " is encoded in an unsupported format. We currently only support\n"
				  + " WAVE/RIFF and AIFF audio files.");
	}

	void NotMono::ComposeGenericErrorMessage()
	{
		SetMessageString( GetOffendingFilename()
				  + " is not a mono audio file "  );
	}

	void UnsupportedAudioSampleFormat::ComposeGenericErrorMessage()
	{
		SetMessageString( GetOffendingFilename()
				  + " samples are encoded in an unsupported format.\n"
				  + " currently we only support PCM samples in signed 8, 16, 24 or 32 bit-wide numbers." );
	}

}
