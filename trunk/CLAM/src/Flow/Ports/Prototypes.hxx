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


#ifndef _PROTOTYPES_
#define _PROTOTYPES_

#include "ProcessingData.hxx"
#include<list>

namespace CLAM {

	/** This class holds a set of valid prototypes for a port
	 */
	class Prototypes {
		typedef std::list<ProcessingData*> ProtoSet;
		ProtoSet mProtos;
	public:
		/** Add a new prototype to the set of valid ones.  A pointer
		 * to the argument is stored, so the life of the argument must
		 * be greater than the life of the port.
		 */
		void AddPrototype(ProcessingData &proto);
		/** Check if a given prototype is valid */
		bool Check(const ProcessingData &proto);
	};

}

#endif
