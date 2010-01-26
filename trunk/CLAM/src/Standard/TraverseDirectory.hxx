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

#include <string>
#ifndef WIN32
#	include <dirent.h>
#else
#	include "CLAM_windows.h"
#endif

class TraverseDirectory
{

#ifdef WIN32
	typedef WIN32_FIND_DATA Directory;
	typedef HANDLE DirectoryEntry;
#else
	typedef DIR* Directory;
	typedef dirent* DirectoryEntry;
#endif

public:
	TraverseDirectory(void);
	virtual ~TraverseDirectory(void)
	{
	}
	void Traverse(const std::string& rootname = "",int maxdepth = -1);


protected:
	virtual void OnFile(const std::string& filename) { };
	virtual void OnDirectory(const std::string& dirname) { };

	/**
	* Called from OnDirectory, to indicate that the current directory
	* should not be recursed any further (but do traverse the files inside
	* that
	* directory)
	*/
	void SkipSubdirectories(void);

	/**
	* Called from OnDirectory, to indicate that the current directory
	* should not be recursed any further, including the files inside
	* that directory)
	*/
	void SkipDirectory(void);

	/** Helper method for client classes */
	std::string GetExtension(const std::string& filename);

private:
	void TraverseHelper(Directory dir,const std::string& dirname,int curdepth,int maxdepth);
	bool IsCurrentOrParentDir(DirectoryEntry dirEntry) const;
	std::string CompleteName(const std::string& currentDirName, DirectoryEntry dirEntry) const;
};

/*

#include "recursedir.h"
#include "strfuncs.h"

#ifdef WIN32

#include "CLAM_windows.h"

#else

#include <sys/types.h>
#include <dirent.h>

#endif

#include <stdio.h>
#include <string.h>

#ifdef WIN32
int _recursedir(
  const char* dir,int l,int m,on_file_func f,on_dir_func d,void* ptr)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	char tmp[2048];
	strstart(tmp,2048);
	if (strcmp(dir,"")!=0)
	{
		stradd(dir);
		stradd("/");
	}
	stradd("*.*");
	str_end();
	hFind = FindFirstFile(tmp, &fd);
	if (hFind == INVALID_HANDLE_VALUE) return -1;
	
	do
	{
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{	
			if (strcmp(fd.cFileName,".") && strcmp(fd.cFileName,".."))
			{
				char tmp2[2048];
				strstart(tmp2,2048);
				stradd(dir);
				stradd("/");
				stradd(fd.cFileName);
				str_end();

				if (d && d(tmp2,ptr)==1) {
					FindClose(hFind);
					return 1;
				}
				if (l<m || m==-1)
				{
					if (_recursedir(tmp2,l+1,m,f,d,ptr)==1)
					{
						FindClose(hFind);
						return 1;
					}
				}
			}	
		}
		else
		{
			if (f)
			{
				char tmp2[2048];
				strstart(tmp,2048);
				stradd(dir);
				stradd("/");
				stradd(fd.cFileName);
				str_end();
				if (f(tmp2,ptr)) {
					FindClose(hFind);
					return 1;
				}
			}
		}
	} while (FindNextFile(hFind, &fd)); // enumerates contents
	FindClose(hFind);

	return 0;
}

int recursedir(
  const char* dir,int m,on_file_func f,on_dir_func d,void* ptr)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	hFind = FindFirstFile(dir, &fd);
	if (hFind == INVALID_HANDLE_VALUE) return -1;
	if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{	
		if (d && d(dir,ptr)==1) {
			FindClose(hFind);
			return 1;
		}
		if (_recursedir(dir,0,m,f,d,ptr)==1)
		{
			FindClose(hFind);
			return 1;
		}
	}
	FindClose(hFind);
	return 0;
}

#else

int _recursedir(
	DIR* dir,const char* name,int l,int m,on_file_func f,on_dir_func d,void* ptr)
{
	struct dirent* e;

	while ((e = readdir(dir)))
	{
		if (strcmp(e->d_name,".") && strcmp(e->d_name,".."))
		{
			char tmp[2048];
			DIR* sd;
			strstart(tmp,2048);
			if (strcmp(name,""))
			{
				stradd(name);
				stradd("/");
			}
			stradd(e->d_name);
			str_end();
			sd = opendir(tmp);
			if (sd) {
				int ret = 0;
				if (d && d(tmp,ptr)) {
					closedir(sd);
					return 1;
				}
				if (l<m || m==-1)
				{
					ret = _recursedir(sd,tmp,l+1,m,f,d,ptr);
				}
				closedir(sd);
				if (ret==1) return 1;
			}else{
				if (f && f(tmp,ptr)) return 1;
			}
		}
	}
	return 0;
}

int recursedir(
  const char* name,int m,on_file_func f,on_dir_func d,void* ptr)
{
	DIR* dir;
	if (strcmp(name,"")) dir = opendir(name);
	else dir = opendir(".");
	if (dir)
	{
		int ret;
		if (d && d(name,ptr)) {
			closedir(dir);
			return 1;
		}
		ret = _recursedir(dir,name,0,m,f,d,ptr);
		closedir(dir);
		return ret;
	}else{
		return -1;
	}
	return 0;
}
#endif
*/

