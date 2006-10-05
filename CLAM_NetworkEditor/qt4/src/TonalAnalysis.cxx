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

#include "TonalAnalysis.hxx"
#include <CLAM/Factory.hxx>
#include <ChordExtractor.hxx>


namespace CLAM
{
	namespace detail
	{
		static Factory<Processing>::Registrator<TonalAnalysis> regtTonalAnalysis( "TonalAnalysis" );
	}
	

// TonalAnalysisConfig method definition
void TonalAnalysisConfig::DefaultInit(void)
{
}


TonalAnalysis::TonalAnalysis( const TonalAnalysisConfig& cfg )
	: mInput("Audio Input",this)
	, mOutput("Pitch Profile",this)
	, mImplementation(new Simac::ChordExtractor )
{
	Configure( cfg );
}

TonalAnalysis::~TonalAnalysis()
{
	delete mImplementation;
}

bool TonalAnalysis::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);
	return true;
}

bool TonalAnalysis::Do()
{
	if( !AbleToExecute() ) return true;
//	mOutput.GetData() = _lastValues;
	mInput.Consume();
	mOutput.Produce();
	return true;
}



} // namespace CLAM



