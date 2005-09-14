#ifndef _Schema_hxx_
#define _Schema_hxx_

#include "DynamicType.hxx"
#include "SchemaAttribute.hxx"

namespace CLAM_Annotator{


	class Schema : public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Schema,2);
		DYN_CONTAINER_ATTRIBUTE(0,public, std::list<SchemaAttribute>, Attributes, Attribute);
		DYN_CONTAINER_ATTRIBUTE(1, public, std::list<std::string>, LLDNames, Names);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
		void AddFrameFloatAttribute(const std::string & name)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope("Frame");
			schemaAttribute.SetName(name);
			schemaAttribute.SetType("Float");
			AddAttribute(schemaAttribute);
			GetLLDNames().push_back(name);
		}
		void AddRestrictedString(
				const std::string & scope,
				const std::string & attribute,
				const char ** availableValues)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("RestrictedString");
			schemaAttribute.AddRestrictionValues();
			schemaAttribute.UpdateData();
			for (const char ** value = availableValues; *value; value++)
				schemaAttribute.GetRestrictionValues().push_back(*value);
			AddAttribute(schemaAttribute);
		}
		void AddRangedInt(
				const std::string & scope,
				const std::string & attribute,
				int min, int max)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Int");
			schemaAttribute.AddiRange();
			schemaAttribute.UpdateData();
			CLAM_Annotator::Range<int> range;
			range.SetMin(min);
			range.SetMax(max);
			schemaAttribute.SetiRange(range);
			AddAttribute(schemaAttribute);
		}
		void AddRangedReal(
				const std::string & scope,
				const std::string & attribute,
				double min, double max)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.AddfRange();
			schemaAttribute.UpdateData();
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Float");
			CLAM_Annotator::Range<float> range;
			range.SetMin(min);
			range.SetMax(max);
			schemaAttribute.SetfRange(range);
			AddAttribute(schemaAttribute);
		}
		void AddString(
				const std::string & scope,
				const std::string & attribute)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("String");
			AddAttribute(schemaAttribute);
		}
		void AddSegmentation(
				const std::string & scope,
				const std::string & attribute,
				const std::string & childScope)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.AddChildScope();
			schemaAttribute.UpdateData();
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Segmentation");
			schemaAttribute.SetChildScope(childScope);
			AddAttribute(schemaAttribute);
		}
		bool Validate(const CLAM::DescriptionDataPool & data);
		const std::list<SchemaAttribute> & GetAllAttributes() const
	       	{
			return GetAttributes();
		}
	private:
		void AddAttribute(SchemaAttribute & attribute)
		{
			GetAttributes().push_back(attribute);
		}
		template <class T>
		Descriptor<T> MakeDescriptor(T value,const std::string& name)
		{
			Descriptor<T> desc;
			desc.SetValue(value);
			desc.SetName(name);
			desc.SetType(GetTypeFromValue(value));
			return desc;
		}
		template <class T>
		bool ValidateDescriptor(const Descriptor<T>& descriptor)
		{
			bool validated = true;
			
			validated = FindElement(descriptor.GetName()).Validate(descriptor);
			CLAM_DEBUG_ASSERT(validated,
				std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
			return validated;
		}

		SchemaAttribute FindElement(const std::string& descriptorName) const;
	};

};

#endif//_Schema_hxx_
