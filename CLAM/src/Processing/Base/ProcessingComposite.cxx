/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Assert.hxx"
#include "mtgsstream.h"
#include "ProcessingComposite.hxx"
#include <string>

namespace CLAM {

	bool ProcessingComposite::ConcreteStart()
	{
		iterator obj;
		for (obj=composite_begin(); obj!=composite_end(); obj++)
		{
			try {
				(*obj)->Start();
			}
			catch (Err &e)
			{
				iterator obj2;
				for (obj2 = composite_begin(); obj2 != obj; obj2++)
					(*obj2)->Stop();
				throw e;
			}
			if ((*obj)->GetExecState() != Running)
			{
				iterator obj2;
				for (obj2 = composite_begin(); obj2 != obj; obj2++)
					(*obj2)->Stop();
				throw(ErrProcessingObj("ConcreteStart(): Child failed to start",this));
			}
		}
		return true;
	}

	bool ProcessingComposite::ConcreteStop()
	{
		iterator obj;
		for (obj=composite_begin(); obj!=composite_end(); obj++)
			(*obj)->Stop();
		return true;
	}

	void ProcessingComposite::Remove(Processing& obj)
	{
		iterator it;
		for (it=composite_begin(); it!=composite_end(); it++)
			if ( (*it) == &obj)
				break;
		if (it == composite_end())
			return; // Not found!
		mObjects.remove(&obj);
		mNames.Remove(obj.GetName());

	}

	void ProcessingComposite::Insert(Processing& obj) throw(ErrProcessingObj)
	{
		CLAM_BEGIN_CHECK
		iterator it;
		for (it=mObjects.begin(); it!=mObjects.end(); it++)
			CLAM_ASSERT( ((*it) != &obj) , "ProcessingComposite::Insert():"
			                              "Object already inserted\n");
		CLAM_END_CHECK
		try {
			mNames.Add(obj.GetName());
		}
		catch (NameTable::DuplicatedName)
		{
			throw(ErrProcessingObj("ProcessingComposite::Insert():"
								   " Duplicated Name",&obj));
		}
		mObjects.push_back(&obj);
	}

	std::string ProcessingComposite::InsertAndGiveName(Processing&obj)
	{
		iterator it;
		for (it=mObjects.begin(); it!=mObjects.end(); it++)
			CLAM_ASSERT( ((*it) != &obj) , "ProcessingComposite::InsertAndGiveName():"
			                              "Object already inserted\n");
		const char * className = obj.GetClassName();
		std::string name = mNames.GenerateFromPrefix(className);
		mObjects.push_back(&obj);
		return name;
	}

	bool ProcessingComposite::NameChanged(Processing& obj, const std::string &old_name) 
	{
		CLAM_BEGIN_CHECK
			iterator it;
			for (it=composite_begin(); it!=composite_end(); it++)
				if ( (*it) == &obj)
					break;
			CLAM_ASSERT(it != composite_end(),
				"ProcessingComposite::NameChanged(): Object not in composite");
		CLAM_END_CHECK

		if (obj.GetName() == old_name)
			return true;
		
		try 
		{
			mNames.Add(obj.GetName());
		}
		catch (NameTable::DuplicatedName)
		{
			return false;
		}
		mNames.Remove(old_name);
		return true;
	}

	NameTable::NameTable()
	{
	}		

	bool NameTable::Exists(const std::string &name)
	{
		return (PONameTable.count(name)!=0);
	}

	void NameTable::Add(const std::string &name) throw(DuplicatedName)
	{
		if (Exists(name))
		{
			DuplicatedName err(name);
			throw(err);
		}
		PONameTable.insert(name);
	}

	void NameTable::Remove(const std::string &name)
	{
		PONameTable.erase(name);
	}

	std::string NameTable::GenerateFromPrefix(const std::string &prefix)
	{
		std::stringstream name;

		int postfix;

		PrefixMap::iterator it = POCountTable.find(prefix);
		if ( it == POCountTable.end() )
			postfix = 0;
		else 
			postfix = POCountTable[prefix];

		do { // Someone might have manually used the generated name, so
			// We check, and we increment the postfix until we find a free name.
			name.str("");
			name << prefix;
			name << "_" << postfix;
			postfix++;
		} while ( PONameTable.count(name.str()) );

		POCountTable[prefix] = postfix;
		PONameTable.insert(name.str());

		return name.str();
	}
		
}
