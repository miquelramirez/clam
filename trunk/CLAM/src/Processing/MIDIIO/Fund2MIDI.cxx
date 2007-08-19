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

#include "Fund2MIDI.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "Fund2MIDI",
		"category", "MIDI",
		"description", "Fund2MIDI",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, Fund2MIDI> regFund2MIDI("Fund2MIDI");
	static FactoryRegistrator<ProcessingFactory, Fund2MIDI> reg = metadata;
}

bool Fund2MIDI::Do( const Fundamental& inFund ) 
{
	TControlData fund_note = Round( 69 + log(inFund.GetFreq(0)/440.)*17.31234 );

	//FIXME clipping if necessary (testing), check inFund.GetFreq(0)
	if (fund_note > 127 | fund_note < 0 )
	{
		std::cout << "value clipped! - old fund_note: " << fund_note ;
		fund_note = CLAM_min( CLAM_max(fund_note,(TControlData)0), (TControlData)127 );
		std::cout << " - new fund_note: " << fund_note << std::endl;
	}
	CLAM_DEBUG_ASSERT( fund_note>=0 & fund_note<128, "Fundamental MIDI note should be betweeen 0..127");

	mFreqControlOut.SendControl( fund_note );
	return true;
}


}

