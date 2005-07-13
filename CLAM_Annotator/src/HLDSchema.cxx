#include "HLDSchema.hxx"
#include <algorithm>
namespace CLAM_Annotator{

	/**************** HLDSchemaElement **************/

	void HLDSchemaElement::DefaultInit()
	{
		AddName();
		AddType();
		UpdateData();
	}

	bool HLDSchemaElement::Validate(const Descriptor<CLAM_Annotator::RestrictedString>& descriptor)const
	{
		bool validated = true;
		validated = (std::find(GetRestrictionValues().begin(),
			GetRestrictionValues().end(),descriptor.GetValue().GetString())!=
			GetRestrictionValues().end());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
		
	}
	
	bool HLDSchemaElement::Validate (const Descriptor<int>& descriptor)const
	{
		bool validated = true;
		validated = (descriptor.GetValue()>=GetiRange().GetMin()&&
			descriptor.GetValue()<=GetiRange().GetMax());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
	}
	
	bool HLDSchemaElement::Validate (const Descriptor<float>& descriptor)const
	{
		bool validated = true;
		validated = (descriptor.GetValue()>=GetfRange().GetMin()&&
			descriptor.GetValue()<=GetfRange().GetMax());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
	}

	/**************** HLDSchema **************/

	bool HLDSchema::Validate(const CLAM::DescriptionDataPool& pool)
	{
		std::list<CLAM_Annotator::HLDSchemaElement>& hlds = GetHLDs();
		std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
		bool validated = true;
		for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
		{
			const std::string & type = it2->GetType();
			const std::string & name = it2->GetName();
			if(type=="Int")
				validated = validated && 
					ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<int>("Song",name),name));
			else if(type=="Float")
				validated = validated && 
					ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<float>("Song",name),name));
			else if(type=="RestrictedString")
				validated = validated && 
					ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<RestrictedString>("Song",name),name));
			else if(type=="String")
				validated = validated &&
					ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<std::string>("Song",name),name));
			CLAM_ASSERT(validated,"NotValidated");
		}
		return validated;
	}

	HLDSchemaElement HLDSchema::FindElement(const std::string& descriptorName) const
	{
		std::list<HLDSchemaElement>::iterator it;
		for(it = GetHLDs().begin(); it != GetHLDs().end(); it++)
		{
			if (it->GetName() == descriptorName)
				return (*it);
		}
		CLAM_ASSERT(false, "Descriptor not found in Scheme");
	}

	/**************** Auxiliary functions	**************/

	std::string GetTypeFromValue(int i)
	{
		return "Int";
	}
	std::string GetTypeFromValue(float f)
	{
		return "Float";
	}
	std::string GetTypeFromValue(const std::string& s)
	{
		return "String";
	}
	std::string GetTypeFromValue(const CLAM_Annotator::RestrictedString& r)
	{
		return "RestrictedString";
	}


};
