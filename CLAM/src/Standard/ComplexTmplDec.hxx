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


#ifndef _ComplexTmplDec_
#define _ComplexTmplDec_

#include <iosfwd>

namespace CLAM
{

	template <class T>
	class ComplexTmpl
	{
	private:
		T mRe, mIm;
	public:
		ComplexTmpl(T re = 0.0,T im = 0.0) //< constructor
		{
			mRe = re;
			mIm = im;
		};

		~ComplexTmpl(){}

		const T Real(void) const {return mRe;}; //< accessor returns the real part
		const T Imag(void) const {return mIm;}; //< accessor returns the imaginary part

		void SetReal(const T& re) { mRe = re;}; //< accesor sets the real part
		void SetImag(const T& im) { mIm = im;}; //< accesor sets the imaginary part

		/// returns magnitude
		const T Mag(void) const
		{
			return sqrt (mRe*mRe + mIm*mIm);
		}

		/// returns angle
		const T Ang(void) const
		{
			return atan2 (mIm,mRe);
		}

		/// friend function to handle polar coordinates
		friend ComplexTmpl<T> ToPolar(const T& r, const T& theta)
		{
			return ComplexTmpl<T>(r * cos(theta), r * sin(theta));
		};

		// ------   member operators ... ------

		///  complex '=' operator  (float)
		const ComplexTmpl<T>& operator = (const float re)
		{
			mRe = re;
			mIm = 0;
			return *this;
		}

		/// complex '=' operator
		const ComplexTmpl<T>& operator = (const ComplexTmpl<T>& a)
		{
			mRe = a.mRe;
			mIm = a.mIm;
			return *this;

		}

		/// complex '+=' operator
		const ComplexTmpl<T>& operator += (const ComplexTmpl<T>& a)
		{
			mRe = mRe + a.mRe;
			mIm = mIm + a.mIm;
			return *this;
		}

		///  complex '-=' operator
		const ComplexTmpl<T>& operator -= (const ComplexTmpl<T>& a)
		{
			mRe = mRe - a.mRe;
			mIm = mIm - a.mIm;
			return *this;
		}

		/// complex '+' operator
		ComplexTmpl<T> operator + (const ComplexTmpl<T>& b) const
		{
			ComplexTmpl<T> ret(mRe + b.mRe , mIm + b.mIm);
			return ret;
		}

		///  complex '-' operator
		ComplexTmpl<T> operator - (const ComplexTmpl<T>& b) const
		{
			ComplexTmpl<T> ret(mRe - b.mRe , mIm - b.mIm);
			return ret;
		}

		/// complex '*' operator
		ComplexTmpl<T> operator * (const ComplexTmpl<T>& b) const
		{
			return ComplexTmpl<T> (
				(mRe * b.mRe) - (mIm * b.mIm),
				(mRe * b.mIm) + (b.mRe * mIm)
			);
		}

		/// Product by a scalar
		ComplexTmpl<T> operator * (const T& scalar ) const
		{
			return ComplexTmpl<T>(mRe*scalar,mIm*scalar);
		}

		/// complex '/' operator
		ComplexTmpl<T> operator / (const ComplexTmpl<T>& b) const
		{
			return ComplexTmpl<T> (
				((mRe * b.mRe) + (mIm * b.mIm)) /((b.mRe * b.mRe) + (b.mIm * b.mIm)),
				((b.mRe * mIm) - (mRe * b.mIm)) /((b.mRe * b.mRe) + (b.mIm * b.mIm))
			);
		}

		/// complex '==' operator
		bool operator == (const ComplexTmpl<T>& b) const
		{
			return (mRe == b.mRe) && (mIm == b.mIm);
		}

		/// complex '!=' operator
		bool operator != (const ComplexTmpl<T>& b) const
		{
			return (mRe != b.mRe) || (mIm != b.mIm);
		}

	};

	template <class T>
	std::istream& operator >> (std::istream & stream, ComplexTmpl<T> & a);

	template <class T>
	std::ostream& operator << (std::ostream & stream, const ComplexTmpl<T> & a);

} // namespace CLAM

#endif // _ComplexTmplDec_
