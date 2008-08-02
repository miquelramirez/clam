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

#include "Enum.hxx"

#ifndef _GlobalEnums_
#define _GlobalEnums_

namespace CLAM {


/** Interpolation
*   Enumeration to use for specifying the type of interpolation used
*/
class EInterpolation: public Enum
{
public:
	
	EInterpolation() : Enum(ValueTable(), eLinear) {}
	EInterpolation(tValue v) : Enum(ValueTable(), v) {};
	EInterpolation(std::string s) : Enum(ValueTable(), s) {};
	virtual Component* Species() const { return new EInterpolation; }

	typedef enum {
		eStep,
		eRound,
		eLinear,
		eSpline,
		ePolynomial2,
		ePolynomial3,
		ePolynomial4,
		ePolynomial5,
		ePolynomialn
	} tEnum;

	static tEnumValue * ValueTable()
	{
		static tEnumValue sValueTable[] =
		{
			{eStep,"Step"},
			{eRound,"Round"},
			{eLinear,"Linear"},
			{eSpline,"Spline"},
			{ePolynomial2,"2ond_order_Polynomial"},
			{ePolynomial3,"3rd_order_Polynomial"},
			{ePolynomial4,"4th_order_Polynomial"},
			{ePolynomial5,"5th_order_Polynomial"},
			{ePolynomialn,"nth_order_Polynomial"},
			{0,NULL}
		};
		return sValueTable;
	}
};

/** Interpolation
*   Enumeration to use for specifying the type of scale used (linear or log)
*/
//TODO (may include different types of logarithmic scales??
class EScale : public Enum
{
public:
	
	EScale() : Enum(ValueTable(), eLinear) {}
	EScale(tValue v) : Enum(ValueTable(), v) {};
	EScale(std::string s) : Enum(ValueTable(), s) {};
	virtual Component* Species() const { return new EScale; };

	enum {
		eLinear,
		eLog
	};

	static tEnumValue * ValueTable()
	{
		static tEnumValue sValueTable[] =
		{
			{EScale::eLinear,"Linear"},
			{EScale::eLog,"Log"},
			{0,NULL}
		};
		return sValueTable;
	}
};


class EWindowType : public Enum
{
public:
	
	EWindowType() : Enum(ValueTable(), eHamming) {}
	EWindowType(tValue v) : Enum(ValueTable(), v) {};
	EWindowType(std::string s) : Enum(ValueTable(), s) {};
	virtual Component* Species() const { return new EWindowType(eHamming); }

	enum {
		eNone,
		eKaiserBessel17,
		eKaiserBessel18,
		eKaiserBessel19,
		eKaiserBessel20,
		eKaiserBessel25,
		eKaiserBessel30,
		eKaiserBessel35,
		eBlackmanHarris74,
		eBlackmanHarris62,
		eBlackmanHarris70,
		eBlackmanHarris92,
		eHamming,
		eTriangular,
		eBlackmanHarris92TransMainLobe,
		eGaussian,
		eBlackmanHarrisLike,
		eSine,
		eSquare
	};

	static tEnumValue * ValueTable()
	{
		static tEnumValue sValueTable[] =
		{
			{eNone,"None"},
			{eKaiserBessel17,"KaiserBessel17"},
			{eKaiserBessel18,"KaiserBessel18"},
			{eKaiserBessel19,"KaiserBessel19"},
			{eKaiserBessel20,"KaiserBessel20"},
			{eKaiserBessel25,"KaiserBessel25"},
			{eKaiserBessel30,"KaiserBessel30"},
			{eKaiserBessel35,"KaiserBessel35"},
			{eBlackmanHarris74,"BlackmanHarris74"},
			{eBlackmanHarris62,"BlackmanHarris62"},
			{eBlackmanHarris70,"BlackmanHarris70"},
			{eBlackmanHarris92,"BlackmanHarris92"},
			{eHamming,"Hamming"},
			{eTriangular,"Triangular"},
			{eBlackmanHarris92TransMainLobe,"BlackmanHarris92 MainLobe"},
			{eGaussian,"Gaussian"},
			{eBlackmanHarrisLike,"BlackmanHarrisLike"},
			{eSine,"Sine"},
			{eSquare,"Square"},
			{0,NULL}
		};
		return sValueTable;
	}
};



}

#endif //_GlobalEnums_

