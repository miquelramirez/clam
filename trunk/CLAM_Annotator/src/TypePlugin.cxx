#include "TypePlugin.hxx"
#include "Pool.hxx"
#include "SchemaAttribute.hxx"
#include "RestrictedString.hxx"
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
		if (!mSchema.HasfRange()) return true;
		if (values[0]<mSchema.GetfRange().GetMin()) return false;
		if (values[0]>mSchema.GetfRange().GetMax()) return false;
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
		if (values[0]<mSchema.GetiRange().GetMin()) return false;
		if (values[0]>mSchema.GetiRange().GetMax()) return false;
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
		scheme.AddAttribute<RestrictedString>(mSchema.GetScope(),mSchema.GetName());
	}

	bool ValidateData(const CLAM::DescriptionDataPool & dataPool)
	{
		const RestrictedString * values =
			dataPool.GetReadPool<RestrictedString>(
					mSchema.GetScope(),
					mSchema.GetName());
		const std::list<std::string> & availableValues = mSchema.GetRestrictionValues();
		return std::find(availableValues.begin(),
			availableValues.end(),values[0].GetString())!=
			availableValues.end();
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
		return true;
	}
};

TypePlugin * TypePlugin::Create(const SchemaAttribute & scheme)
{
	const std::string & type = scheme.GetType();
	if (type=="RestrictedString")
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

