#include "recursedir.h"
#include "strfuncs.h"

#ifdef _MSVC_VER

#include <windows.h>

#else

#include <sys/types.h>
#include <dirent.h>

#endif

#include <stdio.h>
#include <string.h>

#ifdef _MSVC_VER
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
	stadd("*.*");
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
				strstart(tmp2,2048);
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
