/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef FloatArrayDataSource_hxx
#define FloatArrayDataSource_hxx

#include <string>

namespace CLAM
{
namespace VM
{

	class FloatArrayDataSource
	{
		public:
			virtual ~FloatArrayDataSource() {}
			virtual const std::string & getLabel(unsigned bin) const = 0;
			virtual const double * frameData() const = 0;
			virtual unsigned nBins() const = 0;
	};

	class DummyFloatArrayDataSource : public FloatArrayDataSource
	{
		public:
			const std::string & getLabel(unsigned bin) const
			{
				static std::string a("A");
				return a;
			}
			const double * frameData() const
			{
				static double data[] = {0, 0.2, 0, 0.4, 0, 0.6, 0.9, 0.1,0, 0.1, 1, 0.5};
				return data;
			}
			unsigned nBins() const
			{
				return 12;
			}
	};
}
}
#endif

