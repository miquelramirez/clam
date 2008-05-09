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

#ifndef _WindowGeneratorConfig_
#define _WindowGeneratorConfig_
#include <typeinfo>

#include "ProcessingConfig.hxx"
#include "Enum.hxx"
#include "GlobalEnums.hxx"
#include "DataTypes.hxx"

namespace CLAM {

	class Component;

	class EWindowNormalize : public Enum {
	public:
		EWindowNormalize() : Enum(ValueTable(), eAnalysis) {}
		EWindowNormalize(tValue v) : Enum(ValueTable(), v) {};
		EWindowNormalize(std::string s) : Enum(ValueTable(), s) {};

		typedef enum {
			eNone,
			eAnalysis,
			eEnergy,
			eMax
		};

		virtual Component* Species() const
		{
			return new EWindowNormalize(eAnalysis);
		};
		static tEnumValue * ValueTable()
		{
			static tEnumValue sEnumValues[] = {
				{eNone,"NoNormalization"},
				{eAnalysis,"NormalizationForAnalysis"},
				{eEnergy,"NormalizationForConstantEnergy"},
				{eMax,"NormalizationForMaximizingMagnitude"},
				{0,NULL}
			};
			return sEnumValues;
		}
	};

	class WindowGeneratorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (WindowGeneratorConfig, 6,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, EWindowType, Type);
		DYN_ATTRIBUTE (1, public, TSize, Size);
		DYN_ATTRIBUTE (2, public, TSize, MaxSize);
		DYN_ATTRIBUTE (3, public, bool, UseTable);
		DYN_ATTRIBUTE (4, public, EWindowNormalize, Normalize);
		DYN_ATTRIBUTE (5, public, bool, Invert);
	public:
		void DefaultValues();
	protected:
		void DefaultInit();

	};

}

#endif

