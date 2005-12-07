#ifndef _Schema_hxx_
#define _Schema_hxx_

#include "DynamicType.hxx"
#include "SchemaAttribute.hxx"

namespace CLAM_Annotator{


	class Schema : public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Schema,2);
		DYN_ATTRIBUTE(0,public, CLAM::Text, Uri);
		DYN_CONTAINER_ATTRIBUTE(1,public, std::list<SchemaAttribute>, Attributes, Attribute);
	private:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetUri("descriptionScheme:www.iua.upf.edu:clam:none");
		}

	public:
		void AddFrameFloatAttribute(const std::string & name)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope("Frame");
			schemaAttribute.SetName(name);
			schemaAttribute.SetType("Float");
			AddAttribute(schemaAttribute);
		}
		void AddEnumerated(
				const std::string & scope,
				const std::string & attribute,
				const char ** availableValues)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Enumerated");
			schemaAttribute.AddEnumerationValues();
			schemaAttribute.UpdateData();
			for (const char ** value = availableValues; *value; value++)
				schemaAttribute.GetEnumerationValues().push_back(*value);
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
				SegmentationPolicy policy,
				const std::string & childScope)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.AddChildScope();
			schemaAttribute.AddSegmentationPolicy();
			schemaAttribute.UpdateData();
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Segmentation");
			schemaAttribute.SetSegmentationPolicy(policy);
			schemaAttribute.SetChildScope(childScope);
			AddAttribute(schemaAttribute);
		}
		bool Validate(const CLAM::DescriptionDataPool & data);
		const std::list<SchemaAttribute> & GetAllAttributes() const
	       	{
			return GetAttributes();
		}
		const SchemaAttribute & GetAttribute(const std::string & scope, const std::string name) const
		{
			const std::list<SchemaAttribute> & attributes = GetAllAttributes();
			for (std::list<SchemaAttribute>::const_iterator it = attributes.begin();
					it!=attributes.end();
					it++)
			{
				if (it->GetScope()!=scope) continue;
				if (it->GetName()!=name) continue;
				return *it;
			}
			std::string message = "Accessing an attribute '";
			message += scope + "':'" + name + "' not in the scheme.";
			CLAM_ASSERT(false, message.c_str());
		}
	private:
		void AddAttribute(SchemaAttribute & attribute)
		{
			GetAttributes().push_back(attribute);
			GetAttributes().back().UpdateTypePlugin();
		}
	};

};

#endif//_Schema_hxx_
