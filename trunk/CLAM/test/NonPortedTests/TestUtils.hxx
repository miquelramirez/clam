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

#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H

#include "SpectrumComp.hxx"
#include "Audio.hxx"
#include "Err.hxx"
#include "DataTypes.hxx"

using CLAM::Err;
using CLAM::TData;
using CLAM::Spectrum;
using CLAM::Array;

namespace CLAMTest {

	class TestError : public Err
	{
	public:
		TestError(const char* description) : Err(description) {};
	};

	namespace TestUtils {

		TData MaxDiff(const CLAM::Audio &a1,const CLAM::Audio &a2);

		bool operator!=(const CLAM::Audio &a1, const CLAM::Audio &a2);
		bool operator==(const CLAM::Audio &a1, const CLAM::Audio &a2);
		bool operator!=(Spectrum &s1, Spectrum &s2);
		bool operator==(Spectrum &s1, Spectrum &s2);

		TData MaxDiff(const CLAM::Audio &a1,const CLAM::Audio &a2)
		{
			TData diff = 0.0, max_diff = 0.0;
			int i, size = a1.GetSize();

			if (size != a2.GetSize())
				throw TestError("Audio sizes differ in comparision.");

			const Array<TData> &samples1 = a1.GetBuffer();
			const Array<TData> &samples2 = a2.GetBuffer();
			for (i=0; i<size; i++) {
				diff = fabs(samples1[i] -samples2[i]);
				if ( diff > max_diff)
					max_diff = diff;
			}
			return max_diff;
		}

		bool operator!=(const CLAM::Audio &a1, const CLAM::Audio &a2)
		{
			if (MaxDiff(a1,a2) > 0.001)
				return true;
			return false;
		}

		bool operator!=(Spectrum &s1, Spectrum &s2)
		{
			if (TestUtils::MaxDiff(s1,s2) > 0.001)
				return true;
			return false;
		}

		bool operator==(const CLAM::Audio &a1, const CLAM::Audio &a2)
		{
			return !(a1!=a2);
		}

		bool operator==(Spectrum &s1, Spectrum &s2)
		{
			return !(s1!=s2);
		}




	}






}

#endif

