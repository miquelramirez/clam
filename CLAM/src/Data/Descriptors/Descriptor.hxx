/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#ifndef __Descriptor_H__
#define __Descriptor_H__

#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Stats.hxx"

namespace CLAM{

	
/** Abstract class for all descriptors */
//todo: this class could abstract more common behaviour from subclasses by becoming template
class Descriptor:public ProcessingData
{
public:
	Descriptor(int n):ProcessingData(n){mpStats=0;}
	Descriptor(const Descriptor& prototype, bool shareData=false, bool deep=true)
		: ProcessingData(prototype, shareData, deep)
	{
		mpStats=0;
	};
	virtual void Compute()
	{
		CLAM_ASSERT(mpStats,"Descriptor::Compute: Error no valid statistics. This may happen if you forget to set your data after having set the prototype");
		ConcreteCompute();
	}
	virtual void ConcreteCompute()=0;
	void SetPrototype(const Descriptor& proto)
	{
		*this=proto;
		if(mpStats)
		{
			delete mpStats;
			mpStats=0;
		}
	}
protected:
	void InitStats(DataArray* pData)
	{
		if(mpStats) delete mpStats;
		mpStats=new Stats(pData);
	}
protected:
	Stats* mpStats;
	
};

};

#endif // Descriptor.hxx

