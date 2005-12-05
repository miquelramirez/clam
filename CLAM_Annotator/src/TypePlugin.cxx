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

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool)
	{
		const CLAM::TData * values =
			dataPool.GetReadPool<CLAM::TData>(
					mSchema.GetScope(),
					mSchema.GetName());
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			if (!mSchema.HasfRange()) continue;
			if (values[i]<mSchema.GetfRange().GetMin()) return false;
			if (values[i]>mSchema.GetfRange().GetMax()) return false;
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

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool)
	{
		const int * values =
			dataPool.GetReadPool<int>(
					mSchema.GetScope(),
					mSchema.GetName());
		if (!mSchema.HasiRange()) return true;
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			if (values[0]<mSchema.GetiRange().GetMin()) return false;
			if (values[0]>mSchema.GetiRange().GetMax()) return false;
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

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool)
	{
		const Enumerated * values =
			dataPool.GetReadPool<Enumerated>(
					mSchema.GetScope(),
					mSchema.GetName());
		const std::list<std::string> & availableValues = mSchema.GetEnumerationValues();
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
		{
			const std::string & value = values[i].GetString(); 
			if (std::find(availableValues.begin(),availableValues.end(),value)==availableValues.end())
				return false;
		}
		return true;
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

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool)
	{
		const CLAM::Text * values =
			dataPool.GetReadPool<CLAM::Text>(
					mSchema.GetScope(),
					mSchema.GetName());
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
			values[i];
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

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool)
	{
		const CLAM::IndexArray * values =
			dataPool.GetReadPool<CLAM::IndexArray>(
					mSchema.GetScope(),
					mSchema.GetName());
		for (unsigned i=0; i<dataPool.GetNumberOfContexts(mSchema.GetScope());i++)
			values[i];
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
	return 0;
}

}

