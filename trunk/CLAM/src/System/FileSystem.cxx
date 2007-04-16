/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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

