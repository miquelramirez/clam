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
#include "ProcessingFactory.hxx"
#include "XMLAdapter.hxx"
#include "XmlStorageErr.hxx"

namespace CLAM
{
	ProcessingDefinitionAdapter::ProcessingDefinitionAdapter( Processing * adaptee, const std::string & name, const std::string & position, const std::string & size )
		: mAdaptee(adaptee)
		, mConfiguration(0)
		, mName(name)
		, mPosition(position)
		, mSize(size)
	{
	}

	ProcessingDefinitionAdapter::~ProcessingDefinitionAdapter()
	{
		if (mConfiguration) delete mConfiguration;
	}

	void ProcessingDefinitionAdapter::StoreOn (Storage & store) const
	{
		Text nameCopy(mName);
		XMLAdapter<Text> nameAdapter( nameCopy, "id");
		Text className(mAdaptee->GetClassName());
		XMLAdapter<Text> classNameAdapter( className, "type");
		store.Store(nameAdapter);
		store.Store(classNameAdapter);
		if (mPosition!="") // check if position is defined (QPoint values are integers)
		{
			Text positionCopy(mPosition);
			XMLAdapter<Text> positionAdapter (positionCopy, "position");
			store.Store(positionAdapter);
		}
		if (mSize!="") // check if size is defined
		{
			Text sizeCopy(mSize);
			XMLAdapter<Text> sizeAdapter (sizeCopy, "size");
			store.Store(sizeAdapter);
		}
		XMLComponentAdapter configAdapter((ProcessingConfig&)mAdaptee->GetConfig());
		store.Store(configAdapter);
	}

	void ProcessingDefinitionAdapter::LoadFrom (Storage & store) 
	{
		XMLAdapter<Text> nameAdapter( mName, "id");
		if (!store.Load(nameAdapter))
			throw XmlStorageErr("Processing without id attribute");
		Text className;
		XMLAdapter<Text> classNameAdapter( className, "type");
		if (!store.Load(classNameAdapter))
			throw XmlStorageErr("Processing without type attribute");

		XMLAdapter<Text> positionAdapter (mPosition, "position");
		store.Load(positionAdapter);
		XMLAdapter<Text> sizeAdapter (mSize, "size");
		store.Load(sizeAdapter);

		try
		{
			mAdaptee = ProcessingFactory::GetInstance().CreateSafe(className);
		} catch (ErrFactory & e)
		{
			std::string msg = 
				"Error creating a processing object of type '" +
				className + "' which is not available.";
			throw XmlStorageErr(msg);
		}
		Component * cfg = mAdaptee->GetConfig().DeepCopy();
		XMLComponentAdapter configAdapter( *cfg );
		store.Load(configAdapter);
		mConfiguration = dynamic_cast<ProcessingConfig *>(cfg);
		CLAM_ASSERT(mConfiguration, "Retrieved config fails to cast as a ProcessingConfig");
	}
} // namespace CLAM

