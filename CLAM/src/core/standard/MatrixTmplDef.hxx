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


#ifndef _MatrixTmplDef_
#define _MatrixTmplDef_

namespace CLAM
{

	template <class T>
	MatrixTmpl<T>::MatrixTmpl()
	{
		mpMatrixBuffer=new Array<T>;
		mNumRows = 0;
		mNumColumns = 0;
		mpMatrixBuffer->SetSize(0);
	}

	template <class T>
	MatrixTmpl<T>::~MatrixTmpl()
	{
		if(mpMatrixBuffer)
		{
			delete mpMatrixBuffer;
			mpMatrixBuffer=NULL;
		}
	}


	template <class T>
	MatrixTmpl<T>::MatrixTmpl(unsigned int dim1, unsigned int dim2)
	{
		mpMatrixBuffer=new Array<T>(dim1*dim2);
		mNumRows = dim1;
		mNumColumns = dim2;
		mpMatrixBuffer->SetSize(mNumRows*mNumColumns);
	}

	template <class T>
	MatrixTmpl<T>::MatrixTmpl(const MatrixTmpl<T>& originalMatrix)
	{
		mpMatrixBuffer=new Array<T> (originalMatrix.GetNumElements());
		*this = originalMatrix; 
	}

	// Print the matrix
	template <class T>
	void MatrixTmpl<T>::Print() const
	{
		for (unsigned int i=0; i<mNumRows; i++)
		{
			for (unsigned int j=0; j<mNumColumns; j++)
				{
					std::cout.width(10L);
					std::cout << (*this)(i,j) << " ";
					std::cout.fill();
				} 
			std::cout << std::endl;
		}
	}


	template <class T>
	inline std::istream& operator >> (std::istream & stream, MatrixTmpl<T> & a)
	{
		// @todo
		return stream;
	}
	
	template <class T>
	inline std::ostream& operator << (std::ostream & stream, const MatrixTmpl<T> & a)
	{
		// @todo
		return stream;
	}

} // namespace CLAM


#endif // _MatrixTmplDef_

