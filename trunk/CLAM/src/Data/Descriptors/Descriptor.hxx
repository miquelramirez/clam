/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


/**
* Abstract class for all descriptors
* @param Whether to calculate the descriptors over absolute values or not (signed).
* @todo This class could abstract more common behaviour from subclasses by becoming template
*/
template <bool abs>
class DescriptorTmpl:public ProcessingData
{
public:
	DescriptorTmpl(int n):ProcessingData(n){mpStats=0;}
	DescriptorTmpl(const DescriptorTmpl<abs>& prototype, bool shareData=false, bool deep=true)
		: ProcessingData(prototype, shareData, deep)
	{
		mpStats=0;
	};
	virtual ~DescriptorTmpl()
	{
		if (mpStats)
		{
			delete mpStats;
			mpStats=0;
		}
	}
	virtual void Compute()
	{
		CLAM_ASSERT(mpStats,"Descriptor::Compute: Error no valid statistics. This may happen if you forget to set your data after having set the prototype");
		ConcreteCompute();
	}
	virtual void ConcreteCompute()=0;
	void SetPrototype(const DescriptorTmpl<abs>& proto)
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
		mpStats=new StatsTmpl<abs>(pData);
	}
protected:
	StatsTmpl<abs>* mpStats;

};

typedef DescriptorTmpl<false> Descriptor;
typedef DescriptorTmpl<true> DescriptorAbs;

};



#endif // Descriptor.hxx



