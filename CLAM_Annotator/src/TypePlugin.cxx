#include "TypePlugin.hxx"
#include "Pool.hxx"
#include "SchemaAttribute.hxx"
#include "Enumerated.hxx"
#include "IndexArray.hxx"


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
		if (!mSchema.HasfRange()) return true;
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			if (values[i]<mSchema.GetfRange().GetMin()) 
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is under minimum specified value " 
					<< mSchema.GetfRange().GetMin() 
					<< std::endl;
				return false;
			}
			if (values[i]>mSchema.GetfRange().GetMax())
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is over maximum specified value " 
					<< mSchema.GetfRange().GetMax() 
					<< std::endl;
				return false;
			}
		}
		return true;
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
		if (!mSchema.HasfRange()) return true;
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			if (values[i]<mSchema.GetiRange().GetMin()) 
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is under minimum specified value " 
					<< mSchema.GetiRange().GetMin() 
					<< std::endl;
				return false;
			}
			if (values[i]>mSchema.GetiRange().GetMax())
			{
				err 
					<< "Position " << i 
					<< " is has value " << values[i]
					<< " which is over maximum specified value " 
					<< mSchema.GetiRange().GetMax() 
					<< std::endl;
				return false;
			}
		}
		return true;
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
		scheme.AddAttribute<CLAM::IndexArray>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM::IndexArray * values =
			dataPool.GetReadPool<CLAM::IndexArray>(
					mSchema.GetScope(),
					mSchema.GetName());
		bool ok = true;
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			const SegmentationPolicy & policy = mSchema.GetSegmentationPolicy();

			bool offsetsStored =
				policy==SegmentationPolicy::eDiscontinuous ||
				policy==SegmentationPolicy::eOverlapping;
			
			if (offsetsStored)
			{
				bool mayOverlap = SegmentationPolicy::eOverlapping;
				ok = ok && CheckEvenPositions(values[i], i, err);
				ok = ok && CheckOrderForOnsetsAndOffsets(values[i], i, mayOverlap, err);
			}
			else
				ok = ok && CheckOrderForOnsets(values[i], i, err);
		}
		return ok;
	}
private:
	bool CheckEvenPositions(const CLAM::IndexArray& positions, unsigned i, std::ostream & err)
	{
		if (!(positions.Size() & 1)) return true;

		err << "The " << mSchema.GetSegmentationPolicy().GetString() 
			<< " segmentation policy requires both onsets and offset positions, "
			"but segmentation " << i << " has an odd number of positions." << std::endl;
		return false;
	}
	bool CheckOrderForOnsets(const CLAM::IndexArray& positions, unsigned i, std::ostream & err)
	{
		bool ok=true;
		for (unsigned j=1; j<positions.Size(); j++)
		{
			if (positions[j]>=positions[j-1]) continue;
			err << "On segmentation " << i << ", segment " 
				<< j << " is positioned at " << positions[j] << 
				", that's before previous segment that is at position " << positions[j-1] << "." << std::endl;
			ok = false;
		}
		return ok;
	}
	bool CheckOrderForOnsetsAndOffsets(const CLAM::IndexArray& positions, unsigned i, bool mayOverlap, std::ostream & err)
	{
		bool ok=true;
		for (unsigned j=1; j<positions.Size(); j+=1)
		{
			if (positions[j]>=positions[j-1]) continue; // They are ordered, no problem

			if (j&1) // j is an offset
			{
				err << "On segmentation " << i << ", segment " 
					<< j/2 << " has inverted beggining and endingss." << std::endl;
				return false;
			}
			if (positions[j]<positions[j-2])
			{
				err << "On segmentation " << i << ", segment " 
					<< (j>>1) << " is out of order." << std::endl;
				return false;
			}
			if (!mayOverlap) 
			{
				err << "On segmentation " << i << ", segment " 
					<< (j/2-1) << " begins before previous segment ends." << std::endl;
				return false;
			}
		}
		return true;
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
		scheme.AddAttribute<CLAM::IndexArray>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		const CLAM::IndexArray * values =
			dataPool.GetReadPool<CLAM::IndexArray>(
					mSchema.GetScope(),
					mSchema.GetName());
		return true;
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
	return 0;
}

}

