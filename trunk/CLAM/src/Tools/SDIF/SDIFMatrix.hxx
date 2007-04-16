/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _SdifMatrix_hxx_
#define _SdifMatrix_hxx_

#include "SDIFHeader.hxx"

#undef CLAM_USE_XML
#include "Array.hxx"

#include "SDIFType.hxx"

namespace SDIF
{
	/**
	* Contains frame data for an SDIF frame.
	* The data in a Frames are stored in 2D matrices.
	* Each column corresponds to a parameter like frequency or amplitude and
	* each row represents an object like a filter, sinusoid, or noise band.
	* Each Matrix starts with a MatrixHeader, that contains a
	* type id, and the dimensions of the matrix.
	*
	* As Matrixes are stored in a linked list in the Frames, 
	* each Matrix has a pointer to the next matrix inside the
	* frame.
	*/

	class Matrix
	{
	friend class File;
	protected:
		MatrixHeader mHeader;

		Matrix(const MatrixHeader& header)
		:mHeader(header)
		{
		}

		Matrix(
				const TypeId& type = TypeId::sDefault,DataType dataType = eUnknown,
				CLAM::TInt32 nRows = 0, CLAM::TInt32 nColumns = 0);
		virtual char* GetPtr(void) = 0;
		virtual void Resize(int nElems) = 0;
		virtual void SetSize(int nElems) = 0;
	public:
		virtual ~Matrix() { }

		TypeId Type(void) {return mHeader.mType;}

		CLAM::TInt32 Rows(void) { return mHeader.mnRows; }
		CLAM::TInt32 Columns(void) { return mHeader.mnColumns; }
		CLAM::TInt32 SizeInFile(void)
		{
			CLAM::TUInt32 nElems = mHeader.mnColumns*mHeader.mnRows;
			CLAM::TUInt32 elemSize = mHeader.mDataType&0xFF;
			CLAM::TUInt32 size = nElems*elemSize;
			CLAM::TUInt32 padding = 8-size&7;

			return mHeader.SizeInFile()+size+padding;
		}
	};

	template <class T=CLAM::TFloat32> class ConcreteMatrix:public Matrix
	{
	friend class File;
	private:
		CLAM::Array<T> mpData;
	public:
		/** Create a new Matrix.
		* @param type Identifier specifying the matrix type
		* @param dataType Identifier specifying the type of data type in the matrix
		* @param nRows Number of rows in the matrix
		* @param nColumns Number of rows in the matrix
		*/
		ConcreteMatrix(
				const TypeId& type = TypeId::sDefault,
				CLAM::TInt32 nRows = 0, CLAM::TInt32 nColumns = 0)
			: Matrix(type,GetType<T>::Get(),nRows,nColumns)
		{
			CLAM::TInt32 nElems = Rows()*Columns();
			Resize(nElems);
			SetSize(nElems);
		}

		ConcreteMatrix(const MatrixHeader& header)
		:Matrix(header)
		{
			CLAM::TInt32 nElems = Rows()*Columns();
			Resize(nElems);
			SetSize(nElems);
		}

		char* GetPtr(void) { return (char*)mpData.GetPtr(); }
		void Resize(int nElems) { mpData.Resize(nElems); }
		void SetSize(int nElems) { mpData.SetSize(nElems); }


		/** Lookup a value in the matrix
		* @param row,col coordinates of the value
		* @return The indicated value
		*/
		T GetValue(CLAM::TInt32 row,CLAM::TInt32 col)
		{
			return mpData[row*mHeader.mnColumns + col];
		}
		
		/** Set a value in the matrix
		* @param row,col coordinates of the value
		* @param val The new value
		*/
		void SetValue(CLAM::TInt32 row,CLAM::TInt32 col,const T& val)
		{
			mpData[row*mHeader.mnColumns + col] = val;
		}
	};
}

#endif//_SdifMatrix_hxx_

