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
		DYNAMIC_TYPE(SchemaAttribute,11);
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
