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

#ifndef _SchemaAttribute_
#define _SchemaAttribute_

#include <CLAM/DynamicType.hxx>

#include <list>
#include <CLAM/Text.hxx>
#include <CLAM/Enum.hxx>

#include <CLAM/Assert.hxx>

#include "Enumerated.hxx"
#include "TypePlugin.hxx"

namespace CLAM { class DescriptionScheme; }
namespace CLAM { class DescriptionDataPool; }
namespace CLAM_Annotator { class TypePlugin; }

namespace CLAM_Annotator{
	
	class SegmentationPolicy : public CLAM::Enum
	{
	public:
		SegmentationPolicy() : CLAM::Enum(ValueTable(), eUnsized) {}
		SegmentationPolicy(tValue v) : CLAM::Enum(ValueTable(), v) {};
		SegmentationPolicy(const std::string & s) : CLAM::Enum(ValueTable(), s) {};

		virtual CLAM::Component * Species() const {return new SegmentationPolicy();}

		typedef enum {
			eUnsized,
			eContinuous,
			eDiscontinuous,
			eOverlapping
			
		} tEnum;
 		static tEnumValue * ValueTable()
 		{
 			static tEnumValue sValueTable[] = 
 			{
 				{eUnsized,"Unsized"},
 				{eContinuous,"Continuous"},
 				{eDiscontinuous,"Discontinuous"},
 				{eOverlapping,"Overlapping"},
 				{0,NULL}
 			};
 			return sValueTable;
 		}
	};
	
	class SchemaAttribute:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(SchemaAttribute,14);
		DYN_ATTRIBUTE(0, public, std::string, Name); ///< The attribute name, unique within the scope.
		DYN_ATTRIBUTE(1, public, std::string, Scope); ///< The scope name at which the attribute sticks.
		DYN_ATTRIBUTE(2, public, std::string, Type); ///< The type name. Should be at the Type factory.
		DYN_ATTRIBUTE(3, public, CLAM::Text, Documentation); ///< Documentation for the attribute
		/// To be added when type is Enumerated, contains the available values
		DYN_CONTAINER_ATTRIBUTE(4, public, std::list<std::string>, EnumerationValues, Value);
		DYN_ATTRIBUTE(6, public, CLAM::TData, MinValue);
		DYN_ATTRIBUTE(5, public, CLAM::TData, MaxValue);
		DYN_ATTRIBUTE(7, public, std::string, Units);
		/// To be added when type is Segmentation, contains the Scope containing attributes for segments.
		/// Empty string means no scope.
		DYN_ATTRIBUTE(8, public, std::string, ChildScope);
		/// To be added when type is Segmentation, specifies segments restrictions
		DYN_ATTRIBUTE(9, public, SegmentationPolicy, SegmentationPolicy);
		/// To be added when you have a float array
		DYN_CONTAINER_ATTRIBUTE(10, public, std::list<std::string>, BinLabels, Value);
		/// To be added when you have a float array and are looking to store MFCC or Spectrum data
		/// Store First Bin Frequency
		DYN_ATTRIBUTE(11, public, CLAM::TData, FirstBinFreq);
		/// Store Frequency Gap between Bins
		DYN_ATTRIBUTE(12, public, CLAM::TData, BinGap);
		/// Store Number of Bins
		DYN_ATTRIBUTE(13, public, CLAM::TData, NBins);
		
		
		void DefaultInit();
	public:
		template <class T>
		class Holder
		{
		public:
			Holder() : t(0) { }
			~Holder()
			{
				if (t) delete t;
			}
			T* t;
		};

		bool Validate(const CLAM::DescriptionDataPool & pool, std::ostream & err) const;
		void AddTo(CLAM::DescriptionScheme & scheme) const;

		void StoreOn(CLAM::Storage & storage) const
		{
			typedef CLAM::XMLAdapter<std::string> Adapter;
			Adapter adapter1(GetName(), "name", false);
			storage.Store(adapter1);
			Adapter adapter2(GetScope(), "scope", false);
			storage.Store(adapter2);
			Adapter adapter3(GetType(), "type", false);
			storage.Store(adapter3);
			StoreDocumentation(storage);
			StoreEnumerationValues(storage);
			StoreMinValue(storage);
			StoreMaxValue(storage);
			StoreUnits(storage);
			StoreChildScope(storage);
			StoreSegmentationPolicy(storage);
			StoreBinLabels(storage);
			StoreFirstBinFreq(storage);
			StoreBinGap(storage);
			StoreNBins(storage);
		}
		
		void LoadFrom(CLAM::Storage & storage)
		{
			AddAll();
			UpdateData();
			typedef CLAM::XMLAdapter<std::string> Adapter;
			Adapter adapter1(GetName(), "name", false);
			if (!storage.Load(adapter1)) RemoveName();
			Adapter adapter2(GetScope(), "scope", false);
			if (!storage.Load(adapter2)) RemoveScope();
			Adapter adapter3(GetType(), "type", false);
			if (!storage.Load(adapter3)) RemoveType();
			LoadDocumentation(storage);
			LoadEnumerationValues(storage);
			LoadMinValue(storage);
			LoadMaxValue(storage);
			LoadUnits(storage);
			LoadChildScope(storage);
			LoadSegmentationPolicy(storage);
			LoadBinLabels(storage);
			LoadFirstBinFreq(storage);
			LoadBinGap(storage);
			LoadNBins(storage);
			UpdateData();
			UpdateTypePlugin();
		}
		void UpdateTypePlugin() const
		{
			if (mTypePlugin.t) delete mTypePlugin.t;
			mTypePlugin.t = TypePlugin::Create(*this);
		}
		void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool) const
		{
			mTypePlugin.t->InitInstance(instance, pool);
		}
	private:
		mutable Holder<TypePlugin> mTypePlugin;
	};
};

#endif

