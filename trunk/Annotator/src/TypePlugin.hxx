/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _TypePlugin_hxx_
#define _TypePlugin_hxx_

#include <string>
#include <iosfwd>

namespace CLAM { class DescriptionDataPool; }
namespace CLAM { class DescriptionScheme; }
namespace CLAM_Annotator { class SchemaAttribute; }

namespace CLAM_Annotator
{
	/**
	 * Abstract class to Pool data type plugins.
	 * It implements type dependant actions that must be taken in a Pool.
	 */
	class TypePlugin
	{
	public:
		TypePlugin(const SchemaAttribute & scheme);
		virtual ~TypePlugin();
		virtual void AddTo(CLAM::DescriptionScheme & scheme) = 0;
		virtual bool ValidateData(const CLAM::DescriptionDataPool & dataPool, std::ostream & err) = 0;
		virtual void InitInstance(unsigned instance, CLAM::DescriptionDataPool & pool)=0;
		static TypePlugin * Create(const SchemaAttribute & scheme);
	protected:
		const SchemaAttribute & mSchema;
	};

}

#endif//_TypePlugin_hxx_

