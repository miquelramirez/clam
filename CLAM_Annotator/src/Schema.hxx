#ifndef _Schema_hxx_
#define _Schema_hxx_

#include "DynamicType.hxx"
#include "HLDSchema.hxx"

namespace CLAM_Annotator{


	class Schema : public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Schema,2);
		DYN_ATTRIBUTE(0, public, HLDSchema, HLDSchema);
		DYN_CONTAINER_ATTRIBUTE(1, public, std::list<std::string>, LLDNames, Names);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
		void AddFrameFloatAttribute(const std::string & name)
		{
			GetLLDNames().push_back(name);
		}
		void AddRestrictedString(
				const std::string & scope,
				const std::string & attribute,
				const char ** availableValues)
		{
			CLAM_Annotator::HLDSchemaElement testHLDesc;
			testHLDesc.SetScope(scope);
			testHLDesc.SetName(attribute);
			testHLDesc.SetType("RestrictedString");
			testHLDesc.AddRestrictionValues();
			testHLDesc.UpdateData();
			for (const char ** value = availableValues; *value; value++)
				testHLDesc.GetRestrictionValues().push_back(*value);
			AddAttribute(testHLDesc);
		}
		void AddRangedInt(
				const std::string & scope,
				const std::string & attribute,
				int min, int max)
		{
			CLAM_Annotator::HLDSchemaElement testHLDesc;
			testHLDesc.SetScope(scope);
			testHLDesc.SetName(attribute);
			testHLDesc.SetType("Int");
			testHLDesc.AddiRange();
			testHLDesc.UpdateData();
			CLAM_Annotator::Range<int> range;
			range.SetMin(min);
			range.SetMax(max);
			testHLDesc.SetiRange(range);
			AddAttribute(testHLDesc);
		}
		void AddRangedReal(
				const std::string & scope,
				const std::string & attribute,
				double min, double max)
		{
			CLAM_Annotator::HLDSchemaElement testHLDesc;
			testHLDesc.AddfRange();
			testHLDesc.UpdateData();
			testHLDesc.SetScope(scope);
			testHLDesc.SetName(attribute);
			testHLDesc.SetType("Float");
			CLAM_Annotator::Range<float> range;
			range.SetMin(min);
			range.SetMax(max);
			testHLDesc.SetfRange(range);
			AddAttribute(testHLDesc);
		}
		void AddString(
				const std::string & scope,
				const std::string & attribute)
		{
			CLAM_Annotator::HLDSchemaElement testHLDesc;
			testHLDesc.SetScope(scope);
			testHLDesc.SetName(attribute);
			testHLDesc.SetType("String");
			AddAttribute(testHLDesc);
		}
		void AddSegmentation(
				const std::string & scope,
				const std::string & attribute,
				const std::string & childScope)
		{
			CLAM_Annotator::HLDSchemaElement testHLDesc;
			testHLDesc.AddChildScope();
			testHLDesc.UpdateData();
			testHLDesc.SetScope(scope);
			testHLDesc.SetName(attribute);
			testHLDesc.SetType("Segmentation");
			testHLDesc.SetChildScope(childScope);
			AddAttribute(testHLDesc);
		}
		bool Validate(const CLAM::DescriptionDataPool & data);
	private:
		void AddAttribute(HLDSchemaElement & attribute)
		{
			GetHLDSchema().GetHLDs().push_back(attribute);
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
	};

};

#endif//_Schema_hxx_
