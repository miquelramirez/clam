#ifndef _HLDSchema_
#define _HLDSchema_

#include "DynamicType.hxx"

#include <list>
#include "Text.hxx"

#include "Pool.hxx"
#include "Assert.hxx"

#include "EType.hxx"
#include "AnnotatorDescriptor.hxx"
#include "RestrictedString.hxx"
#include "Range.hxx"

namespace CLAM_Annotator{
	
	//TODO: type should be an enum not a string
	
	class HLDSchemaElement:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(HLDSchemaElement,7);
		DYN_ATTRIBUTE(0, public, std::string, Name);
		DYN_ATTRIBUTE(1, public, std::string, Scope);
		//TODO: this should better be an enum
		DYN_ATTRIBUTE(2, public, std::string, Type);
		/*In case type is a RestrictedString, an optional list of enumeration values must be offered*/
		DYN_CONTAINER_ATTRIBUTE(3, public, std::list<std::string>, RestrictionValues, Restriction);
		//In case type is a number we need to add a range
		DYN_ATTRIBUTE(4, public, Range<float>, fRange);
		DYN_ATTRIBUTE(5, public, Range<int>, iRange);
		DYN_ATTRIBUTE(6, public, std::string, ChildScope);
		
		void DefaultInit();
		
	public:
		template <class T>
		bool Validate (const Descriptor<T>& descriptor)const
		{
			return true;
		}
		bool Validate (const Descriptor<CLAM_Annotator::RestrictedString>& descriptor)const;
		bool Validate (const Descriptor<int>& descriptor)const;
		bool Validate (const Descriptor<float>& descriptor)const;
		
	};
		
	
	class HLDSchema:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(HLDSchema,1);
		DYN_CONTAINER_ATTRIBUTE(0,public, std::list<HLDSchemaElement>,HLDs,Attribute);
		
		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
	public:
		template <class T>
		bool ValidateDescriptor(T value,const std::string& name)
		{
			return ValidateDescriptor<T>(MakeDescriptor(value,name));
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

		bool Validate(const CLAM::DescriptionDataPool& pool);
		HLDSchemaElement FindElement(const std::string& descriptorName) const;
		
	private:
		template <class T>
		Descriptor<T> MakeDescriptor(T value,const std::string& name)
		{
			Descriptor<T> desc;
			desc.SetValue(value);
			desc.SetName(name);
			desc.SetType(GetTypeFromValue(value));
			return desc;
		}
	public:
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
			GetHLDs().push_back(testHLDesc);
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
			GetHLDs().push_back(testHLDesc);
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
			GetHLDs().push_back(testHLDesc);
		}
		void AddString(
				const std::string & scope,
				const std::string & attribute)
		{
			CLAM_Annotator::HLDSchemaElement testHLDesc;
			testHLDesc.SetScope(scope);
			testHLDesc.SetName(attribute);
			testHLDesc.SetType("String");
			GetHLDs().push_back(testHLDesc);
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
			GetHLDs().push_back(testHLDesc);
		}




	};


	std::string GetTypeFromValue(int i);
	std::string GetTypeFromValue(float f);
	std::string GetTypeFromValue(const CLAM::Text& s);
	std::string GetTypeFromValue(const CLAM_Annotator::RestrictedString& r);
};

#endif
