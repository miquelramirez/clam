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

#include "TraverseDirectory.hxx"
#include <sys/types.h>
#include <cstring>

TraverseDirectory::TraverseDirectory(void)
{

}
// Helper method for TraverseHelper
bool TraverseDirectory::IsCurrentOrParentDir(DirectoryEntry dirEntry) const
{
#ifndef WIN32
	return !strcmp(dirEntry->d_name,".") || !strcmp(dirEntry->d_name,"..");
#else
	return false;
#endif
}

std::string TraverseDirectory::CompleteName(const std::string& currentDirName, DirectoryEntry dirEntry) const
{
#ifndef WIN32
	bool noDirName = currentDirName == "";
	return noDirName? dirEntry->d_name : currentDirName+"/"+dirEntry->d_name;
#else
	return "";
#endif

}

void TraverseDirectory::TraverseHelper( Directory dir, const std::string& currentDirname,
	int curdepth, int maxdepth )
{
#ifndef WIN32
	dirent* dirEntry;
	while ((dirEntry = readdir(dir)))
	{
		if (IsCurrentOrParentDir(dirEntry))
			continue;

		std::string currentItemName = CompleteName(currentDirname, dirEntry);
		DIR* subdir = opendir(currentItemName.c_str());
		if (subdir)
		{
			OnDirectory(currentItemName); // 'template method'
			if (curdepth<maxdepth || maxdepth==-1)
			{
				TraverseHelper(subdir, currentItemName, curdepth+1, maxdepth);
			}
			closedir(subdir);
		}else
		{
			OnFile(currentItemName); // 'template method'
		}
	}
#else
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	std::string tmp;;
	if(currentDirname!="")
	{
		tmp+=currentDirname;
		tmp+="/";
	}
	tmp+="*.*";
	hFind = FindFirstFile(tmp.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE) return;
	
	do
	{
		std::string tmp2=currentDirname;
		tmp2+="/";
		tmp2+=fd.cFileName;
				
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{	
			if (strcmp(fd.cFileName,".") && strcmp(fd.cFileName,".."))
			{
				
				OnDirectory(tmp2);
				if (curdepth<maxdepth || maxdepth==-1)
				{
					TraverseHelper(fd, tmp2, curdepth+1, maxdepth);
				}
			}	
		}
		else
		{
			OnFile(tmp2);
		}
	} while (FindNextFile(hFind, &fd)); // enumerates contents
	FindClose(hFind);
#endif
}

void TraverseDirectory::Traverse(const std::string& rootname,int maxdepth)
{
#ifndef WIN32
	DIR* dir;

	dir = opendir(rootname == "" ? "." : rootname.c_str());

	if (dir)
	{
		OnDirectory(rootname);
		TraverseHelper(dir,rootname,0,maxdepth);
		closedir(dir);
	}
#else
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	std::string tmp = rootname;
	if ((tmp.rfind("/")!=tmp.length()-1)
		&&
		(tmp.rfind("\\")!=tmp.length()-1))
	{
		tmp += "\\";
	}
	tmp += "*.*";
	hFind = FindFirstFile(tmp.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE) return;
	if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{	
		OnDirectory(rootname);
		TraverseHelper(fd,rootname,0,maxdepth);
	}
	FindClose(hFind);;

#endif
}


//Auxiliary function to return the extension of a given filename
std::string TraverseDirectory::GetExtension(const std::string& filename)
{
   	return filename.substr(filename.rfind('.')+1);	
}

