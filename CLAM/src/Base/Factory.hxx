#ifndef _Factory_hxx_
#define _Factory_hxx_

#include <map>
#include <string>
#include <list>

#include "Assert.hxx"

namespace CLAM {

/// Error type thrown by Factory classes (Factory, FactoryRegistry)
class ErrFactory : public Err
{
public:
	ErrFactory(const char* msg) : Err(msg)
	{}

};


template <typename AbstractProductType>
class Factory
{
public:

	typedef AbstractProductType AbstractProduct;
	typedef AbstractProduct* (*CreatorMethod)(void);
	typedef std::string RegistryKey;

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

	/** Gives ownership of the new created Product registered with
	 * the given name.
	 * It throws an ErrFactory if the name isn't found in the registry.
	 */
	AbstractProduct* CreateSafe( const RegistryKey name ) throw (ErrFactory)
	{
		return  _registry.GetCreatorSafe(name)();
	}


	void Clear() { _registry.RemoveAllCreators(); }


	void AddCreator(const RegistryKey name, CreatorMethod creator)
	{
		_registry.AddCreator(name, creator);
	}


	void AddCreatorSafe(const RegistryKey name, CreatorMethod creator) throw (ErrFactory)
	{
		_registry.AddCreatorSafe(name, creator);
	}


	void GetRegisteredNames( std::list<std::string>& namesList )
	{
		_registry.GetRegisteredNames( namesList );
	}

public: // Inner classes. Public for better testing

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
			CLAM_ASSERT(res,"GetCreatorSafe invoked with a non existent key");

			return res;
		}

		CreatorMethod GetCreatorSafe( RegistryKey creatorId) throw (ErrFactory) 
		{
			if ( _creators.begin() == _creators.end() )
				throw ErrFactory("GetCreatorSafe invoked on an empty registry");

			CreatorMethod res = CommonGetCreator(creatorId);
			if (!res)
				throw ErrFactory("GetCreatorSafe invoked with a non existent key");
			return res;
		}

		void AddCreator( RegistryKey creatorId, CreatorMethod creator ) 
		{
			if( !CommonAddCreator( creatorId, creator ) ) 
					CLAM_ASSERT( false, "creatorId was already a key in the registry" );
			
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
			return  _creators.insert( CreatorMap::value_type( creatorId, creator ) ).second;
		}

	};

	template< typename ConcreteProductType>
	class Registrator
	{
		typedef Factory<AbstractProductType> TheFactoryType;

	public:
		Registrator( RegistryKey key, TheFactoryType& fact ) {
			fact.AddCreator( key, Create );
		}

		Registrator( TheFactoryType& fact ) {
			ConcreteProductType dummy;
			fact.AddCreator( dummy.GetClassName(), Create );
		}

		Registrator( RegistryKey key ) {
			TheFactoryType::GetInstance().AddCreator( key, Create );
		}

		Registrator( ) {
			ConcreteProductType dummy;
			TheFactoryType::GetInstance().AddCreator( dummy.GetClassName(), Create );
		}

		static AbstractProduct* Create() {
			return new ConcreteProductType;
		}

	};

private:
	Registry _registry;

};



} // namespace

#endif // _Factory_hxx_

