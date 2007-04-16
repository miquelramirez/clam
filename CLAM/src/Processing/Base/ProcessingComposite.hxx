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

#ifndef _PROCESSINGOBJECTCOMPOSITE
#define _PROCESSINGOBJECTCOMPOSITE

#include "Processing.hxx"
#include <list>
#include <set>
#include <map>

namespace CLAM {

/** Abstract class for processing object agregates. */
	class ProcessingComposite : public Processing
	{
		std::list<Processing*> mObjects;

	protected:

		virtual bool ConcreteConfigure(const ProcessingConfig&) = 0;

		/**
		 * Concrete start implementation, called when the user calls 
		 * Start() on the Processing(Composite), should be used to implement 
		 * any specific start implementation required by classes deriving from 
		 * this class. When overriding this function in a class derived from 
		 * ProcessingComposite be sure to call the base class (so child Processings 
		 * are properly started).
		 */
		virtual bool ConcreteStart();
		/**
		 * See ConcreteStart().
		 */
		virtual bool ConcreteStop();

	public:

		// Processing Object interface.

		virtual ~ProcessingComposite() {}

		virtual bool Do(void)=0;

		virtual const ProcessingConfig &GetConfig() const = 0;

		virtual const char *GetClassName() const {return "Composite";}

		// Composite interface.

		void Insert(Processing& o) throw(ErrProcessingObj);

		void Remove(Processing& o);

		/** Processing object composite iterator */
		typedef std::list<Processing*>::iterator iterator;

		iterator composite_begin() {return mObjects.begin();}

		iterator composite_end() {return mObjects.end();}

		std::size_t composite_size() {return mObjects.size();}

	};

}

#endif

