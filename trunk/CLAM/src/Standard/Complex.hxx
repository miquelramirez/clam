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


#ifndef CLAM_Complex_hxx
#define CLAM_Complex_hxx

#include <iosfwd>
#include "CLAM_Math.hxx"
#include "TypeInfo.hxx"

namespace CLAM
{

	class Complex
	{
		typedef TData T;
	private:
		T mRe;
		T mIm;

	public:
		Complex(T re = 0.f, T im = 0.f) : mRe(re), mIm(im) {}
		Complex(const Complex &rhs) : mRe(rhs.mRe), mIm(rhs.mIm) {}

		const T Real() const { return mRe; } //< accessor returns the real part
		const T Imag() const { return mIm; } //< accessor returns the imaginary part

		void SetReal(const T re) { mRe = re; } //< accesor sets the real part
		void SetImag(const T im) { mIm = im; } //< accesor sets the imaginary part

		/// returns magnitude
		const T Mag() const
		{
#ifdef CLAM_OPTIMIZE
			const float insignificant = 0.000001;
			T absIm = Abs(mIm);
			T absRe = Abs(mRe);
			if(absIm<insignificant && absRe>insignificant) return absRe;
			if(absRe<insignificant && absIm>insignificant) return absIm;
#endif
			return CLAM_sqrt(mRe*mRe + mIm*mIm);
		}

		/// returns squared magnitude
		const T SquaredMag() const
		{
			return mRe*mRe + mIm*mIm;
		}

		/// returns angle
		const T Ang() const
		{
			return CLAM_atan2(mIm, mRe);
		}

		/// function to handle polar coordinates
		Complex ToPolar(const T& r, const T& theta)
		{
			return Complex(r*CLAM_cos(theta), r*CLAM_sin(theta));
		}

		// ------   member operators ... ------

		///  complex '=' operator  (float)
		Complex& operator = (const T re)
		{
			mRe = re;
			mIm = 0.f;
			return *this;
		}

		/// complex '=' operator
		Complex& operator = (const Complex& rhs)
		{
			mRe = rhs.mRe;
			mIm = rhs.mIm;
			return *this;

		}

		/// complex '+=' operator
		Complex& operator += (const Complex& rhs)
		{
			mRe += rhs.mRe;
			mIm += rhs.mIm;
			return *this;
		}

		///  complex '-=' operator
		Complex& operator -= (const Complex& rhs)
		{
			mRe -= rhs.mRe;
			mIm -= rhs.mIm;
			return *this;
		}

		/// complex '+' operator
		Complex operator + (const Complex& rhs) const
		{
			return Complex(mRe + rhs.mRe, mIm + rhs.mIm);
		}

		///  complex '-' operator
		Complex operator - (const Complex& rhs) const
		{
			return Complex(mRe - rhs.mRe, mIm - rhs.mIm);
		}

		/// complex '*' operator
		Complex operator * (const Complex& rhs) const
		{
			return Complex(mRe*rhs.mRe - mIm*rhs.mIm, mRe*rhs.mIm + rhs.mRe*mIm);
		}

		/// Product by a scalar
		Complex operator * (const T& scalar) const
		{
			return Complex(mRe*scalar, mIm*scalar);
		}

		/// complex '/' operator
		Complex operator / (const Complex& rhs) const
		{
			const float rhsInvSquaredMag = 1.f/(rhs.mRe*rhs.mRe + rhs.mIm*rhs.mIm);

			return Complex((mRe*rhs.mRe + mIm*rhs.mIm)*rhsInvSquaredMag, (rhs.mRe*mIm - mRe*rhs.mIm)*rhsInvSquaredMag);
		}

		/// complex '==' operator
		bool operator == (const Complex& b) const
		{
			return (mRe == b.mRe) && (mIm == b.mIm);
		}

		/// complex '!=' operator
		bool operator != (const Complex& b) const
		{
			return !(*this == b);
		}

	};

	std::istream& operator >> (std::istream & stream, Complex & a);
	std::ostream& operator << (std::ostream & stream, const Complex & a);

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, Complex);

} // namespace CLAM

#endif // CLAM_Complex_hxx

