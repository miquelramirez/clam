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

#include "SchemaAttribute.hxx"
#include <algorithm>
#include <CLAM/Pool.hxx>
#include <CLAM/DescriptionScheme.hxx>

namespace CLAM_Annotator{

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
		mTypePlugin.t = 0;
	}

	void SchemaAttribute::AddTo(CLAM::DescriptionScheme & scheme) const
	{
		UpdateTypePlugin();

		CLAM_ASSERT(mTypePlugin.t,
			(std::string("Adding an unrecognized type: ")+GetType()).c_str());

		mTypePlugin.t->AddTo(scheme);
	}

	bool SchemaAttribute::Validate(const CLAM::DescriptionDataPool & pool, std::ostream & err) const
	{
		UpdateTypePlugin();
		// TODO: Check also when scope size is > 1
		if (!mTypePlugin.t)
		{
			err 
				<< "Attribute '" << GetScope() << ":" << GetName() 
				<< "' has type '" << GetType() << "' "
				<< "which is not supported by this program."
				<< std::endl;
			return false;
		}
		std::ostringstream subErr;
		if (!pool.IsInstantiated(GetScope(), GetName())) return true;
		if (mTypePlugin.t->ValidateData(pool,subErr)) return true;
		err << "Error validating attribute '" << GetScope() << ":" << GetName() << "':" << std::endl;
		err << subErr.str();
		return false;
	}


};

