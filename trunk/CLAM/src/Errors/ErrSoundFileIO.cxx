#include "ErrSoundFileIO.hxx"

namespace CLAM
{
	ErrSoundFileIO::ErrSoundFileIO() throw()
		: ErrProcessingObj( )
	{
	}

	ErrSoundFileIO::ErrSoundFileIO( const char* msg ) throw ()
		: ErrProcessingObj( msg )
	{
	}

	ErrSoundFileIO::ErrSoundFileIO( const ErrSoundFileIO& obj ) throw ()
		: ErrProcessingObj( obj )
	{
	}

	ErrSoundFileIO::~ErrSoundFileIO() throw()
	{
	}

	UnavailableSoundFile::UnavailableSoundFile() throw()
		: ErrSoundFileIO()
	{
	}

	UnavailableSoundFile::UnavailableSoundFile( const char* msg ) throw ()
		: ErrSoundFileIO( msg )
	{
	}

	UnavailableSoundFile::UnavailableSoundFile( const UnavailableSoundFile& obj ) throw()
		: ErrSoundFileIO( obj )
	{
	}
	
	UnavailableSoundFile::~UnavailableSoundFile() throw()
	{
	}

	UnsupportedSoundFileSampleEncoding::UnsupportedSoundFileSampleEncoding() throw()
		: ErrSoundFileIO()
	{
	}

	UnsupportedSoundFileSampleEncoding::UnsupportedSoundFileSampleEncoding( const char* msg ) throw ()
		: ErrSoundFileIO( msg )
	{
	}

	UnsupportedSoundFileSampleEncoding::UnsupportedSoundFileSampleEncoding( const UnsupportedSoundFileSampleEncoding& obj ) throw()
		: ErrSoundFileIO( obj )
	{
	}
	
	UnsupportedSoundFileSampleEncoding::~UnsupportedSoundFileSampleEncoding() throw()
	{
	}


	UnsupportedSoundFileFormat::UnsupportedSoundFileFormat() throw()
		: ErrSoundFileIO()
	{
	}

	UnsupportedSoundFileFormat::UnsupportedSoundFileFormat( const char* msg ) throw ()
		: ErrSoundFileIO( msg )
	{
	}

	UnsupportedSoundFileFormat::UnsupportedSoundFileFormat( const UnsupportedSoundFileFormat& obj ) throw()
		: ErrSoundFileIO( obj )
	{
	}
	
	UnsupportedSoundFileFormat::~UnsupportedSoundFileFormat() throw()
	{
	}

		
}
