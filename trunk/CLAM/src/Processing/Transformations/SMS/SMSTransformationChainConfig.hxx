/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _SMSTransformationChainConfig_
#define _SMSTransformationChainConfig_

#include "ProcessingComposite.hxx"
#include "ProcessingData.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControlTmplArray.hxx"
#include "Array.hxx"

#include "Frame.hxx" //TODO provisional
#include "SegmentTransformation.hxx" //TODO provisional

namespace CLAM{


	/**	Special Processing config that can be used inside a SMSTransformationChainConfig. It is a
	 *	polymorphic configuration that delegates this polymorphism to one of its members 
	 *	(@see mpConcreteConfig) and has access to the concrete class through its name stored
	 *	in a dynamic attribute (ConcreteClassName)
	 */
	class SMSTransformationChaineeConfig:public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SMSTransformationChaineeConfig, 1,ProcessingConfig);
		/** Name of concrete Config class */
		DYN_ATTRIBUTE (0, public, std::string, ConcreteClassName);
	public:
		/** Initialization for default constructor. All attributes are added, ConcreteClassName is
		 *	set to "Unknown" and pointer to concrete configuration is set to null. 
		 */
		void DefaultInit();
		/** Initialization for copy constructor. All dynamic attributes are added and copied from
		 *	original configuration. Concrete Configuration is set 'by hand'.
		 */
		void CopyInit(const SMSTransformationChaineeConfig& originalConfig);
		/** Overriding virtual method in base class to store concrete configuration by hand as it
		 *	is not a dynamic attribute.
		 */
		void StoreOn(Storage & s) const; 
		/** Overriding virtual method in base class to load concrete configuration by hand as it
		 *	is not a dynamic attribute.
		 */
		void LoadFrom(Storage& s);
		/** Returns the concrete configuration as a reference to the base class.*/
		ProcessingConfig& GetConcreteConfig() const {return *mpConcreteConfig;}
		/** Sets the concrete configuration, passing a reference to the base class. Note
		 *	though, that the actual object referenced must be a concrete configuration of
		 *	the same class indicated by the ConcreteClassName attribute.
		 */
		void SetConcreteConfig(const ProcessingConfig& cfg)
		{
			AddConcreteConfig();
			*mpConcreteConfig=cfg;
		}

		/** Virtual destructor. Deletes pointer to concrete configuration */
		virtual ~SMSTransformationChaineeConfig();

		/** Adds a new instantiated concrete configuration using the Concrete Class name as a
		 *	type selector (ConcreteClassName must be set in advanced) deleting any previously
		 *	existing concrete configuration.
		 */
		void AddConcreteConfig()
		{
			if(mpConcreteConfig) 
				delete mpConcreteConfig;
			mpConcreteConfig=InstantiateConcreteConfig();
		}

		
	protected:
		/** Instantiates a concrete configuration using input string as a type selector. */
		ProcessingConfig* InstantiateConcreteConfig(const std::string& type);

		/** Instantiates a concrete configuration using the ConcreteClassName attribute as a
		 *	type selector (ConcreteClassName must be set in advanced)
		 */
		ProcessingConfig* InstantiateConcreteConfig()
		{
			return InstantiateConcreteConfig(GetConcreteClassName());
		}
		
		/** Actual pointer to the concrete configuration. It is a pointer to base class but the
		 *	object is always an instance of a concrete one.
		 */
		ProcessingConfig* mpConcreteConfig;
		
	};


	/** Configuration for a SMSTransformationChain. It is basically made of a list of
	 *	children ProcessingConfigs. Pointers are used to be able to handle polymorphism
	 *	on these children. These pointers to a ProcessingConfig are indeed pointers to
	 *	instances of a derived class.
	 */
	class SMSTransformationChainConfig:public ProcessingConfig
	{
	public:
		typedef std::list<SMSTransformationChaineeConfig>::iterator iterator;
		typedef std::list<SMSTransformationChaineeConfig>::const_iterator const_iterator;

		DYNAMIC_TYPE_USING_INTERFACE (SMSTransformationChainConfig, 2, ProcessingConfig);
		/** List of children configurations, a list of pointers to base class is kept */
		DYN_CONTAINER_ATTRIBUTE (0, public, std::list<SMSTransformationChaineeConfig>, Configurations, Config);
		/** Array of On/off initial values for control*/
		DYN_ATTRIBUTE (1, public, Array<bool>, OnArray);

		
		/** By default all attributes are added. */
		void DefaultInit();
		/** Returns a configuration iterator at the beginning of the list*/
		iterator ConfigList_begin() {return GetConfigurations().begin();}
		/** Returns a configuration iterator at the end of the list*/
		iterator ConfigList_end() {return GetConfigurations().end();}

		/** Returns a configuration iterator at the beginning of the list*/
		const_iterator ConfigList_begin_const() const {return GetConfigurations().begin();}
		/** Returns a configuration iterator at the end of the list*/
		const_iterator ConfigList_end_const() const {return GetConfigurations().end();}

		bool AddSMSConfiguration(const SegmentTransformationConfig& cfg,std::string type)
		{
			AddConfiguration(cfg,type);
			return true;
		}

		/** Returns the size of the configuration list*/
		std::size_t ConfigList_size() const {return GetConfigurations().size();}

		//TODO: maybe a method for deleting a configuration is also necessary
		
		/** Returns true if there are no configurations */
		bool IsEmpty(){return GetnConfigurations()<=0;}
		/** Accessor to the number of configurations in the list*/
		TSize GetnConfigurations(){return ConfigList_size();}

		/** Virtual destructor. */
		virtual ~SMSTransformationChainConfig(){}
	protected:
		/** Adds a configuration at the end of the list. */
		virtual void AddConfiguration(const ProcessingConfig& newConcreteConfig,const std::string& className);

	};
	
}//namespace

#endif

