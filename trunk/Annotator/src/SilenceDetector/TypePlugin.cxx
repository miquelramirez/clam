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


#include "TypePlugin.hxx"
#include <CLAM/Pool.hxx>
#include "SchemaAttribute.hxx"
#include <CLAM/Array.hxx>
#include <algorithm>
#include "Enumerated.hxx"
#include "FrameDivision.hxx"

namespace CLAM_Annotator
{

TypePlugin::TypePlugin(const SchemaAttribute & scheme)
	: mSchema(scheme)
{
}

TypePlugin::~TypePlugin()
{
}

class FloatTypePlugin : public TypePlugin
{
public:
	FloatTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<CLAM::TData>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM::TData * values =
			dataPool.GetReadPool<CLAM::TData>(
					mSchema.GetScope(),
					mSchema.GetName());
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			if (mSchema.HasMinValue() && values[i]<mSchema.GetMinValue()) 
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is under minimum specified value " 
					<< mSchema.GetMinValue() 
					<< std::endl;
				return false;
			}
			if (mSchema.HasMaxValue() && values[i]>mSchema.GetMaxValue())
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is over maximum specified value " 
					<< mSchema.GetMaxValue() 
					<< std::endl;
				return false;
			}
		}
		return true;
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		pool.GetWritePool<CLAM::TData>(
				mSchema.GetScope(),
				mSchema.GetName())[instance]=0.0;
	}
};

class IntTypePlugin : public TypePlugin
{
public:
	IntTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<int>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream& err)
	{
		const int * values =
			dataPool.GetReadPool<int>(
					mSchema.GetScope(),
					mSchema.GetName());
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			if (mSchema.HasMinValue() && values[i]<mSchema.GetMinValue())
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is under minimum specified value " 
					<< mSchema.GetMinValue() 
					<< std::endl;
				return false;
			}
			if (mSchema.HasMaxValue() && values[i]>mSchema.GetMaxValue())
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is over maximum specified value " 
					<< mSchema.GetMaxValue() 
					<< std::endl;
				return false;
			}
		}
		return true;
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		pool.GetWritePool<int>(
				mSchema.GetScope(),
				mSchema.GetName())[instance]=0;
	}
};

class EnumTypePlugin : public TypePlugin
{
public:
	EnumTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<Enumerated>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const Enumerated * values =
			dataPool.GetReadPool<Enumerated>(
					mSchema.GetScope(),
					mSchema.GetName());
		const std::list<std::string> & availableValues = mSchema.GetEnumerationValues();
		bool ok = true;
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			const std::string & value = values[i].GetString(); 
			if (std::find(availableValues.begin(),availableValues.end(),value)!=availableValues.end()) continue;

			err << "Enumeration value '" << value << "' at position " << i << " is not valid." << std::endl;
			// TODO: Tell which ones are available
			ok = false;
		}
		if (ok) return true;
		err << "Valid values for the attribute are:";
		for (std::list<std::string>::const_iterator it = availableValues.begin(); it!= availableValues.end(); it++)
			err << " '" << *it << "'";
		err << std::endl;
		return false;
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		const std::string & firstValue = mSchema.GetEnumerationValues().front();
		pool.GetWritePool<Enumerated>(
				mSchema.GetScope(),
				mSchema.GetName())[instance]=firstValue;
	}
};

class StringTypePlugin : public TypePlugin
{
public:
	StringTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<CLAM::Text>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM::Text * values =
			dataPool.GetReadPool<CLAM::Text>(
					mSchema.GetScope(),
					mSchema.GetName());
		values[0];
		return true;
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		pool.GetWritePool<CLAM::Text>(
				mSchema.GetScope(),
				mSchema.GetName())[instance]="";
	}
};

class SegmentationTypePlugin : public TypePlugin
{
public:
	SegmentationTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<CLAM::DataArray>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM::DataArray * values =
			dataPool.GetReadPool<CLAM::DataArray>(
					mSchema.GetScope(),
					mSchema.GetName());

		unsigned nChilds = -1;
		if (mSchema.HasChildScope() && mSchema.GetChildScope()!="")
			nChilds = dataPool.GetNumberOfContexts(mSchema.GetChildScope());

		const SegmentationPolicy & policy = mSchema.GetSegmentationPolicy();

		if (policy==SegmentationPolicy::eUnsized)
			return ValidateUnsized(values[0], err, nChilds);
		if (policy==SegmentationPolicy::eContinuous)
			return ValidateContinuous(values[0], err, nChilds);
		if (policy==SegmentationPolicy::eDiscontinuous)
			return ValidateDiscontinuous(values[0], err, nChilds);
		if (policy==SegmentationPolicy::eOverlapping)
			return ValidateOverlapping(values[0], err, nChilds);
		err << "Unsuported Segmentation policy" << std::endl;
		return false;	
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		pool.GetWritePool<CLAM::DataArray>(
				mSchema.GetScope(),
				mSchema.GetName())[instance].Init();
	}
private:
	bool ValidateUnsized(const CLAM::DataArray & values, std::ostream & err, unsigned nChilds)
	{
		bool ok=true;
		if (nChilds!=unsigned(-1) && nChilds!=values.Size())
		{
			err << "The number of marks (" << values.Size()
				<< ") and the number of children " << nChilds << " doesn't match"
				<< std::endl;
			ok=false;
		}
		for (unsigned i=1; i<values.Size(); i++)
		{
			if (values[i]>=values[i-1]) continue;
			err << "Mark " << i << " at " << values[i]
				<< " is placed before previous mark at " << values[i-1]
				<< std::endl;
			ok=false;
		}
		return ok;
	}
	bool ValidateContinuous(const CLAM::DataArray & values, std::ostream & err, unsigned nChilds)
	{
		bool ok=true;
		// TODO: Continuous segmentation should be nChilds-1?
		if (nChilds!=unsigned(-1) && nChilds!=values.Size()+1)
		{
			err << "The number of segments (" << values.Size()+1
				<< ") and the number of children " << nChilds << " doesn't match"
				<< std::endl;
			ok=false;
		}
		for (unsigned i=1; i<values.Size(); i++)
		{
			if (values[i]>=values[i-1]) continue;
			err << "Segment " << i << " starts at " << values[i]
				<< " which happens before previous start at " << values[i-1]
				<< std::endl;
			ok=false;
		}
		return ok;
	}
	bool ValidateDiscontinuous(const CLAM::DataArray & values, std::ostream & err, unsigned nChilds)
	{
		bool ok=true;
		if (values.Size() & 1)
		{
			err << "Even number of segmentation points" << std::endl;
			ok=false;
		}
		if (nChilds!=unsigned(-1) && nChilds*2!=values.Size())
		{
			err << "The number of segments (" << (values.Size()>>1)
				<< ") and the number of children " << nChilds << " doesn't match"
				<< std::endl;
			ok=false;
		}
		for (unsigned i = 1; i<values.Size(); i++)
		{
			if (values[i]>=values[i-1]) continue;
			ok=false;
			if (i & 1) // Offset
			{
				err << "The end of the segment " << (i>>1)
					<< " is placed at " << values[i]
					<< ", before its begining at " << values[i-1]
					<< std::endl;
			}
			else // Onset
			{
				err << "The segment " << (i>>1)
					<< " is overlapping the previous segment " << values[i]
					<< ", before its begining at " << values[i-1]
					<< std::endl;
			}
		}
		return ok;
	}
	bool ValidateOverlapping(const CLAM::DataArray & values, std::ostream & err, unsigned nChilds)
	{
		bool ok=true;
		if (values.Size() & 1)
		{
			err << "Even number of segmentation points" << std::endl;
			ok=false;
		}
		if (nChilds!=unsigned(-1) && nChilds*2!=values.Size())
		{
			err << "The number of segments (" << (values.Size()>>1)
				<< ") and the number of children " << nChilds << " doesn't match"
				<< std::endl;
			ok=false;
		}
		for (unsigned i = 1; i<values.Size(); i++)
		{
			if (values[i]>=values[i-1]) continue;
			if (i & 1) // Offset
			{
				err << "The end of the segment " << (i>>1)
					<< " is placed at " << values[i]
					<< ", before its begining at " << values[i-1]
					<< std::endl;
			}
			else // Onset
			{
				if (i<2) continue;
				if (values[i]>=values[i-2]) continue;
				err << "The segment " << (i>>1)
					<< " is overlapping the previous segment " << values[i]
					<< ", before its begining at " << values[i-1]
					<< std::endl;
			}
			ok=false;
		}
		return ok;
	}
	bool ValidateBounds(const CLAM::DataArray & values, std::ostream & err, CLAM::TData duration)
	{
		bool ok = true;
		for (unsigned i = 1; i<values.Size(); i++)
		{
			if (values[i]>=0 && values[i]<=duration) continue;
			err << "Segmentation point at " << values[i] << " is out of bounds" << std::endl;
			ok=false;
		}
		return ok;
	}
};

class FrameDivisionTypePlugin : public TypePlugin
{
public:
	FrameDivisionTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<CLAM_Annotator::FrameDivision>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM_Annotator::FrameDivision * values =
			dataPool.GetReadPool<CLAM_Annotator::FrameDivision>(
					mSchema.GetScope(),
					mSchema.GetName());
		return true;
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		CLAM_Annotator::FrameDivision & division = 
			pool.GetWritePool<CLAM_Annotator::FrameDivision>(
				mSchema.GetScope(),
				mSchema.GetName())[instance];
	}
};

class FloatArrayTypePlugin : public TypePlugin
{
public:
	FloatArrayTypePlugin(const SchemaAttribute & scheme)
		: TypePlugin(scheme)
	{
	}

	void AddTo(CLAM::DescriptionScheme & scheme)
	{
		scheme.AddAttribute<CLAM::DataArray >(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM::DataArray  * values =
			dataPool.GetReadPool<CLAM::DataArray >(
					mSchema.GetScope(),
					mSchema.GetName());
		return true;
	}
	void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)
	{
		CLAM::DataArray  & array = 
			pool.GetWritePool<CLAM::DataArray >(
				mSchema.GetScope(),
				mSchema.GetName())[instance];
		unsigned nBins = mSchema.HasNBins() ? mSchema.GetNBins()
			: (mSchema.HasBinLabels() ? mSchema.GetBinLabels().size() 
			: 0 );
		array.Resize(nBins);
		array.SetSize(nBins);
		for (unsigned i = 0; i < nBins; i++)
			array[i]=0;
		// TODO: Init also the values
	}
};


TypePlugin * TypePlugin::Create(const SchemaAttribute & scheme)
{
	const std::string & type = scheme.GetType();
	if (type=="Enumerated")
		return new EnumTypePlugin(scheme);
	if (type=="Float")
		return new FloatTypePlugin(scheme);
	if (type=="Int")
		return new IntTypePlugin(scheme);
	if (type=="String")
		return new StringTypePlugin(scheme);
	if (type=="Segmentation")
		return new SegmentationTypePlugin(scheme);
	if (type=="FrameDivision")
		return new FrameDivisionTypePlugin(scheme);
	if (type=="FloatArray")
		return new FloatArrayTypePlugin(scheme);
	return 0;
}

}

