#ifndef _Polymorphic_hxx_
#define _Polymorphic_hxx_

#include "Component.hxx"
#include "XMLStorage.hxx"
#include "XMLAdapter.hxx"
#include <string>

// TODO
// Memory management
// Pointer, Reference or Get interface?
// DT required interface
// Change Dummys
// Adopt function

namespace CLAM
{
	/**
	 * This class is an utility class to hold polymorphic pointers as attributes
	 * on DynamicTypes and simplify the problem of the XML Loading
	 * of abstract XML Schema types.
	 * - This class holds the property of the object, this means that
	 *   the destructor will destroy it.
	 * - You can adopt the pointer property but the Polymorphic object will
	 *   loose the reference to the object.
	 * @experimental This is a draft
	 */
	template <typename FactoryType>
	class Polymorphic : public Component
	{
	public:
		typedef FactoryType Factory;
		typedef typename Factory::AbstractProduct Abstract;
	private:
		Abstract * mAdaptee;

	public:
		Polymorphic()
		{
			mAdaptee = 0;
		}

		Polymorphic(const std::string & concreteClassName)
		{
			mAdaptee = Factory::GetInstance().Create(concreteClassName);
		}

		Polymorphic(Abstract & toCopy)
		{
			mAdaptee = & toCopy;
		}

		~Polymorphic()
		{
			if (mAdaptee) delete mAdaptee;
		}

		Polymorphic & operator=(Abstract & toCopy)
		{
			mAdaptee = & toCopy;
		}

		Abstract & Get()
		{
			CLAM_ASSERT(mAdaptee, "Derreferencing a null polymorph pointer");
			return *mAdaptee;
		}

		const Abstract & Get() const
		{
			CLAM_ASSERT(mAdaptee, "Derreferencing a null polymorph pointer");
			return *mAdaptee;
		}

		void Set(Abstract * pointee)
		{
			mAdaptee = pointee;
		}

		const char * GetClassName() const 
		{
			return "Polymorphic";
		}

		operator Abstract&()
		{
			return *mAdaptee;
		}

		operator Abstract* ()
		{
			return mAdaptee;
		}

		operator const Abstract&() const 
		{
			return *mAdaptee;
		}


		void StoreOn(Storage & storage) const
		{
			// TODO case nill
			std::string className = mAdaptee->GetClassName();
			XMLAdapter<std::string> typeAttribute(className, "type", false);
			storage.Store(typeAttribute);
			mAdaptee->StoreOn(storage);
		}

		void LoadFrom(Storage & storage)
		{
			// TODO case nill
			// TODO case incorrect type
			// TODO case mAdaptee!=null
			std::string className;
			XMLAdapter<std::string> typeAttribute(className, "type", false);
			storage.Load(typeAttribute);
			mAdaptee = Factory::GetInstance().Create(className);
			mAdaptee->LoadFrom(storage);
		}
		
		
	};
}
#endif//_Polymorphic_hxx_

