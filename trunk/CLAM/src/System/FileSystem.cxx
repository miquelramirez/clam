#include "FileSystem.hxx"
#include "Lock.hxx"

namespace CLAM
{
	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{
	}

	FileSystem& FileSystem::GetInstance()
	{

		static FileSystem theInstance;

		return theInstance;
	}

	void FileSystem::UnlockFile( std::string filename )
	{
		Mutex::ScopedLock lock( mFileLockingSemaphor );
		
		mLockedFiles[ filename ] = false;
	}

	void FileSystem::LockFile( std::string filename )
	{
		Mutex::ScopedLock lock( mFileLockingSemaphor );

		mLockedFiles[ filename ] = true;
	}

	bool FileSystem::IsFileLocked( std::string filename )
	{
		Mutex::ScopedLock lock( mFileLockingSemaphor );

		FileStatusTable::iterator pos = mLockedFiles.find( filename );

		return pos != mLockedFiles.end() && pos->second==true;
	}
}
