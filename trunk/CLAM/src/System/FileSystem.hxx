#ifndef __FILESYSTEM__
#define __FILESYSTEM__

#include <string>
#include <map>
#include "Mutex.hxx"

namespace CLAM
{
	class FileSystem
	{
	public:
		~FileSystem();
		
		static FileSystem& GetInstance();

		void UnlockFile( std::string filename );
		void LockFile( std::string filename );
		bool IsFileLocked( std::string filename );

	protected:

		FileSystem();

	protected:
		typedef std::map< std::string, bool  > FileStatusTable;

		FileStatusTable  mLockedFiles;
		Mutex            mFileLockingSemaphor;
	};

}

#endif // FileSystem.hxx
