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

#include "GlobalEnums.hxx"

namespace CLAM{

//Initializer for Interpolation enum
Enum::tEnumValue EInterpolation::sEnumValues[] = {
	{EInterpolation::eStep,"Step"},
	{EInterpolation::eRound,"Round"},
	{EInterpolation::eLinear,"Linear"},
	{EInterpolation::eSpline,"Spline"},
	{EInterpolation::ePolynomial2,"2ond_order_Polynomial"},
	{EInterpolation::ePolynomial3,"3rd_order_Polynomial"},
	{EInterpolation::ePolynomial4,"4th_order_Polynomial"},
	{EInterpolation::ePolynomial5,"5th_order_Polynomial"},
	{EInterpolation::ePolynomialn,"nth_order_Polynomial"},
	{0,NULL}
};

Enum::tValue EInterpolation::sDefault = EInterpolation::eLinear;



//Initializer for Scale enum
Enum::tEnumValue EScale::sEnumValues[] = {
	{EScale::eLinear,"Linear"},
	{EScale::eLog,"Log"},
	{0,NULL}
};

Enum::tValue EScale::sDefault = EScale::eLinear;


	Enum::tEnumValue EWindowType::sEnumValues[] = {
		{EWindowType::eKaiserBessel17,"KaiserBessel17"},
		{EWindowType::eKaiserBessel18,"KaiserBessel18"},
		{EWindowType::eKaiserBessel19,"KaiserBessel19"},
		{EWindowType::eKaiserBessel20,"KaiserBessel20"},
		{EWindowType::eKaiserBessel25,"KaiserBessel25"},
		{EWindowType::eKaiserBessel30,"KaiserBessel30"},
		{EWindowType::eKaiserBessel35,"KaiserBessel35"},
		{EWindowType::eBlackmanHarris74,"BlackmanHarris74"},
		{EWindowType::eBlackmanHarris62,"BlackmanHarris62"},
		{EWindowType::eBlackmanHarris70,"BlackmanHarris70"},
		{EWindowType::eBlackmanHarris92,"BlackmanHarris92"},
		{EWindowType::eHamming,"Hamming"},
		{EWindowType::eTriangular,"Triangular"},
		{EWindowType::eBlackmanHarris92TransMainLobe,"BlackmanHarris92 MainLobe"},
		{EWindowType::eGaussian,"Gaussian"},
		{EWindowType::eBlackmanHarrisLike,"BlackmanHarrisLike"},
		{0,NULL}
	};

	Enum::tValue EWindowType::sDefault = EWindowType::eHamming;

}


