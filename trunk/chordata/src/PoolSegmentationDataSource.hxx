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

#ifndef PoolSegmentationDataSource_hxx
#define PoolSegmentationDataSource_hxx

#include <CLAM/qtmonitors/SegmentationDataSource.hxx>

#include <CLAM/DiscontinuousSegmentation.hxx>

namespace CLAM
{
namespace VM
{
	class PoolSegmentationDataSource : public SegmentationDataSource
	{
		public:
			PoolSegmentationDataSource();
			void updateData(CLAM::Segmentation & data);
			bool setCurrentTime(double timeMiliseconds);
			double currentTime();
			
			std::string getLabel(unsigned bin) const
			{
				static std::string dummyLabel;
				return dummyLabel;
			}
			const CLAM::Segmentation & frameData()
			{
				return _data;
			}
			void release()
			{
			}
			bool isEnabled() const
			{
				return true;
			}

		private:
			CLAM::DiscontinuousSegmentation _data;
			double _currentTime;
	};
}
}

#endif// PoolSegmentationDataSource_hxx
