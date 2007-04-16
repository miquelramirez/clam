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

#include "ControlMapper.hxx"
#include "Factory.hxx"
 
namespace CLAM 
{

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

bool ControlMapper::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);

	if (mMappingPtr) delete mMappingPtr;
	mMappingPtr = Mapping::Create(mConfig.GetMapping());
	if (mMappingPtr)
	{
		mMappingPtr->Set(mConfig.GetArguments());
	}
	else 
		{ 
			AddConfigErrorMessage( "Unknown mapping");
			return false;
		}

	return true;
}


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::ControlMapper> regtControlMapper( "ControlMapper" );


} // namespace CLAM

