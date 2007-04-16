/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "ProcessingDefinitionAdapter.hxx"
#include "Assert.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Factory.hxx"
#include "XMLAdapter.hxx"
#include "XmlStorageErr.hxx"
#ifdef USE_LADSPA
#	include "LadspaFactory.hxx"
#endif

namespace CLAM
{

	typedef Factory<CLAM::Processing> ProcessingFactory;

	ProcessingDefinitionAdapter::ProcessingDefinitionAdapter( Processing * adaptee, const std::string & name )
		:  mAdaptee(adaptee), mName(name)
	{
	}

	ProcessingDefinitionAdapter::~ProcessingDefinitionAdapter()
	{
	}

	void ProcessingDefinitionAdapter::StoreOn (Storage & store) const
	{
		Text nameCopy(mName);
		XMLAdapter<Text> nameAdapter( nameCopy, "id");
		Text className(mAdaptee->GetClassName());
		XMLAdapter<Text> classNameAdapter( className, "type");
		store.Store(nameAdapter);
		store.Store(classNameAdapter);

		XMLComponentAdapter configAdapter((ProcessingConfig&)mAdaptee->GetConfig());
		store.Store(configAdapter);
	}

	void ProcessingDefinitionAdapter::LoadFrom (Storage & store) 
	{	
		XMLAdapter<Text> nameAdapter( mName, "id");
		store.Load(nameAdapter);
		Text className("");
		XMLAdapter<Text> classNameAdapter( className, "type");
		store.Load(classNameAdapter);

		try
		{
#ifdef USE_LADSPA //TODO move conditional code inside LadspaFactory
			try
			{
				mAdaptee = ProcessingFactory::GetInstance().CreateSafe( className  );
			}
			catch (ErrFactory&)
			{
				mAdaptee = LadspaFactory::GetInstance().CreateSafe( className );
			}
#else
			mAdaptee = ProcessingFactory::GetInstance().CreateSafe(className);
#endif	

		} catch (ErrFactory & e)
		{
			std::string msg = 
				"Error creating a processing object of type '" +
				className + "' which is not available.";
			throw XmlStorageErr(msg);
		}
		ProcessingConfig&  cfg = (ProcessingConfig&)mAdaptee->GetConfig();
		XMLComponentAdapter configAdapter( cfg );
		store.Load(configAdapter);
		mAdaptee->Configure(cfg);
	}
} // namespace CLAM

