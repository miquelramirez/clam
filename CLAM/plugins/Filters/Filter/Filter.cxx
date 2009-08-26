/*
 * Copyright (C) 2009 Hernan Ordiales
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

#include "Filter.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Filter",
		"category", "Plugins",
		"description", "Filter",
		0
	};
	static FactoryRegistrator<ProcessingFactory, Filter> reg = metadata;
}

	void FilterConfig::DefaultInit( )
	{
		AddAll();
		UpdateData();
		SetAmountOfInputCoefficients(1);
		SetAmountOfOutputCoefficients(1);
	}
}
