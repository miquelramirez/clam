/*
* Copyright (c) 2013 David Garcia Garzón
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

#ifndef ClamTest_FileCleaner_hxx
#define ClamTest_FileCleaner_hxx


#include <string>
#include <list>
#include <cstdio>

namespace CLAMTest { class FileCleaner; }

class CLAMTest::FileCleaner
{
	typedef std::list<std::string> FileNames;
	FileNames _toRemove;
public:
	std::string add(const std::string & filename)
	{
		_toRemove.push_back(filename);
		return filename;
	}
	std::string addTemp()
	{
		std::string filename(std::tmpnam(0));
		add(filename);
		return filename;
	}
	~FileCleaner()
	{
		for (FileNames::iterator it = _toRemove.begin();
				it != _toRemove.end(); it++)
			std::remove(it->c_str());
	}
};


#endif//ClamTest_FileCleaner_hxx

