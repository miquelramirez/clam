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


#ifndef _MatrixTmplDec_
#define _MatrixTmplDec_

#include <iosfwd>
#include "Array.hxx"
#include "CLAM_Math.hxx"

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
			CLAM_ASSERT(mNumRows == mNumColumns,
				"Determinant only valid for square matrices");
			float sum = 0;

			if (mNumColumns == 1)
				return (*this)(0,0);

			for (unsigned int i=0; i< mNumColumns;i++) // For the first row
			{
				sum += MatrixBuffer()[i] * pow((TData)-1,(TData)i) * (GetSubmatrix(0,i).GetDet());
			}
			// i+1 : the matrix index are from [0..N-1] 
			return sum;
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
			CLAM_ASSERT(mNumRows == mNumColumns,
				"Inverse can only be calculated for square matrix");
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

		// Get the inverse of a matrix
		MatrixTmpl<T> GetInverse() const
		{
			CLAM_ASSERT(mNumRows == mNumColumns,
				"Inverse can only be calculated for square matrix");
			MatrixTmpl<T> ret(mNumRows, mNumColumns);
			for (unsigned i=0; i<mNumRows; i++)
				for (unsigned j=0; j<mNumColumns; j++)
					ret(i,j) = pow((TData)-1,(TData)i+j) * ( GetSubmatrix(i,j).GetDet() );
			ret = (1/GetDet()) * (ret.GetTrans());
			return ret; 
		}

		// Delete a Row and return a new Matrix Oject with reduced dims
		MatrixTmpl<T> GetDelRow(unsigned int row) const
		{
			MatrixTmpl<T> ret (mNumRows-1,mNumColumns);

			for (unsigned i=0;i<row;i++) // copy unshifted part
			{
				for(unsigned j=0;j<mNumColumns;j++)
					ret(i,j) = MatrixBuffer()[i*mNumColumns+j];
			}
			for (unsigned i=row;i<mNumRows-1;i++) // shift rest one row up
			{
				for(unsigned j=0;j<mNumColumns;j++)
					ret(i,j) = MatrixBuffer()[(i+1)*mNumColumns+j];
			}
			return ret; 
		}

		// Delete Column
		MatrixTmpl<T> GetDelColumn(unsigned int column) const
		{
			MatrixTmpl<T> ret (mNumRows,mNumColumns-1);

			for (unsigned i=0;i<mNumRows;i++) // shift matrix one column left
			{
				for(unsigned j=0;j<column;j++) 
					ret(i,j) = MatrixBuffer()[i*mNumColumns+j];
			}

			for (unsigned i=0;i<mNumRows;i++) // shift matrix one column left
			{
				for(unsigned j=column;j<mNumColumns-1;j++)
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
			CLAM_ASSERT(iPosition < mNumRows,
				"Index beyond matrix dimension");
			CLAM_ASSERT(iPosition >= 0,
				"Index beyond matrix dimension");
			CLAM_ASSERT(jPosition < mNumColumns,
				"Index beyond matrix dimension");
			CLAM_ASSERT(jPosition >= 0,
				"Index beyond matrix dimension");

			MatrixBuffer()[mNumColumns*iPosition+jPosition] = element;
		}

		T GetAt (unsigned int iPosition, unsigned int jPosition) const
		{
			CLAM_ASSERT(iPosition < mNumRows,
				"Index beyond matrix dimension");
			CLAM_ASSERT(iPosition >= 0,
				"Index beyond matrix dimension");
			CLAM_ASSERT(jPosition < mNumColumns,
				"Index beyond matrix dimension");
			CLAM_ASSERT(jPosition >= 0,
				"Index beyond matrix dimension");

			return MatrixBuffer()[mNumColumns*iPosition+jPosition];
		}

		// Get one column
		friend MatrixTmpl<T> GetColumn(unsigned int column, MatrixTmpl<T>& m )
		{
			CLAM_ASSERT(column<m.mNumColumns,
				"Column beyond matrix dimensions");
			CLAM_ASSERT(column >= 0,
				"Column beyond matrix dimensions");

			MatrixTmpl<T> ret(m.mNumRows, 1); // Column vector
			for (unsigned int i=0; i<m.mNumRows; i++)
				ret(i,0) = m(i,column);
			return ret;
		}

		// Get one row
		friend MatrixTmpl<T> GetRow(unsigned int row, MatrixTmpl<T>& m)
		{
			CLAM_ASSERT(row < m.mNumRows,
				"Row beyond matrix dimensions");
			CLAM_ASSERT(row >= 0,
				"Row beyond matrix dimensions");
	
			MatrixTmpl<T> ret(1, m.mNumColumns); // Row vector
			for (unsigned int i=0; i<m.mNumColumns; i++)
				ret(0,i) = m(row,i);
			return ret;
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
			CLAM_ASSERT(iPosition < mNumRows,
				"Index beyond matrix dimension");
			CLAM_ASSERT(iPosition >= 0,
				"Index beyond matrix dimension");
			CLAM_ASSERT(jPosition < mNumColumns,
				"Index beyond matrix dimension");
			CLAM_ASSERT(jPosition >= 0,
				"Index beyond matrix dimension");

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
			CLAM_ASSERT(mNumRows == newMatrix.mNumRows,"Adding matrix with different dimensions");
			CLAM_ASSERT(mNumColumns == newMatrix.mNumColumns,"Adding matrix with different dimensions");
			for (unsigned int i=0; i< (mNumRows*mNumColumns) ; i++)
				MatrixBuffer()[i] += newMatrix.MatrixBuffer()[i];
			return *this;
		}

		const MatrixTmpl<T>& operator -= (const MatrixTmpl<T>& newMatrix)
		{
			// Substract element by element
			CLAM_ASSERT(mNumRows == newMatrix.mNumRows,"Substracting matrix with different dimensions");
			CLAM_ASSERT(mNumColumns == newMatrix.mNumColumns,"Substracting matrix with different dimensions");
			for (unsigned int i=0; i< (mNumRows*mNumColumns) ; i++)
				MatrixBuffer()[i] -= newMatrix.MatrixBuffer()[i];
			return *this;
		}

		friend MatrixTmpl<T> operator + (MatrixTmpl<T>& m1, MatrixTmpl<T>& m2)
		{
			CLAM_ASSERT(m1.mNumRows == m2.mNumRows,"Adding matrix with different dimensions");
			CLAM_ASSERT(m1.mNumColumns == m2.mNumColumns,"Adding matrix with different dimensions");
			MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns);	// Construction of an Vector object
			for (unsigned int i=0; i<ret.mNumRows; i++)
				for (unsigned int j=0; j<ret.mNumColumns; j++)
					ret(i,j) = m1(i,j) + m2(i,j);
			return ret;
		}

		// add an element to all the matrix elements
		friend MatrixTmpl<T> operator + (const MatrixTmpl<T>& m1, const T & element)
		{
			MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns); 
			for (unsigned int i=0; i<ret.mNumRows; i++)
				for (unsigned int j=0; j<ret.mNumColumns; j++)
					ret(i,j) = m1(i,j) + element;
			return ret;
		}

		friend MatrixTmpl<T> operator - (MatrixTmpl<T>& m1, MatrixTmpl<T>& m2)
		{
			CLAM_ASSERT(m1.mNumRows == m2.mNumRows,"Substracting matrix with different dimensions");
			CLAM_ASSERT(m1.mNumColumns == m2.mNumColumns,"Substracting matrix with different dimensions");
			MatrixTmpl<T> ret(m1.mNumRows, m1.mNumColumns);	// Construction of an Vector object
			for (unsigned int i=0; i<ret.mNumRows; i++)
				for (unsigned int j=0; j<ret.mNumColumns; j++)
					ret(i,j) = m1(i,j) - m2(i,j);
			return ret;
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
			CLAM_ASSERT( m1.mNumColumns == m2.mNumRows,"Multiplications with incompatible arrays");
			MatrixTmpl<T> ret(m1.mNumRows, m2.mNumColumns);
			for (unsigned int i=0; i<ret.mNumRows; i++)
				for (unsigned int j=0; j<ret.mNumColumns; j++) {
					ret(i,j) = 0;
					for( unsigned int k=0; k<m1.mNumColumns; k++)
						ret(i,j) += m1(i,k)*m2(k,j);
				}
			return ret;
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

