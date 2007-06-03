/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef KeySpaceMonitor_hxx
#define KeySpaceMonitor_hxx

#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

class KeySpaceMonitor : public CLAM::PortMonitor<std::vector<CLAM::TData> >, public CLAM::VM::FloatArrayDataSource
{
public:
	KeySpaceMonitor()
		: _size(24)
	{
	}
private:
	const char* GetClassName() const { return "KeySpace"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::string a[] = {
			"G",
			"G#",
			"A",
			"A#",
			"B",
			"C",
			"C#",
			"D",
			"D#",
			"E",
			"F",
			"F#",
			"g",
			"g#",
			"a",
			"a#",
			"b",
			"c",
			"c#",
			"d",
			"d#",
			"e",
			"f",
			"f#",
			};
		return a[bin];
	}
	const CLAM::TData * frameData()
	{
		const std::vector<CLAM::TData> & pcp = FreezeAndGetData();
		_size = pcp.size()? pcp.size()-1: 0;
		return pcp.size()? &pcp[1] : 0;
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
	}
private:
	unsigned _size;
	std::vector<CLAM::TData> _pcp;
};


#endif

