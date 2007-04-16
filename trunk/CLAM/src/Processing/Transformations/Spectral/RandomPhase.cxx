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

#include "RandomPhase.hxx"
#include "Factory.hxx"

namespace CLAM
{


bool RandomPhase::Do(const Spectrum& in, Spectrum& out)
{
	srand(time(0));

	out = in; 

	float amount = mAmount.GetLastValue() * 0.01;
	float invAmount = 1. - amount;
	DataArray& oPhase = out.GetPhaseBuffer();
	
	int spectrumSize = in.GetSize();
	
	int k = TWO_PI / RAND_MAX;
	for(int i = 0; i<spectrumSize; i++)
	{
		oPhase[i] = oPhase[i]*invAmount + rand()* k * amount;
	}
	return true;
}

typedef Factory<Processing> ProcessingFactory;

static ProcessingFactory::Registrator<RandomPhase> regtRandomPhase( "RandomPhase" );

}

