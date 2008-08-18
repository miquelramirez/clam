
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

#ifndef PoolFloatArrayDataSource_hxx
#define PoolFloatArrayDataSource_hxx

#include "FloatArrayDataSource.hxx"
//#include "Project.hxx"
#include <vector>
#include "FrameDivision.hxx"
#include "FloatVectorStorage.hxx"

namespace CLAM_Annotator { class FrameDivision; }

namespace CLAM
{
namespace VM
{
	class PoolFloatArrayDataSource : public FloatArrayDataSource
	{
		public:
			PoolFloatArrayDataSource();
			void setDataSource(unsigned nBins, CLAM::TData binGap, CLAM::TData firstBinOffset, std::vector<std::string> binLabels);
			void clearData();
			void setStorage(FloatVectorStorage *storage, CLAM::TData samplingRate, CLAM_Annotator::FrameDivision *frameDivision, unsigned nFrames);
			void noStorage();
			bool setCurrentTime(double timeMiliseconds);

			std::string getLabel(unsigned bin) const
			{
				if (bin < _binLabels.size())
					return _binLabels[bin];

				if (!_binGap) return "ERR";

				std::ostringstream oss;
				oss << _firstBinOffset + bin * _binGap;
				return oss.str();
			}
			const TData * getData() const
			{
				if (!_storage) return 0;
				return &_storage->Data()[0];
			}
			const CLAM::TData * frameData()
			{
				return _frameData;
			}
			unsigned nFrames() const
			{
				return _nFrames;
			}
			unsigned nBins() const
			{
				return _nBins;
			}
		private:
			std::string _scope;
			std::string _name;
			std::string _parentScope;
			std::string _parentName;
			std::vector<std::string> _binLabels;
			unsigned _nFrames;
			const CLAM_Annotator::FrameDivision * _frameDivision;
			CLAM::TData _samplingRate;
			const CLAM::TData *_frameData;
			unsigned _nBins;
			CLAM::TData _binGap;
			CLAM::TData _firstBinOffset;
			unsigned _currentFrame;
			FloatVectorStorage *_storage;
	};
}
}

#endif

