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

#ifndef SegmentationViewMonitor_hxx
#define SegmentationViewMonitor_hxx

#include "SegmentationDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

#include <CLAM/DiscontinuousSegmentation.hxx>

class SegmentationViewMonitor :
	public CLAM::PortMonitor<CLAM::DiscontinuousSegmentation>,
	public CLAM::VM::SegmentationDataSource
{
public:
	SegmentationViewMonitor()
		: _currentTime(0)
	{
	}
private:
	const char* GetClassName() const { return "SegmentationView"; };
	std::string getLabel(unsigned bin) const
	{
		static std::string dummyLabel;
		return dummyLabel;
	}
	const CLAM::Segmentation & frameData()
	{
		const CLAM::Segmentation & data = FreezeAndGetData();
		_currentTime = data.maxPosition();
		return data;
	}
	void release()
	{
		UnfreezeData();
	}
	bool isEnabled() const
	{
		return IsRunning();
	}
	double currentTime()
	{
		return _currentTime;
	}
private:
	double _currentTime;
};


#endif// SegmentationViewMonitor_hxx
