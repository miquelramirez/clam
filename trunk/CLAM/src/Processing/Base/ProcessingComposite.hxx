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

#ifndef _PROCESSINGOBJECTCOMPOSITE
#define _PROCESSINGOBJECTCOMPOSITE

#include "Processing.hxx"
#include <list>
#include <set>
#include <map>

namespace CLAM {


	/** This is an internal class which olds the processing object
	 * name table. Its main pourpose is avoiding that two processing
	 * objects share the same name. It can also be used to generate
	 * default names given a name prefix.  */
	class NameTable {
	
		typedef std::set<std::string> NameSet;
		typedef std::map<std::string,int> PrefixMap;
		NameSet PONameTable;
		PrefixMap POCountTable;

	public:
		NameTable();
		/** Error class */
		class DuplicatedName { public: DuplicatedName(std::string){}};

		/** Singleton accessor to the Processing Object name table instance */
		static NameTable &GetNameTable();

		/** This methods checks if a name has been previously used by a processing object.
		 * @param the name of the processing object to check.
		 * @return true if the name has been used, false if not.
		 */
		bool Exists(const std::string &name);

		/** This method registers a name as being used by a processing object.
		 * @param the name of the processing object.
		 */
		void Add(const std::string &name) throw(DuplicatedName);

		/** This method registers as unused a name that was previously
		 *  used by a processing object. 
		 */
		void Remove(const std::string &name);

		/** This method creates a new unused name using the given prefix,
		 * and registers it as being used by a processing object.
		 * @param character string to be used as the starting characters
		 * of the new name.
		 * @return the new generated name.
		 */
		std::string GenerateFromPrefix(const std::string &prefix);
	};


/** Abstract class for processing object agregates. */
	class ProcessingComposite : public Processing
	{

		NameTable mNames;

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

		std::string InsertAndGiveName(Processing&);

		/**
		 * Changes the name inside the composite for a child
		 * @param child the child process you want to rename
		 * @param old_name the old name you dislike
		 * @pre child should belong to the composite.
		 * @pre old_name is the previous name
		 * @return false when the name change is not possible
		 * because there is another processing with the same name.
		 */
		bool NameChanged(Processing&child,const std::string &old_name);

		void Remove(Processing& o);

		iterator composite_begin() {return mObjects.begin();}

		iterator composite_end() {return mObjects.end();}

		std::size_t composite_size() {return mObjects.size();}

	};

}

#endif
