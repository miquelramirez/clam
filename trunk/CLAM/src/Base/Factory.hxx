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
//#include <iostream>

#include "Assert.hxx"
#include "ErrFactory.hxx"

namespace CLAM {

template <typename AbstractProductType>
class Factory
{
public:

	typedef AbstractProductType AbstractProduct;
	typedef std::string RegistryKey;

	typedef AbstractProduct* (*CreatorMethod)(void); //TODO drop after refactoring

	//TODO document
	class Creator
	{
	public:
		virtual AbstractProductType* Create() = 0;
		virtual ~Creator(){};
	};

	Factory() {};
	~Factory() {};

	static Factory& GetInstance()	{
		static Factory theInstance;
		return theInstance;
	}

	/**
	 * Gives ownership of the new created Product registered with
	 * the given name.
	 * It asserts that the name is in the registry.
	 */
	AbstractProduct* Create( const RegistryKey name )
	{
		// it asserts that name is in the registry
		CreatorMethod creator =	_registry.GetCreator( name );
		return (*creator)();
	}

	AbstractProduct* NewCreate( const RegistryKey name )
	{
		Creator& creator = _newregistry.GetCreator( name );
		return creator.Create();
	}



	/** Gives ownership of the new created Product registered with
	 * the given name.
	 * It throws an ErrFactory if the name isn't found in the registry.
	 */
	AbstractProduct* CreateSafe( const RegistryKey name ) throw (ErrFactory)
	{
		return  _registry.GetCreatorSafe(name)();
	}
	AbstractProduct* NewCreateSafe( const RegistryKey name ) throw (ErrFactory)
	{
		return  _newregistry.GetCreatorSafe(name).Create();
	}


	void Clear()
	{ 
		_registry.RemoveAllCreators();
		_newregistry.RemoveAllCreators();
	}


	void AddCreator(const RegistryKey name, CreatorMethod creator)
	{
		_registry.AddCreator(name, creator);
	}
	void NewAddCreator(const RegistryKey name, Creator* creator)
	{
		_newregistry.AddCreator(name, creator);
	}
	
	void AddCreatorWarningRepetitions(const RegistryKey name, CreatorMethod creator)
	{
		_registry.AddCreatorWarningRepetitions(name, creator);
	}
	void NewAddCreatorWarningRepetitions(const RegistryKey name, Creator* creator)
	{
		_newregistry.AddCreatorWarningRepetitions(name, creator);
	}


	void AddCreatorSafe(const RegistryKey name, CreatorMethod creator) throw (ErrFactory)
	{
		_registry.AddCreatorSafe(name, creator);
	}
	void NewAddCreatorSafe(const RegistryKey name, Creator* creator) throw (ErrFactory)
	{
		_newregistry.AddCreatorSafe(name, creator);
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
//	BEGIN CLASS BASED REGISTRY
	class NewRegistry
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

// 	END CLASS BASED REGISTRY

	class Registry
	{
	private:
		typedef typename std::map<std::string, CreatorMethod> CreatorMap;

	public:
		CreatorMethod GetCreator( RegistryKey creatorId) 
		{
			CLAM_ASSERT(_creators.begin() != _creators.end(),
				"the Factory Registry shouldn't be empty");

			CreatorMethod res = CommonGetCreator(creatorId);
			if (!res)
			{
				std::string errmsg("GetCreator invoked with a non existent key: ");
				errmsg += creatorId + "\nRegistered keys are:\n";
				errmsg += GetRegisteredNames();
				CLAM_ASSERT(res,errmsg.c_str());
			}

			return res;
		}

		CreatorMethod GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory) 
		{
			if ( _creators.begin() == _creators.end() )
				throw ErrFactory("GetCreatorSafe invoked on an empty registry");

			CreatorMethod res = CommonGetCreator(creatorId);
			if (!res)
			{
				std::string msg("GetCreatorSafe invoked with a non existent key: ");
				msg += creatorId;
				msg += "\nRegistered keys are:\n";
				msg += GetRegisteredNames();
				throw ErrFactory(msg.c_str());
			}
			return res;
		}

		void AddCreator( RegistryKey creatorId, CreatorMethod creator ) 
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
		void AddCreatorWarningRepetitions( RegistryKey creatorId, CreatorMethod creator ) 
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

		void AddCreatorSafe( RegistryKey creatorId, CreatorMethod creator ) throw (ErrFactory) 
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
		CreatorMethod CommonGetCreator( RegistryKey& creatorId ) {
			typename CreatorMap::const_iterator i = 
				_creators.find(creatorId);
			if ( i==_creators.end() ) // not found
				return NULL;
			return i->second;
		}

		bool CommonAddCreator( RegistryKey& creatorId, CreatorMethod creator) {
			// returns false if the key was repeated.
			typedef typename CreatorMap::value_type ValueType;
			return  _creators.insert( ValueType( creatorId, creator ) ).second;
		}

	};
	/**
 	* This class provides a convenient way to add items (creators) into a factory.
 	* To add class A (subclass of Base) to the factory it's useful to declare a static 
 	* Registrator object like this: static Factory<Base>::Registrator<A> reg("key");
 	* The Registrator constructor called at load-time is in charge to insert the creator
 	* to the factory.
 	* Various constructors exists giving the user options like using either 
 	* the singleton factory or a given one.
 	*/
	template< typename ConcreteProductType>
	class Registrator
	{
		typedef Factory<AbstractProductType> TheFactoryType;

	public:
		Registrator( RegistryKey key, TheFactoryType& fact ) {
			mKey=key;
//			std::cout << CLAM_MODULE << "Registrator(key,factory) " << mKey << std::endl;
			fact.AddCreatorWarningRepetitions( mKey, Create );
			fact.NewAddCreatorWarningRepetitions( mKey, new ConcreteCreator() );
		}

		Registrator( TheFactoryType& fact ) {
			ConcreteProductType dummy;
			mKey=dummy.GetClassName();
//			std::cout << CLAM_MODULE << "Registrator(factory) " << dummy.GetClassName() << std::endl;
			fact.AddCreatorWarningRepetitions( dummy.GetClassName(), Create );
			fact.NewAddCreatorWarningRepetitions( dummy.GetClassName(), new ConcreteCreator() );
		}

		Registrator( RegistryKey key ) {
			mKey=key;
//			std::cout << CLAM_MODULE << "Registrator(key) " << mKey << std::endl;
			TheFactoryType::GetInstance().AddCreatorWarningRepetitions( mKey, Create );
			TheFactoryType::GetInstance().NewAddCreatorWarningRepetitions( mKey, new ConcreteCreator() );
		}

		Registrator( ) {
			ConcreteProductType dummy;
			mKey=dummy.GetClassName();
//			std::cout << CLAM_MODULE << "Registrator() " << mKey << std::endl;
			TheFactoryType::GetInstance().AddCreatorWarningRepetitions( mKey, Create );
			TheFactoryType::GetInstance().NewAddCreatorWarningRepetitions( mKey, new ConcreteCreator() );
		}
		~Registrator() {
//			std::cout << CLAM_MODULE << "~Registrator() " << mKey << std::endl;
		}

		static AbstractProduct* Create() {
			return new ConcreteProductType;
		}
		class ConcreteCreator : public Creator
		{
		public:
			AbstractProduct *Create()
			{	
				return new ConcreteProductType();
			}

		};

	private:
		RegistryKey mKey;

	};
	int Count() { return _registry.Count(); }

private:
	Registry _registry;
	NewRegistry _newregistry;

};


} // namespace

#endif // _Factory_hxx_

