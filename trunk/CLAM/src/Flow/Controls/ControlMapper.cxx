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

#include "ControlMapper.hxx"
 
using namespace CLAM;

void ControlMapperConfig::DefaultInit(void)
{
	AddName();
	AddMapping();
	AddArguments();
	
	UpdateData();
}


ControlMapper::ControlMapper()
:mInput("input",this,&ControlMapper::DoMap)
,mOutput("output",this)
{
	mMappingPtr = 0;
	ControlMapperConfig cfg;
	Configure(cfg);
}

ControlMapper::ControlMapper(const ControlMapperConfig& cfg)
:mInput("input",this,&ControlMapper::DoMap)
,mOutput("output",this)
{
	mMappingPtr = 0;
	Configure(cfg);
}

bool ControlMapper::ConcreteConfigure( const ProcessingConfig& cfg )
{
	mConfig = dynamic_cast<const ControlMapperConfig&>(cfg);

	if (mMappingPtr) delete mMappingPtr;
	mMappingPtr = Mapping::Create(mConfig.GetMapping());
	if (mMappingPtr)
	{
		mMappingPtr->Set(mConfig.GetArguments());
	}
	return true;
}
