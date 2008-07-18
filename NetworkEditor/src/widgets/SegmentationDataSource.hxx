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

#ifndef SegmentationDataSource_hxx
#define SegmentationDataSource_hxx

#include <string>
#include <vector>
#include <CLAM/Segmentation.hxx>
#include <CLAM/Assert.hxx>

namespace CLAM
{
namespace VM
{

	class SegmentationDataSource
	{
		public:
			virtual ~SegmentationDataSource() {}
			virtual const CLAM::Segmentation & frameData() = 0;
			virtual void release() {}
			virtual bool isEnabled() const
			{
				return true;
			}
	};
/*
	class DummySegmentationDataSource : public SegmentationDataSource
	{
			unsigned _nBins;
			CLAM::TData * _data;
			std::vector<std::string> _labels;
		public:
			DummySegmentationDataSource(unsigned nBins, CLAM::TData * data=0)
			{
				static CLAM::TData defaultData[] = {
					0, 0.2, 0, 0.4, 0, 0.6, 0.9, 0.1,0, 0.1, 1, 0.5,
					0, 0.2, 0, 0.4, 0, 0.6, 0.9, 0.1,0, 0.1, 1, 0.5,
					0, 0.2, 0, 0.4, 0, 0.6, 0.9, 0.1,0, 0.1, 1, 0.5,
					0, 0.2, 0, 0.4, 0, 0.6, 0.9, 0.1,0, 0.1, 1, 0.5,
					0, 0.2, 0, 0.4, 0, 0.6, 0.9, 0.1,0, 0.1, 1, 0.5
					};
				_data = data ? data : defaultData;
				CLAM_ASSERT(data||nBins<60,
					"No so many dummy data for the bins you asked");
				_nBins = nBins;
			}
			std::string getLabel(unsigned bin) const
			{
				static std::string a("");
				if (bin<_labels.size())
					return _labels[bin];
				return a;
			}
			const CLAM::TData * frameData()
			{
				return _data;
			}
			unsigned nBins() const
			{
				return _nBins;
			}
			bool isEnabled() const
			{
				return false;
			}
			void setLabels(const std::vector<std::string> & labels)
			{
				_labels = labels;
			}
			void setLabels(const char ** labels)
			{
				_labels.clear();
				for (const char * label = *labels; *label; label++)
					_labels.push_back(label);
			}
	};
*/
}
}
#endif

