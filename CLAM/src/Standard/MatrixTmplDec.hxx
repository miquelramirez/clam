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


#ifndef _MatrixTmplDec_
#define _MatrixTmplDec_

#include <iosfwd>
#include "Array.hxx"

namespace CLAM
{

	template <class T>
	class MatrixTmpl
	{
	public:
		MatrixTmpl();
		MatrixTmpl(unsigned int dim1, unsigned int dim2);
		MatrixTmpl(const MatrixTmpl<T>& originalMatrix);
		~MatrixTmpl();
		int GetNumRows() const {return mNumRows;}
		int GetNumColumns() const {return mNumColumns;}
		int GetNumElements() const {return mNumRows*mNumColumns;}
		const Array <T>& GetBuffer() const {return MatrixBuffer();}
		Array <T>& GetBuffer() {return MatrixBuffer();}

		T Sum() const
		{
			T sum=0;
			for (unsigned int i=0; i<(mNumRows * mNumColumns); i++)
				sum += MatrixBuffer()[i];
			return sum;
		}

		T Max() const
		{
			T max = MatrixBuffer()[0];
			for (unsigned int i=1; i<(mNumRows*mNumColumns); i++) 
				if (MatrixBuffer()[i] > max)
					max = MatrixBuffer()[i];
			return max;
		}

		T Min() const
		{
			T min = MatrixBuffer()[0];
			for (unsigned int i=1; i<(mNumRows*mNumColumns); i++) 
				if (MatrixBuffer()[i] < min)
					min = MatrixBuffer()[i];
			return min;
		}

		float Mean() const {return (float)(Sum())/(mNumRows*mNumColumns);}

		// Print the matrix
		void Print() const;

	 // Determinant
		float GetDet() const
		{
			if(mNumRows != mNumColumns)
				throw Err("Determinant only valid for square matrices");
			float sum = 0;

			if (mNumColumns == 1)
				return (*this)(0,0);

			else {
				for (unsigned int i=0; i< mNumColumns;i++) // For the first row
					{
						sum += MatrixBuffer()[i] * pow((TData)-1,(TData)i) * (GetSubmatrix(0,i).GetDet());
					}
				// i+1 : the matrix index are from [0..N-1] 
				return sum;
			}
		}	

		// Get Transposed Matrix
		MatrixTmpl<T> GetTrans()
		{
			MatrixTmpl<T> ret(mNumColumns, mNumRows);
			for (unsigned int i=0; i<mNumColumns; i++)
				for(unsigned int j=0; j<mNumRows; j++)
					ret(i,j) = (*this)(j,i);
			return ret;
		}

		// Transpose the matrix
		void Trans()
		{
			MatrixTmpl<T> ret(mNumColumns, mNumRows);
			for (unsigned int i=0; i<mNumColumns; i++)
				for(unsigned int j=0; j<mNumRows; j++)
					ret(i,j) = (*this)(j,i);
			(*this) = ret;
		}
		// Reset the matrix (set all the elements to 0)
		void Reset()
		{
			MatrixTmpl<T> ret(mNumRows, mNumColumns);
			for (unsigned int i=0; i<mNumRows; i++)
				for(unsigned int j=0; j<mNumColumns; j++)
		ret(i,j) = 0;
			(*this) = ret;
		}


		// Invert the matrix
		void Invert()
		{
			if (mNumRows != mNumColumns)
				throw Err("Inverse can only be calculated for square matrix");
			else
			{
				MatrixTmpl<T> ret(mNumRows, mNumColumns);
				MatrixTmpl<T> aux(mNumRows-1, mNumColumns-1);
				for (unsigned int i=0; i<mNumRows; i++)
					for (unsigned int j=0; j<mNumRows; j++)
						{
							aux = GetSubmatrix(i,j);
							ret(i,j) = pow((TData)-1,(TData)i+j) * aux.GetDet();
						}
				ret = (1 / GetDet()) * (ret.GetTrans());
				(*this) = ret;
			} 
		}

		// Get the inverse of a matrix
		MatrixTmpl<T> GetInverse() const
		{
			if (mNumRows != mNumColumns)
				throw Err("Inverse can only be calculated for square matrix");
			else
			{
				MatrixTmpl<T> ret(mNumRows, mNumColumns);
				for (unsigned int i=0; i<mNumRows; i++)
					for (unsigned int j=0; j<mNumColumns; j++)
						ret(i,j) = pow((TData)-1,(TData)i+j) * ( GetSubmatrix(i,j).GetDet() );
				ret = (1/GetDet()) * (ret.GetTrans());
				return ret; 
			}
		}

		// Delete a Row and return a new Matrix Oject with reduced dims
		MatrixTmpl<T> GetDelRow(unsigned int row) const
		{
			unsigned int i,j;
			MatrixTmpl<T> ret (mNumRows-1,mNumColumns);

			for (i=0;i<row;i++) // copy unshifted part
			{
				for(j=0;j<mNumColumns;j++)
					ret(i,j) = MatrixBuffer()[i*mNumColumns+j];
			}
			for (i=row;i<mNumRows-1;i++) // shift rest one row up
			{
				for(j=0;j<mNumColumns;j++)
					ret(i,j) = MatrixBuffer()[(i+1)*mNumColumns+j];
			}
			return ret; 
		}

		// Delete Column
		MatrixTmpl<T> GetDelColumn(unsigned int column) const
		{
			unsigned int i,j;
			MatrixTmpl<T> ret (mNumRows,mNumColumns-1);

			for (i=0;i<mNumRows;i++) // shift matrix one column left
			{
				for(j=0;j<column;j++) 
					ret(i,j) = MatrixBuffer()[i*mNumColumns+j];
			}

			for (i=0;i<mNumRows;i++) // shift matrix one column left
			{
				for(j=column;j<mNumColumns-1;j++)
					ret(i,j) = MatrixBuffer()[i*mNumColumns+j+1];
			}
			return ret;
		}

		// Get a Submatrix(i,j) deleting the i row and the j column
		MatrixTmpl<T> GetSubmatrix(unsigned int i, unsigned int j) const
		{
			MatrixTmpl<T> aux( GetDelRow(i) );
			MatrixTmpl<T> ret( aux.GetDelColumn(j) );
			return ret;
		}

		// Convert matrix to his Submatrix(i,j).
		void Submatrix(unsigned int i, unsigned int j)
		{
			MatrixTmpl<T> aux( GetDelRow(i) );
			(*this) = aux.GetDelColumn(j);
		}

		// Solving linear equations [A][x]=[b]
		// friend MatrixTmpl<T> Solve(const MatrixTmpl<T>& A, const MatrixTmpl<T>& b);

		// LU Decomposition
		// Decompose the matrix into a product of lower and upper triangular matrices.
		// Decompose();
		// Compute forward/backward substitution on decomposed LU matrix product
		// Substitution(LU);

		// Eigenvalues and Eigen 
		// friend MatrixTmpl<T> Eigenvalues(const MatrixTmpl<T>& m);
		// friend MatrixTmpl<T> Eigenvectors(const MatrixTmpl<T>& m);

		// Gram Schmidt Orthonormalization
		// friend MatrixTmpl<T> Orth(const MatrixTmpl<T>& m);

		/**** Get/Set an element *****/
		void SetAt (unsigned int iPosition, unsigned int jPosition, T element)
		{
			if( (iPosition >= mNumRows) | (iPosition < 0) | (jPosition >= mNumColumns) | (jPosition < 0))
				throw Err("Index exceed matrix dimension");
			else
				MatrixBuffer()[mNumColumns*iPosition+jPosition] = element;
		}

		T GetAt (unsigned int iPosition, unsigned int jPosition) const
		{
			if( (iPosition >= mNumRows) | (iPosition < 0) | (jPosition >= mNumColumns) | (jPosition < 0))
				throw Err("Index exceed matrix dimension");
			else
				return MatrixBuffer()[mNumColumns*iPosition+jPosition];
		}

		// Get one column
		friend MatrixTmpl<T> GetColumn(unsigned int column, MatrixTmpl<T>& m )
		{
			if ( (column >= m.mNumColumns) | (column < 0))
							throw Err("Index exceed matrix dimension");
			else {
				MatrixTmpl<T> ret(m.mNumRows, 1); // Column vector
				for (unsigned int i=0; i<m.mNumRows; i++)
					ret(i,0) = m(i,column);
				return ret;
			}
		}

		// Get one row
		friend MatrixTmpl<T> GetRow(unsigned int row, MatrixTmpl<T>& m)
		{
			if ( (row >= m.mNumRows) | (row < 0))
							throw Err("Index exceed matrix dimension");
			else {
				MatrixTmpl<T> ret(1, m.mNumColumns); // Row vector
				for (unsigned int i=0; i<m.mNumColumns; i++)
					ret(0,i) = m(row,i);
				return ret;
			}
		}

		/* Apply an unary function to all the elements of the matrix */
		/* sin, cos, sqrt, cbrt, exp, log, log10, asin, acos, tan, atan */
		void Apply( T (*f)(T) )
		{
			for (unsigned int i=0; i<mNumRows; i++)
				for(unsigned int j=0; j<mNumColumns; j++)
					(*this)(i,j) = f( (*this)(i,j) );
		 }

		 void Apply( T (*f)(T,int),int parameter )
		{
			for (unsigned int i=0; i<mNumRows; i++)
				for(unsigned int j=0; j<mNumColumns; j++)
					(*this)(i,j) = f( (*this)(i,j), parameter );
		 }

		friend MatrixTmpl<T> GetApply( const MatrixTmpl<T> &m, double f(double) )
		{
			MatrixTmpl<T> ret(m.mNumRows, m.mNumColumns);
			for (unsigned int i=0; i<m.mNumRows; i++)
				for(unsigned int j=0; j<m.mNumColumns; j++)
					ret(i,j) = T(f( m(i,j) ));
			return ret;
		 }

		friend MatrixTmpl<T> AbsMatrix(const MatrixTmpl<T> &m) // absolute value
		{
			MatrixTmpl<T> ret(m.mNumRows, m.mNumColumns);
			for (unsigned int i=0; i<m.mNumRows; i++)
				for(unsigned int j=0; j<m.mNumColumns; j++)
					ret(i,j) = abs( m(i,j) );
			return ret;
		}

		/**** Operators ****/
		T& operator () (unsigned int iPosition,unsigned	int jPosition) const {
			if( (iPosition >= mNumRows) | (iPosition < 0) | (jPosition >= mNumColumns) | (jPosition < 0))
				throw Err("Index exceed matrix dimension");
			return ( MatrixBuffer()[mNumColumns*iPosition+jPosition] ); 
		}

		const MatrixTmpl<T>& operator = (const MatrixTmpl<T>& originalMatrix)
		{
			// MatrixBuffer() = originalMatrix.MatrixBuffer();
			*mpMatrixBuffer = *(originalMatrix.mpMatrixBuffer); 
			//*mpMatrixBuffer = originalMatrix.MatrixBuffer();
			mNumRows = originalMatrix.mNumRows;
			mNumColumns = originalMatrix.mNumColumns;
			return *this;
		}

		const MatrixTmpl<T>& operator = (const T element)
		{
			MatrixBuffer().SetSize(1);
			MatrixBuffer()[0] = element;
			mNumRows = mNumColumns = 1;
			return *this;
		}

		const MatrixTmpl<T>& operator += (const MatrixTmpl<T>& newMatrix)
		{
			// Adding element by element
			if ( (mNumRows != newMatrix.mNumRows) | (mNumColumns != newMatrix.mNumColumns))
				throw Err("Addition of matrix of different dimensions not permitted"); 
			else {
				for (unsigned int i=0; i< (mNumRows*mNumColumns) ; i++)
					MatrixBuffer()[i] += newMatrix.MatrixBuffer()[i];
				return *this;
			}
		}

		const MatrixTmpl<T>& operator -= (const MatrixTmpl<T>& newMatrix)
		{
			// Adding element by element
			if ( (mNumRows != newMatrix.mNumRows) | (mNumColumns != newMatrix.mNumColumns) )
				throw Err("Substraction of matrix of different dimensions not permitted");	
			else {
				for (unsigned int i=0; i< (mNumRows*mNumColumns) ; i++)
					MatrixBuffer()[i] -= newMatrix.MatrixBuffer()[i];
				return *this;
			} 
		}

		friend MatrixTmpl<T> operator + (MatrixTmpl<T>& m1, MatrixTmpl<T>& m2)
		{
			if ( (m1.mNumRows != m2.mNumRows) | (m1.mNumColumns != m2.mNumColumns) ) 
				throw Err("Addition of matrix of different dimension not permitted");
			else {
				MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns);	// Construction of an Vector object
				for (unsigned int i=0; i<ret.mNumRows; i++)
					for (unsigned int j=0; j<ret.mNumColumns; j++)
						ret(i,j) = m1(i,j) + m2(i,j);
				return ret;
			}
		}

		// add an element to all the matrix elements
		friend MatrixTmpl<T> operator + (MatrixTmpl<T>& m1, const T element)
		{
				MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns); 
				for (unsigned int i=0; i<ret.mNumRows; i++)
					for (unsigned int j=0; j<ret.mNumColumns; j++)
						ret(i,j) = m1(i,j) + element;
				return ret;
		}

		friend MatrixTmpl<T> operator - (MatrixTmpl<T>& m1, MatrixTmpl<T>& m2)
		{
			if ( (m1.mNumRows != m2.mNumRows) | (m1.mNumColumns != m2.mNumColumns) ) 
				throw Err("Substraction of matrix of different dimension not permitted"); 
			else {
				MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns);	// Construction of an Vector object
				for (unsigned int i=0; i<ret.mNumRows; i++)
					for (unsigned int j=0; j<ret.mNumColumns; j++)
						ret(i,j) = m1(i,j) - m2(i,j);
				return ret;
			}
		}

		// substract an element to all the matrix elements
		friend MatrixTmpl<T> operator - (MatrixTmpl<T>& m1, const T element)
		{
			MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns); 
			for (unsigned int i=0; i<ret.mNumRows; i++)
				for (unsigned int j=0; j<ret.mNumColumns; j++)
		ret(i,j) = m1(i,j) - element;
			return ret;
		}

		friend MatrixTmpl<T> operator * (T scalar,const MatrixTmpl<T>& m)
		{
			MatrixTmpl<T> mult(m.mNumRows, m.mNumColumns);
			for (unsigned int i=0; i<(mult.mNumRows*mult.mNumColumns); i++)
				mult.MatrixBuffer()[i] = scalar * m.MatrixBuffer()[i];
			return mult;
		}

		friend MatrixTmpl<T> operator * (const MatrixTmpl<T>& m1, const MatrixTmpl<T>& m2)
		{
			if( m1.mNumColumns != m2.mNumRows)
				throw Err("Multiplication not permitted"); 
			else
			{
				MatrixTmpl<T> ret(m1.mNumRows, m2.mNumColumns);
				for (unsigned int i=0; i<ret.mNumRows; i++)
					for (unsigned int j=0; j<ret.mNumColumns; j++) {
						ret(i,j) = 0;
						for( unsigned int k=0; k<m1.mNumColumns; k++)
							ret(i,j) += m1(i,k)*m2(k,j);
					}
				return ret;
			}
		}

		friend MatrixTmpl<T> operator / (const MatrixTmpl<T>& m, T scalar)
		{
			MatrixTmpl<T> ret(m.mNumRows, m.mNumColumns);
			for (unsigned int i=0; i<(ret.mNumRows*ret.mNumColumns); i++)
				ret.MatrixBuffer()[i] = m.MatrixBuffer()[i] / scalar;
			return ret;
		}

		friend bool operator == (const MatrixTmpl<T>& m1, const MatrixTmpl<T>& m2)
		{
			if ( (m1.mNumRows == m2.mNumRows) && (m1.mNumColumns == m2.mNumColumns) && (m1.MatrixBuffer() ==
	m2.MatrixBuffer()) )
				return true;
			else
				return false; 
		}


		Array <T>& MatrixBuffer() const { return *mpMatrixBuffer;}

	protected:

		// Dimensions
		unsigned int mNumRows;
		unsigned int mNumColumns;

		// Buffer
		Array <T>* mpMatrixBuffer;

	};

	template <class T>
	std::istream& operator >> (std::istream & stream, MatrixTmpl<T> & a);
	
	template <class T>
	std::ostream& operator << (std::ostream & stream, const MatrixTmpl<T> & a);
	
} // namespace CLAM

#endif // _MatrixTmplDec_
