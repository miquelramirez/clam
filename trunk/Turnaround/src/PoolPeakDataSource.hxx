
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

#ifndef PoolPeakDataSource_hxx
#define PoolPeakDataSource_hxx

#include "PeakDataSource.hxx"
#include <vector>
#include <list>
#include "FrameDivision.hxx"

namespace CLAM_Annotator { class FrameDivision; }

namespace CLAM
{
namespace VM
{
	class PoolPeakDataSource : public PeakDataSource
	{
			typedef std::vector<TData> Buffer;
			typedef std::list<Buffer> Storage;
			typedef std::vector<std::pair<Buffer*, Buffer*> > Index;
		public:
			PoolPeakDataSource();
			void setDataSource(unsigned nBins);
			void clearData();
			void updateData(std::vector<CLAM::TData>, std::vector<CLAM::TData>, CLAM::TData samplingRate, CLAM_Annotator::FrameDivision *frameDivision, unsigned nFrames);
			bool setCurrentTime(double timeMiliseconds);

			const TData * positionData()
			{
				if (_positionData.empty()) return 0;
				return _positionFrameData;
			}
			const TData * magnitudeData()
			{
				if (_magnitudeData.empty()) return 0;
				return _magnitudeFrameData;
			}
			const TData * positionFrameData()
			{
				return _positionFrameData;
			}
			const TData * magnitudeFrameData()
			{
				return _magnitudeFrameData;
			}
			unsigned nFrames() const
			{
				return _nFrames;
			}
			unsigned nBins() const
			{
				return _nBins;
			}
			bool isEnabled() const
			{
				return true; // ?
			}
		private:
			std::vector<std::string> _binLabels;
			std::vector<TData> _positionData;
			std::vector<TData> _magnitudeData;
			unsigned _nFrames;
			const CLAM_Annotator::FrameDivision * _frameDivision;
			CLAM::TData _samplingRate;
			const CLAM::TData *_positionFrameData;
			const CLAM::TData *_magnitudeFrameData;
			unsigned _nBins;
			unsigned _currentFrame;
			Index _frameDataIndex;
			Storage _positionStorage;
			Storage _magnitudeStorage;
	};
}
}

#endif

