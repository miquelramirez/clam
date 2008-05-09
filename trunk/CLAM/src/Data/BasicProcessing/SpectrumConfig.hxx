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

#ifndef __SpectrumConfig__
#define __SpectrumConfig__
#include <typeinfo>

#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"
#include "GlobalEnums.hxx"
#include "ProcessingDataConfig.hxx"

namespace CLAM {

/**
* The Spectrum configuration object.
*/
class SpectrumConfig : public ProcessingDataConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SpectrumConfig, 5, ProcessingDataConfig);
	/**
	* The kind of scale
	*/
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, TData, SpectralRange);
	DYN_ATTRIBUTE (2, public, int, Size);
	DYN_ATTRIBUTE (3, public, SpecTypeFlags, Type);
	DYN_ATTRIBUTE (4, public, int, BPFSize);
public:
	/*
	* Sets to the default configuration.
	*/
	void DefaultValues();
	~SpectrumConfig(){};
protected:
	/*
	* Dynamic Type constructor: instanciates all the attributes.
	*/
	void DefaultInit();
};

} // namespace CLAM

#endif

