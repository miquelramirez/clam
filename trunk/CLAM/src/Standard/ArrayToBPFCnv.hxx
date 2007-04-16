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

#ifndef _ArrayToBPFCnv_
#define _ArrayToBPFCnv_

#include "BPF.hxx"
#include "DataTypes.hxx"

namespace CLAM {

/*Conversion Routines*/

	//Convert values array (supposed to be equidistant) to BPF
	template <class TX, class TY> 
	void ConvertToBPF(BPFTmpl<TX,TY>& newBPF,const Array<TY>& originalArray)
	{
		CLAM_ASSERT(originalArray.Size()>=1, "Zero lenght array.");
		CLAM_ASSERT(originalArray.Size()==newBPF.Size(), "Different array dimensions");

		for(int i=0;i<originalArray.Size();i++)
		{
			newBPF.SetXValue( i, (TX)i );
			newBPF.SetValue( i,originalArray[i]);
		}
	}

	template <class TX, class TY> 
	void ConvertToBPF( BPFTmpl<TX,TY>& newBPF, TX X0, TX deltaX, const Array<TY>& originalArray )
	{
		CLAM_ASSERT( originalArray.Size() >= 1, 
			     "ArrayToBPFCnv::ConvertToBPF(): Array to be converted into a BPF must have a positive non-zero length!" );

		CLAM_ASSERT(originalArray.Size()==newBPF.Size(), "Different array dimensions on write");
		
		TX currentX = X0;

		for ( int i = 0; i < originalArray.Size(); i++ )
		{
			newBPF.SetXValue( i, currentX );
			newBPF.SetValue( i, originalArray[i] );
			currentX += deltaX;
		}

	}

	//Convert X and Y values arrays to BPF
	template <class TX, class TY> 
	void ConvertToBPF(BPFTmpl<TX,TY>& newBPF,const Array<TX>& originalXArray,
		const Array<TY>& originalYArray)
	{
		CLAM_ASSERT(originalXArray.Size()>=1, "Zero lenght X array.");
		CLAM_ASSERT(originalYArray.Size()>=1, "Zero lenght Y array.");
		CLAM_ASSERT(originalXArray.Size()==originalYArray.Size(), "Different array dimensions for X and Y");
		CLAM_ASSERT(originalXArray.Size()==newBPF.Size(), "Different array dimensions on write");

		for(int i=0;i<originalXArray.Size();i++)
		{	
			newBPF.SetValue(i,originalYArray[i]);
			newBPF.SetXValue(i,originalXArray[i]);
		}
	}
	
	//Convert BPF to X and Y values arrays
	template <class TX, class TY> 
	void ConvertToArray(const BPFTmpl<TX,TY>& originalBPF,Array<TX>& 
		newXArray,Array<TY>& newYArray)
	{
		for(int i=0;i<originalBPF.Size();i++)
		{
			newXArray.AddElem(originalBPF.GetXValue(i));
			newYArray.AddElem(originalBPF.GetValueFromIndex(i));
		}
	}
	
	/*Convert BPF to values array (points are supposed to be equidistant in the 
	X axis)*/
	template <class TX, class TY> 
	void ConvertToArray(const BPFTmpl<TX,TY>& originalBPF,
		Array<TY>& newArray)
	{
		for(int i=0;i<originalBPF.Size();i++)
		{
			 newArray.AddElem(originalBPF.GetValueFromIndex(i));
		}
	}


}

#endif

