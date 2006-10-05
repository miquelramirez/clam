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
	: _input("Audio Input",this)
	, _output("Pitch Profile",this)
	, _implementation(new Simac::ChordExtractor )
{
	Configure( cfg );
}

TonalAnalysis::~TonalAnalysis()
{
	delete _implementation;
}

bool TonalAnalysis::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(_config, c);
	_input.SetSize( _implementation->frameSize() );
	_input.SetHop( _implementation->hop() );
	_floatBuffer.resize(_implementation->frameSize());
	return true;
}

bool TonalAnalysis::Do()
{
	if( !AbleToExecute() ) return true;
	CLAM::TData * input = &(_input.GetAudio().GetBuffer()[0]);
	for (unsigned i = 0; i < _implementation->frameSize(); i++)
		_floatBuffer[i] = input[i];
	_implementation->doIt(&_floatBuffer[0]);
	std::vector<TData> & output = _output.GetData();
	output.resize(_implementation->pcp().size());
	for (unsigned i = 0; i < _implementation->pcp().size(); i++)
		output[i] = _implementation->pcp()[i];

	_input.Consume();
	_output.Produce();
	return true;
}



} // namespace CLAM



