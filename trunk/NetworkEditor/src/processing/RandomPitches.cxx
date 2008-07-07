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

#include "RandomPitches.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "RandomPitches",
	//	"category", "CLAM",
	//	"description", "RandomPitches",
		0
	};
	static FactoryRegistrator<ProcessingFactory, RandomPitches> reg = metadata;
}
	

// RandomPitchesConfig method definition
void RandomPitchesConfig::DefaultInit(void)
{
}


// RandomPitches method definition
RandomPitches::RandomPitches()
	: mInput("Audio Input",this)
	, mOutput("Pitch Profile",this)
{
	RandomPitchesConfig cfg;
	Configure( cfg );
}

RandomPitches::RandomPitches( const RandomPitchesConfig& cfg )
	: mInput("Audio Input",this)
	, mOutput("Pitch Profile",this)
{
	Configure( cfg );
}

RandomPitches::~RandomPitches()
{
}

bool RandomPitches::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);
	_lastValues.resize(12);
	return true;
}

bool RandomPitches::Do()
{
//	if( !AbleToExecute() ) return true;
	for (unsigned i=0; i<_lastValues.size(); i++)
	{
		_lastValues[i] += std::rand();
		_lastValues[i] -= std::rand();
	}
	mOutput.GetData() = _lastValues;
	mInput.Consume();
	mOutput.Produce();
	return true;
}



} // namespace CLAM



