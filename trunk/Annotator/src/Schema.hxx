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

#ifndef _Schema_hxx_
#define _Schema_hxx_
#include <typeinfo>
#include <CLAM/DynamicType.hxx>
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
		void AttributeDocumentation(
				const std::string & scope,
				const std::string & attribute,
				const std::string & documentationHtml)
		{
			SchemaAttribute & attributeSchema = GetAttribute(scope, attribute);
			attributeSchema.AddDocumentation();
			attributeSchema.UpdateData();
			attributeSchema.SetDocumentation(documentationHtml);
		}
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
		void AddFloatArray(
				const std::string & scope,
				const std::string & attribute,
				const char ** keys)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("FloatArray");
			schemaAttribute.AddBinLabels();
			schemaAttribute.UpdateData();
			for (const char ** value = keys; *value; value++)
				schemaAttribute.GetBinLabels().push_back(*value);

			AddAttribute(schemaAttribute);
		}
		
		void AddFloatArray(
				const std::string & scope,
				const std::string & attribute,
				double firstBin,
				double binGap,
				int nBins)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("FloatArray");
			
			schemaAttribute.AddFirstBinOffset();
			schemaAttribute.AddBinGap();
			schemaAttribute.AddNBins();
			
			schemaAttribute.UpdateData();
			
			schemaAttribute.SetFirstBinOffset(firstBin);
			schemaAttribute.SetBinGap(binGap);
			schemaAttribute.SetNBins(nBins);
			
			AddAttribute(schemaAttribute);
		}

		void AddRangedInt(
				const std::string & scope,
				const std::string & attribute,
				int min, int max)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.AddMinValue();;
			schemaAttribute.AddMaxValue();
			schemaAttribute.UpdateData();
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Int");

			schemaAttribute.SetMinValue(min);
			schemaAttribute.SetMaxValue(max);
			AddAttribute(schemaAttribute);
		}
		void AddRangedReal(
				const std::string & scope,
				const std::string & attribute,
				double min, double max)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.AddMinValue();;
			schemaAttribute.AddMaxValue();
			schemaAttribute.UpdateData();
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("Float");

			schemaAttribute.SetMinValue(min);
			schemaAttribute.SetMaxValue(max);
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
		void AddFrameDivision(
				const std::string & scope,
				const std::string & attribute,
				const std::string & childScope)
		{
			CLAM_Annotator::SchemaAttribute schemaAttribute;
			schemaAttribute.AddChildScope();
			schemaAttribute.UpdateData();
			schemaAttribute.SetScope(scope);
			schemaAttribute.SetName(attribute);
			schemaAttribute.SetType("FrameDivision");
			schemaAttribute.SetChildScope(childScope);
			AddAttribute(schemaAttribute);
		}
		bool Validate(const CLAM::DescriptionDataPool & data, std::ostream & err) const;
		void InitInstance(const std::string & scope, unsigned instance, CLAM::DescriptionDataPool & data) const;
		void InitMissingAttributes(CLAM::DescriptionDataPool & data) const;
		const std::list<SchemaAttribute> & GetAllAttributes() const
		{
			return GetAttributes();
		}
		SchemaAttribute & GetAttribute(const std::string & scope, const std::string name)
		{
			std::list<SchemaAttribute> & attributes = GetAttributes();
			for (std::list<SchemaAttribute>::iterator it = attributes.begin();
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
		const SchemaAttribute & GetAttribute(const std::string & scope, const std::string name) const
		{
			const std::list<SchemaAttribute> & attributes = GetAttributes();
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

