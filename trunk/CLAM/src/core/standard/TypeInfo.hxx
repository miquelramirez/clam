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

#ifndef _TYPEINFO_H_
#define _TYPEINFO_H_

//class StaticTrue {};
//class StaticFalse {};
#include "StaticBool.hxx"

namespace CLAM {
	bool SameType(const std::type_info & type1, const std::type_info & type2);

	class DefaultTypeInfo {
	public:
		typedef StaticFalse StorableAsLeaf;
		typedef StaticFalse IsComponent;
		typedef StaticFalse IsIterable;
	};

	/** C native types TypeInfo base class */
	class BasicCTypeInfo : public DefaultTypeInfo {
	public:
		typedef StaticTrue StorableAsLeaf;
	};

	class ContainerTypeInfo : public DefaultTypeInfo {
	public:
		typedef StaticTrue IsIterable;
	};

	// Default TypeInfo definition
	template <typename T> class TypeInfo : public DefaultTypeInfo {};

	// A short for defining a class belonging to a TypeInfo group
#define CLAM_TYPEINFOGROUP(GROUP, TYPE) \
	template <> class TypeInfo< TYPE > : public GROUP  {};

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, char);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, unsigned char);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, signed char);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, const char);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, const unsigned char);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, const signed char);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, char *);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, unsigned char *);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, signed char *);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, const char *);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, const unsigned char *);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, const signed char *);

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, void *);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, short int);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, short unsigned int);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, int);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, unsigned int);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, long);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, unsigned long);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, bool);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, double);
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, float);

}


#endif//_TYPEINFO_H_


