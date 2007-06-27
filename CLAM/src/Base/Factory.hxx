/*
 * Copyright (c) 2007 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _Factory_hxx_
#define _Factory_hxx_

#include <map>
#include <string>
#include <list>
//#include <iostream> //TODO only for debugging

#include "Assert.hxx"
#include "ErrFactory.hxx"

namespace CLAM {

/**
Factory usage example. 
To define a factory for your types you should create your own factory subclass.

@code
// MyFactory.hxx
class MyFactory : public CLAM::Factory<MyAbstractProduct>
{
public:
	static MyFactory& GetInstance();
};
@endcode

You must define GetInstance() method in a .cxx 
@code
// MyFactory.cxx
#include "MyFactory.hxx"
MyFactory& MyFactory::GetInstance()
{	
	static MyFactory theInstance;
	return theInstance;
}
@endcode

To automatically register concrete products into the factory at program loading time 
(before "main()"), declare a registrator object like this:
@code
// put this in a .cxx (not in a header)
static CLAM::FactoryRegistrator<MyFactory, MyConcreteProduct> regMyConcreteProduct("MyConcreteProduct");
@endcode
 */
template <typename AbstractProductType>
class Factory
{
public:

	typedef AbstractProductType AbstractProduct;
	typedef std::string RegistryKey;

	typedef AbstractProduct* (*CreatorMethod)(void); //TODO drop after refactoring

	/** Abstract class for Creator objects which are stored in the Factory::Registry */
	class Creator
	{
	public:
		virtual AbstractProductType* Create() = 0;
		virtual ~Creator(){};
	};

	Factory() {};
	~Factory() {};
/*
	static Factory& GetInstance()	{
		static Factory theInstance;
		return theInstance;
	}
*/
	/**
	 * Gives ownership of the new created Product registered with
	 * the given name.
	 * It asserts that the name is in the registry.
	 */
	AbstractProduct* Create( const RegistryKey name )
	{
		Creator& creator = _registry.GetCreator( name );
		return creator.Create();
	}



	/** Gives ownership of the new created Product registered with
	 * the given name.
	 * It throws an ErrFactory if the name isn't found in the registry.
	 */
	AbstractProduct* CreateSafe( const RegistryKey name ) throw (ErrFactory)
	{
		return  _registry.GetCreatorSafe(name).Create();
	}


	void Clear()
	{ 
		_registry.RemoveAllCreators();
	}

	void AddCreator(const RegistryKey name, Creator* creator)
	{
		_registry.AddCreator(name, creator);
	}

	void AddCreatorWarningRepetitions(const RegistryKey name, Creator* creator)
	{
		_registry.AddCreatorWarningRepetitions(name, creator);
	}

	void AddCreatorSafe(const RegistryKey name, Creator* creator) throw (ErrFactory)
	{
		_registry.AddCreatorSafe(name, creator);
	}


	void GetRegisteredNames( std::list<std::string>& namesList )
	{
		_registry.GetRegisteredNames( namesList );
	}

public: // Inner classes. Public for better testing
	/**
	 * This class is an implementation class of the Factory. It is basically
	 * a container that maps keys with creators. It is not ment to be used
	 * directly by the user.
	 */
	class Registry
	{
	private:
		typedef typename std::map<std::string, Creator*> CreatorMap;

	public:
		Creator& GetCreator( RegistryKey creatorId) 
		{
			CLAM_ASSERT(_creators.begin() != _creators.end(),
				"the Factory Registry shouldn't be empty");

			Creator* res = CommonGetCreator(creatorId);
			if (!res)
			{
				std::string errmsg("GetCreator invoked with a non existent key: ");
				errmsg += creatorId + "\nRegistered keys are:\n";
				errmsg += GetRegisteredNames();
				CLAM_ASSERT(res,errmsg.c_str());
			}

			return *res;
		}

		Creator& GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory) 
		{
			if ( _creators.begin() == _creators.end() )
				throw ErrFactory("GetCreatorSafe invoked on an empty registry");

			Creator* res = CommonGetCreator(creatorId);
			if (!res)
			{
				std::string msg("GetCreatorSafe invoked with a non existent key: ");
				msg += creatorId;
				msg += "\nRegistered keys are:\n";
				msg += GetRegisteredNames();
				throw ErrFactory(msg.c_str());
			}
			return *res;
		}

		void AddCreator( RegistryKey creatorId, Creator* creator ) 
		{
			bool res = CommonAddCreator(creatorId, creator);
			if (!res)
			{
				std::string errmsg("Adding creator method in the factory: CreatorId '");
				errmsg += creatorId + "' was already registered.\nRegistered keys are:\n";
				errmsg += GetRegisteredNames();
				CLAM_ASSERT(res, errmsg.c_str());
			}
		}
		void AddCreatorWarningRepetitions( RegistryKey creatorId, Creator* creator ) 
		{
			bool res = CommonAddCreator(creatorId, creator);
			if (!res)
			{
				std::string errmsg("WARNING. While adding a creator method in the factory, id '");
				errmsg += creatorId + "' was already registered.";
//				errmsg += "\n Registered keys: " + GetRegisteredNames();
				CLAM_WARNING(false, errmsg.c_str() );
			}
		}

		void AddCreatorSafe( RegistryKey creatorId, Creator* creator ) throw (ErrFactory) 
		{
			if( !CommonAddCreator( creatorId, creator ) ) 
				throw ErrFactory("A repeated key was passed");
		}

		void RemoveAllCreators() 
		{
			_creators.clear();
		}

		std::size_t Count() { return _creators.size(); }

		void GetRegisteredNames( std::list<RegistryKey>& namesList )
		{
			typename CreatorMap::const_iterator i;

			for ( i = _creators.begin(); i != _creators.end(); i++ )
			{
				namesList.push_back( i->first );
			}
		}
		std::string GetRegisteredNames()
		{
			std::string result;
			typedef std::list<RegistryKey> Names;
			Names names;
			GetRegisteredNames(names);
			for(Names::iterator it=names.begin(); it!=names.end(); it++)
			{
				result += (*it)+", ";
			}
			return result;
			
		}

	private: // data
		CreatorMap _creators;

		// helper methods:
		Creator* CommonGetCreator( RegistryKey& creatorId ) 
		{
			typename CreatorMap::const_iterator i = 
				_creators.find(creatorId);
			if ( i==_creators.end() ) // not found
				return 0;
			return i->second;
		}

		bool CommonAddCreator( RegistryKey& creatorId, Creator* creator) 
		{
			// returns false if the key was repeated.
			typedef typename CreatorMap::value_type ValueType;
			return  _creators.insert( ValueType( creatorId, creator ) ).second;
		}

	};

	int Count() { return _registry.Count(); }

private:
	Registry _registry;

};


/**
* This class provides a convenient way to add items (creators) into a factory.
* To add class A (subclass of Base) to the factory it's useful to declare a static 
* FactoryRegistrator object like this: static FactoryRegistrator<MyTypeFactory, MyConcreteType> reg("key");
* The FactoryRegistrator constructor called at load-time is in charge to insert the creator
* to the factory.
* Various constructors exists giving the user options like using either 
* the singleton factory or a given one.
*/
template< typename TheFactoryType, typename ConcreteProductType>
class FactoryRegistrator
{
	typedef typename TheFactoryType::AbstractProduct AbstractProduct;
	typedef typename TheFactoryType::RegistryKey RegistryKey;
public:
	FactoryRegistrator( RegistryKey key, TheFactoryType& fact ) {
//		std::cout << "FactoryRegistrator(key,factory) " << key << std::endl;
		fact.AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}

	FactoryRegistrator( TheFactoryType& fact ) {
		ConcreteProductType dummy;
		RegistryKey key=dummy.GetClassName();
//		std::cout << "FactoryRegistrator(factory) " << dummy.GetClassName() << std::endl;
		fact.AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}

	FactoryRegistrator( RegistryKey key ) {
//		std::cout << "FactoryRegistrator(key) " << key << std::endl;
		TheFactoryType::GetInstance().AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}

	FactoryRegistrator( ) {
		ConcreteProductType dummy;
		RegistryKey key=dummy.GetClassName();
//		std::cout << "FactoryRegistrator() " << key << std::endl;
		TheFactoryType::GetInstance().AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}
	~FactoryRegistrator() {
//		std::cout << "~FactoryRegistrator() " << std::endl;
	}
	
	class ConcreteCreator : public TheFactoryType::Creator
	{
	public:
		AbstractProduct *Create()
		{	
			return new ConcreteProductType();
		}

	};
};

} // namespace

#endif // _Factory_hxx_

