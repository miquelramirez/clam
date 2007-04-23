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

#ifndef _FILENAME_H_
#define _FILENAME_H_
#include <exception>
#include <iosfwd>
#include "Text.hxx"
#include "Assert.hxx"
#include "Component.hxx"
#include "TypeInfo.hxx"
#include <string>

namespace CLAM {

/**
 * This class is a base class for configuration fields representing file names.
 * By deriving from Text you have most of the std::string operations available
 * and input serialization supporting spaces on reading.
 * 
 * Use InFilename and OutFilename subclasses to discriminate the serialization
 * direction
 * and subclass them to specify concrete filters and file type family names.
 */
class Filename : public Text
{
public:
		/// Item of a Filename filter definition
		struct Filter {
			const char * description;
			const char * wildcard;
		};
		Filename() {}
		Filename(const std::string & s) : Text(s) {} 
		Filename(const char * s) : Text(s) {}
		virtual ~Filename() {}

		/// Rewrite this method to set a name for the type family name.
		/// It is used to build messages such as 'Open the audio file'
		/// where 'audio' is the type family.
		virtual const char * TypeFamily() const { return ""; }
		/// Rewrite this method to define the set of filters that subclasses
		/// bounds to. See the examples on how to return it.
		/// Last item should be {0,0} for class users to stop iterating.
		virtual const Filter * Filters() const {
			static const Filter filters[] = {
				{0, 0}
			}; 
			return filters;
		}
};

/**
 * Input file name configuration field.
 *
 * Common operations on std::string are available to this field.
 * Specialize this class for concrete input file formats
 * by redefining Filename::Filter and Filename::TypeFamily.
 *
@code
class InImageFilename : public InFilename
{
public:
	InImageFilename(const std::string & s="") : InFilename(s) {}
	InImageFilename(const char * s) : InFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Portable Network Graphics", "*.png"},
			{"JPEG Image", "*.jpg"},
			{"Scalable Vector Graphics", "*.svg"},
			{0,0}
		};
		return filters;
	}
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, InImageFilename);
@endcode
@see OutFilename
 */

class InFilename : public Filename
{
public:
		InFilename(const std::string & s="") : Filename(s) {} 
		InFilename(const char * s) : Filename(s) {}
};

/**
 * Output file name configuration field.
 *
 * Common operations on std::string are available to this field.
 * Specialize this class for concrete input file formats
 * by redefining Filename::Filter and Filename::TypeFamily.
 *
@code
class OutImageFilename : public OutFilename
{
public:
	OutImageFilename(const std::string & s="") : OutFilename(s) {}
	OutImageFilename(const char * s) : OutFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Portable Network Graphics", "*.png"},
			{"JPEG Image", "*.jpg"},
			{0,0}
		};
		return filters;
	}
};
CLAM_TYPEINFOGROUP(BasicCTypeOutfo, OutImageFilename);
@endcode
@see InFilename
 */

class OutFilename : public Filename
{
public:
		OutFilename(const std::string & s="") : Filename(s) {} 
		OutFilename(const char * s) : Filename(s) {}
};

CLAM_TYPEINFOGROUP(BasicCTypeInfo, Filename);
CLAM_TYPEINFOGROUP(BasicCTypeInfo, InFilename);
CLAM_TYPEINFOGROUP(BasicCTypeInfo, OutFilename);

}

#endif // _FILENAME_H_

