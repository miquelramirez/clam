#ifndef _SchemaAttribute_
#define _SchemaAttribute_

#include "DynamicType.hxx"

#include <list>
#include "Text.hxx"
#include "Enum.hxx"

#include "Assert.hxx"

#include "AnnotatorDescriptor.hxx"
#include "Enumerated.hxx"
#include "Range.hxx"
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
		DYNAMIC_TYPE(SchemaAttribute,8);
		DYN_ATTRIBUTE(0, public, std::string, Name); ///< The attribute name, unique within the scope.
		DYN_ATTRIBUTE(1, public, std::string, Scope); ///< The scope name at which the attribute sticks.
		DYN_ATTRIBUTE(2, public, std::string, Type); ///< The type name. Should be at the Type factory.
		/// To be added when type is Enumerated, contains the available values
		DYN_CONTAINER_ATTRIBUTE(3, public, std::list<std::string>, EnumerationValues, Value);
		/// To be \b optionaly added when type is Float, contains the boundary values
		DYN_ATTRIBUTE(4, public, Range<float>, fRange);
		/// To be added when type is Int, contains the boundary values
		DYN_ATTRIBUTE(5, public, Range<int>, iRange);
		/// To be added when type is Segmentation, contains the Scope containing attributes for segments.
		/// Empty string means no scope.
		DYN_ATTRIBUTE(6, public, std::string, ChildScope);
		DYN_ATTRIBUTE(7, public, SegmentationPolicy, SegmentationPolicy);
		
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

		bool Validate(const CLAM::DescriptionDataPool & pool) const;
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
			StoreEnumerationValues(storage);
			StorefRange(storage);
			StoreiRange(storage);
			StoreChildScope(storage);
			StoreSegmentationPolicy(storage);
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
			LoadEnumerationValues(storage);
			LoadfRange(storage);
			LoadiRange(storage);
			LoadChildScope(storage);
			LoadSegmentationPolicy(storage);
			UpdateData();
			UpdateTypePlugin();
		}
		void UpdateTypePlugin() const
		{
			if (mTypePlugin.t) delete mTypePlugin.t;
			mTypePlugin.t = TypePlugin::Create(*this);
		}
	private:
		mutable Holder<TypePlugin> mTypePlugin;
	};
};

#endif
