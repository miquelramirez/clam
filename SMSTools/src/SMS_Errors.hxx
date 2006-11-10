#ifndef __SMS_ERRORS__
#define __SMS_ERRORS__

/**
 *  @file SMS_Errors.hxx
 *  This header file contains all declarations of Errors modeled
 *  as exceptions in the SMS Tools context
 */ 

#include <string>
#include <exception>

namespace SMS
{
	class Error
	{
	public:
		Error();
		Error( const Error& obj );		
		virtual ~Error();
		virtual const std::string& WhatHappened();
		void SetMessageString( std::string msgString );

	protected:
		std::string& GetMessageString();


	protected:	
		std::string mMessageString;
	};

	class FileError : public Error
	{
	public:
		FileError();
		
		void SetOffendingFilename( const std::string& filename );
		void SetOffendingFilename( const char* filename );
		const std::string& GetOffendingFilename() const;

	protected:
		virtual void ComposeGenericErrorMessage();

		std::string mOffendingFilename;
	};

	class UnavailableFile : public FileError
	{
	protected:
		virtual void ComposeGenericErrorMessage();
	};

	class UnsupportedFileFormat : public FileError
	{
	protected:
		virtual void ComposeGenericErrorMessage();
	};

	class NotMono : public FileError
	{
	protected:
		virtual void ComposeGenericErrorMessage();
	};

	class UnsupportedAudioSampleFormat : public FileError
	{	
	protected:
		virtual void ComposeGenericErrorMessage();
	};
}

#endif // __SMS_ERRORS__
