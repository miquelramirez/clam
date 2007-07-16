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
#include "ChordExtractor.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, TonalAnalysis> regTonalAnalysis("TonalAnalysis");
}
	

// TonalAnalysisConfig method definition
void TonalAnalysisConfig::DefaultInit(void)
{
	AddAll();

	UpdateData();

	SetFilterInertia(0.7);
	SetTunningEnabled(true);
	SetPeakWindowingEnabled(true);
	SetHopRatio(8.0);
}


TonalAnalysis::TonalAnalysis( const TonalAnalysisConfig& cfg )
	: _input("Audio Input",this)
	, _pcp("Pitch Profile",this)
	, _chordCorrelation("Chord Correlation",this)
	, _chromaPeaks("Chroma Peaks",this)
	, _tunning("Tunning",this)
	, _implementation( 0 )
{
	Configure( cfg );
}

#ifdef _MSC_VER
TonalAnalysis::TonalAnalysis( bool callThisAsFakeConstructorToBeCalledToAvoidWindowsInstantiationBug )
{
}
#endif

TonalAnalysis::~TonalAnalysis()
{
	if (_implementation) delete _implementation;
}

bool TonalAnalysis::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(_config, c);

	if (_implementation) delete _implementation;
	_implementation = new Simac::ChordExtractor;

	_implementation->filterInertia( _config.GetFilterInertia() );
	_implementation->enableTunning( _config.GetTunningEnabled() );
	_implementation->enablePeakWindowing( _config.GetPeakWindowingEnabled() );
	_implementation->hopRatio( _config.GetHopRatio() );
	
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

	std::vector<TData> & pcp = _pcp.GetData();
	pcp.resize(_implementation->pcp().size());
	for (unsigned i = 0; i < _implementation->pcp().size(); i++)
		pcp[i] = _implementation->pcp()[i];
	_pcp.Produce();

	std::vector<TData> & chordCorrelation = _chordCorrelation.GetData();
	chordCorrelation.resize(_implementation->chordCorrelation().size());
	for (unsigned i = 0; i < _implementation->chordCorrelation().size(); i++)
		chordCorrelation[i] = _implementation->chordCorrelation()[i];
	_chordCorrelation.Produce();

	std::vector<std::pair<TData,TData> > & chromaPeaks = _chromaPeaks.GetData();
	chromaPeaks.resize(_implementation->peaks().size()); //TODO processing time resize!!!!
	for (unsigned i = 0; i < _implementation->peaks().size(); i++)
		chromaPeaks[i] = _implementation->peaks()[i];
	_chromaPeaks.Produce();

	std::pair<TData,TData> & tunning = _tunning.GetData();
	tunning = _implementation->instantTunning();
//	tunning.first=_implementation->tunning();
//	tunning.second=_implementation->tunningStrength();
	_tunning.Produce();

	_input.Consume();
	return true;
}



} // namespace CLAM



