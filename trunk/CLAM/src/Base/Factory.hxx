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
#include <set>
#include <iostream> //TODO only for debugging

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

private:	
	typedef std::string Attribute;
	typedef std::string Value;
	struct Pair 
	{
		Attribute attribute;
		Value value;
	};
	typedef RegistryKey Key; // TODO remove

public:
	typedef std::list<Key> Keys;
	typedef std::list<std::string> Values;
	typedef std::list<Pair> Pairs; 

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

	bool ExistsKey( const RegistryKey& key) //TODO pau: rename to KeyExists
	{
		return _registry.ExistsKey(key);
	}

	bool AttributeExists (const std::string& key, const std::string& attribute)
	{
		return _registry.AttributeExists(key,attribute);
	}

	/// Get all keys that have attribute==value in its metadata.
	Keys GetKeys(const std::string& attribute, const std::string& value)
	{
		return _registry.GetKeys(attribute, value);
	}
	/// Get all keys in the factory
	Keys GetKeys()
	{
		return GetKeys("","");
	}
	/// Return all the metadata available for a product key
	Pairs GetPairsFromKey(const std::string& key)
	{
		return _registry.GetPairsFromKey(key);
	}
	/// Get the set of all values present for a given metadata attribute.
	/// Example GetSetOfValues("category") could return ["modulators","generators","reverbs"] without repeated items.
	Values GetSetOfValues(const std::string& attribute)
	{
		return _registry.GetSetOfValues(attribute);
	}
	/// Return the list of values for a metadata attribute for a product key.
	Values GetValuesFromAttribute(const std::string& key, const std::string& attribute)
	{
		return _registry.GetValuesFromAttribute(key, attribute);
	}
	/// Return the value for a metadata attribute of product key.
	/// If multiple values exist returns the first value.
	Value GetValueFromAttribute(const std::string& key, const std::string& attribute)
	{
		return GetValuesFromAttribute(key,attribute).front();
	}

	void AddAttribute(const std::string& key, const std::string& attribute, const std::string& value)
	{
		_registry.AddAttribute(key, attribute, value);
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
		struct FactoryEntry {
			Creator * creator;
			Pairs pairs;
		};
		typedef std::map<Key, FactoryEntry> FactoryEntries;

	public:
		Creator& GetCreator( RegistryKey creatorId) 
		{
			CLAM_ASSERT(_factoryEntries.begin() != _factoryEntries.end(),
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
			if ( _factoryEntries.begin() == _factoryEntries.end() )
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
			_factoryEntries.clear();
		}

		std::size_t Count() { return _factoryEntries.size(); }

		void GetRegisteredNames( std::list<RegistryKey>& namesList )
		{
			typename FactoryEntries::const_iterator i;

			for ( i = _factoryEntries.begin(); i != _factoryEntries.end(); i++ )
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

		bool ExistsKey(const RegistryKey& key)
		{
			typename FactoryEntries::const_iterator it = _factoryEntries.find(key);
			if(it == _factoryEntries.end())
			{
				return false;
			}
			return true;
		}
		bool AttributeExists(const std::string& key, const std::string& attribute)
		{
			Pairs pairsFromKey = GetPairsFromKey(key);
			typename Pairs::const_iterator itPairs;
			for (itPairs=pairsFromKey.begin();itPairs!=pairsFromKey.end();itPairs++)
			{
				if (itPairs->attribute==attribute) return true;
			}
			return false;
		}

		/// Get all keys that have attribute==value in its metadata.
		Keys GetKeys(const std::string& attribute, const std::string& value)
		{
			Keys result;
			typename FactoryEntries::const_iterator it;
			for(it = _factoryEntries.begin(); it != _factoryEntries.end(); it++)
			{
				if( (attribute == "") )
				{
					result.push_back(it->first);
					continue;
				}
				Pairs attributes = it->second.pairs;
				typename Pairs::const_iterator itAtt;
				for(itAtt = attributes.begin(); itAtt != attributes.end(); itAtt++)
				{
					if( ((*itAtt).attribute == attribute) && ((*itAtt).value == value) )
					{
						result.push_back(it->first);
					}
				}
			}
			return result;
		}
		/// Get all keys in the factory
		Keys GetKeys()
		{
			return GetKeys("","");
		}
		/// Return all the metadata available for a product key
		Pairs GetPairsFromKey(const std::string& key)
		{
			Pairs attributes;
			typename FactoryEntries::const_iterator it = _factoryEntries.find(key);
			if(it!=_factoryEntries.end())
			{
				attributes = it->second.pairs;
			}
			return attributes;
		}
		/// Get the set of all values present for a given metadata attribute.
		/// Example GetSetOfValues("category") could return ["modulators","generators","reverbs"] without repeated items.
		Values GetSetOfValues(const std::string& attribute)
		{
			std::set<Value> AttributeSet;
			std::set<Value>::const_iterator itSet;
			Values values;
			typename FactoryEntries::const_iterator it;
			for(it = _factoryEntries.begin(); it != _factoryEntries.end(); it++)
			{
				Pairs attributes = it->second.pairs;
				typename Pairs::const_iterator itAtt;
				for(itAtt = attributes.begin(); itAtt != attributes.end(); itAtt++)
				{
					if((*itAtt).attribute == attribute)
					{
						itSet = AttributeSet.find((*itAtt).value);
						if(itSet == AttributeSet.end())
						{
							AttributeSet.insert((*itAtt).value);
						}
					}
				}
			}
			// keep using the ProcessingFactory::Values
			for(itSet = AttributeSet.begin(); itSet != AttributeSet.end(); itSet++)
			{
				values.push_back(*itSet);
			}
			return values;
		}
		/// Return the list of values for a metadata attribute for a product key.
		Values GetValuesFromAttribute(const std::string& key, const std::string& attribute)
		{
			Values values;
			typename FactoryEntries::const_iterator it = _factoryEntries.find(key);
			if(it != _factoryEntries.end())
			{
				typename Pairs::const_iterator itAtt;
				for(itAtt = it->second.pairs.begin(); itAtt != it->second.pairs.end(); itAtt++)
				{
					if((*itAtt).attribute == attribute)
					{
						values.push_back((*itAtt).value);
					}
				}
			}
			return values;
		}
		/// Return the value for a metadata attribute of product key.
		Value GetValueFromAttribute(const std::string& key, const std::string& attribute)
		{
			return GetValuesFromAttribute(key,attribute).front();
		}

		void AddAttribute(const std::string& key, const std::string& attribute, const std::string& value)
		{
			typename FactoryEntries::const_iterator it;
			it = _factoryEntries.find(key);
			/*if(!ExistsKey(key)) // NOT NEEDED AFETER UNIFYING
			{
				std::cout << "[Factory] tryind to add metadata to a non-existing key \"" << key << "\"" << std::endl; 
		//		return;  //pau: debugging: add metadata anyway. maybe factory registrator is about to be instantiated.
			}*/
			
			Pair pair;
			pair.attribute = attribute;
			pair.value = value;

			_factoryEntries[key].pairs.push_back(pair);
			/*Pairs pairs;
			if(it == _factoryEntries.end()) // it's a new key: insert it in the _factoryEntries map
			{
				pairs.push_back(pair);
				_factoryEntries.insert( typename FactoryEntries::value_type( key, pairs ) );

			} 
			else
			{
				_factoryEntries[key].push_back(pair);
			}*/
		}


	private: // data
		FactoryEntries _factoryEntries;

		// helper methods:
		Creator* CommonGetCreator( RegistryKey& creatorId ) 
		{
			typename FactoryEntries::const_iterator i = 
				_factoryEntries.find(creatorId);
			if ( i==_factoryEntries.end() ) // not found
				return 0;
			return i->second.creator;
		}

		bool CommonAddCreator( RegistryKey& creatorId, Creator* creator) 
		{
			FactoryEntry factoryEntry;
			Pairs pairs;
			factoryEntry.creator = creator;
			factoryEntry.pairs = pairs;
			// returns false if the key was repeated.
			typedef typename FactoryEntries::value_type ValueType;
			return  _factoryEntries.insert( ValueType( creatorId, factoryEntry ) ).second;
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
	FactoryRegistrator( const char* metadata[] ) 
	{
		CLAM_ASSERT(std::string(metadata[0])==std::string("key"), "FactoryRegistrator: first char* metadata should be 'key'"); //TODO fix
		CLAM_ASSERT(metadata[1], "FactoryRegistrator: value for first attriute ('key') must not be 0");
		std::string key = metadata[1];

		TheFactoryType & factory = TheFactoryType::GetInstance();
		factory.AddCreatorWarningRepetitions( key, new ConcreteCreator() );
		std::string attribute, value;
		for(unsigned i = 2; metadata[i]; i++)
		{
			if(!metadata[i+1])
			{
				std::cout << "[METADATA] error with attribute \"" << metadata[i] << "\"" << std::endl;
			}
			attribute = metadata[i];
			value = metadata[++i];
			factory.AddAttribute(key, attribute, value);
		}
	}

	FactoryRegistrator( RegistryKey key, TheFactoryType& fact ) 
	{
//		std::cout << "FactoryRegistrator(key,factory) " << key << std::endl;
		fact.AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}

	FactoryRegistrator( TheFactoryType& fact ) 
	{
		ConcreteProductType dummy;
		RegistryKey key=dummy.GetClassName();
//		std::cout << "FactoryRegistrator(factory) " << dummy.GetClassName() << std::endl;
		fact.AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}

	FactoryRegistrator( RegistryKey key ) 
	{
//		std::cout << "FactoryRegistrator(key) " << key << std::endl;
		TheFactoryType::GetInstance().AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}

/*
	FactoryRegistrator( ) 
	{
		ConcreteProductType dummy;
		RegistryKey key=dummy.GetClassName();
//		std::cout << "FactoryRegistrator() " << key << std::endl;
		TheFactoryType::GetInstance().AddCreatorWarningRepetitions( key, new ConcreteCreator() );
	}
*/
	~FactoryRegistrator() 
	{
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

