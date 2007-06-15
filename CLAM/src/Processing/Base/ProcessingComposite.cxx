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

#include "Assert.hxx"
#include "ProcessingComposite.hxx"

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
			if ((*obj)->IsRunning())
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
	}

	void ProcessingComposite::Insert(Processing& obj) throw(ErrProcessingObj)
	{
		iterator it;
		for (it=mObjects.begin(); it!=mObjects.end(); it++)
			if((*it)==&obj) return;				
		mObjects.push_back(&obj);
	}
}

