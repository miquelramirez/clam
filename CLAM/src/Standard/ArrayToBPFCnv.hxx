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

#ifndef _ArrayToBPFCnv_
#define _ArrayToBPFCnv_

#include "BPF.hxx"
#include "DataTypes.hxx"

namespace CLAM {

template <class TX, class TY> class ArrayToBPFCnv
{
public:
/*Constructors*/
	ArrayToBPFCnv()
	{
	}
	
/*Conversion Routines*/

	//Convert values array (supposed to be equidistant) to BPF
	void ConvertToBPF(BPFTmpl<TX,TY>& newBPF,const Array<TY>& originalArray) const
	{
		int i;
		if((originalArray.Size()<1) || (originalArray.Size()!=newBPF.Size()))
		{
			throw Err("Inconsistent size in original array dimensions. By ArrayToBPFCnv");
		}
		for(i=0;i<originalArray.Size();i++)
		{
			newBPF.SetValue((TX)i,originalArray[i]);
		}
	}

	//Convert X and Y values arrays to BPF
	void ConvertToBPF(BPFTmpl<TX,TY>& newBPF,const Array<TX>& originalXArray,
		const Array<TY>& originalYArray) const
	{
		int i;
		if((originalXArray.Size()<1)||(originalYArray.Size()<1)||(originalXArray.Size() != originalYArray.Size()) || (newBPF.Size() != originalXArray.Size()) )
		{
			throw Err("Inconsistent size in original array dimensions. By ArrayToBPFCnv");
		}
		for(i=0;i<originalXArray.Size();i++)
		{	
			newBPF.SetValue(i,originalYArray[i]);
			newBPF.SetXValue(i,originalXArray[i]);
		 }
	}
	
	//Convert BPF to X and Y values arrays
	void ConvertToArray(const BPFTmpl<TX,TY>& originalBPF,Array<TX>& 
		newXArray,Array<TY>& newYArray) const
	{
		int i;
		for(i=0;i<originalBPF.Size();i++)
		{
			newXArray.AddElem(originalBPF.GetXValue(i));
			newYArray.AddElem(originalBPF.GetValueFromIndex(i));
		}
	}
	
	/*Convert BPF to values array (points are supposed to be equidistant in the 
	X axis)*/
	void ConvertToArray(const BPFTmpl<TX,TY>& originalBPF,
		Array<TY>& newArray)
	{
		int i;
		for(i=0;i<originalBPF.Size();i++)
		{
			 newYArray.AddElem(originalBPF.GetValueFromIndex(i));
		}
	}
};

typedef ArrayToBPFCnv<TData,TData> ArrayToBPFCnv_;

}

#endif
