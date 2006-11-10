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

#include "Schema.hxx"
#include "Pool.hxx"

namespace CLAM_Annotator
{

	void Schema::InitInstance(const std::string & scope, unsigned instance, CLAM::DescriptionDataPool & data) const
	{
		std::list<SchemaAttribute>& attributes = GetAttributes();
		std::list<SchemaAttribute>::iterator attribute;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			if (attribute->GetScope()!=scope) continue;
			attribute->InitInstance(instance, data);
		}
	}
	bool Schema::Validate(const CLAM::DescriptionDataPool& pool, std::ostream & err) const
	{
		const std::list<SchemaAttribute>& attributes = GetAllAttributes();
		std::list<SchemaAttribute>::const_iterator attribute;
		bool validated = true;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			validated = validated && attribute->Validate(pool, err);
		}
		return validated;
	}
	void Schema::InitMissingAttributes(CLAM::DescriptionDataPool& pool) const
	{
		const std::list<SchemaAttribute>& attributes = GetAllAttributes();
		std::list<SchemaAttribute>::const_iterator attribute;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			if (pool.IsInstantiated(attribute->GetScope(), attribute->GetName())) continue;
			unsigned nInstances = pool.GetNumberOfContexts(attribute->GetScope());
			for (unsigned i=0; i<nInstances; i++)
				attribute->InitInstance(i, pool);
		}
	}
}



