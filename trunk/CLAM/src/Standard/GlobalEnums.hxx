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
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EInterpolation() : Enum(sEnumValues, sDefault) {}
	EInterpolation(tValue v) : Enum(sEnumValues, v) {};
	EInterpolation(std::string s) : Enum(sEnumValues, s) {};

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

	virtual Component* Species() const
	{
		return (Component*) new EInterpolation;
	};
};

/** Interpolation
*   Enumeration to use for specifying the type of scale used (linear or log)
*/
//TODO (may include different types of logarithmic scales??
class EScale : public Enum
{
public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EScale() : Enum(sEnumValues, sDefault) {}
	EScale(tValue v) : Enum(sEnumValues, v) {};
	EScale(std::string s) : Enum(sEnumValues, s) {};

	typedef enum {
		eLinear,
		eLog
	};

	virtual Component* Species() const
	{
		// TODO: This is a xapusa. I want a default constructor!
		return (Component*) new EScale;
	};
};


class EWindowType : public Enum
{
public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EWindowType() : Enum(sEnumValues, sDefault) {}
	EWindowType(tValue v) : Enum(sEnumValues, v) {};
	EWindowType(std::string s) : Enum(sEnumValues, s) {};

	typedef enum {
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
		eBlackmanHarrisLike
	};

	virtual Component* Species() const
	{
		return (Component*) new EWindowType(eHamming);
	};
};



}

#endif //_GlobalEnums_
