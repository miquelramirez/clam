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

#ifndef __QTPLOTTER__
#define __QTPLOTTER__

#include <map>
#include <string>
#include <vector>
#include "QtPlots.hxx"

class QWorkspace;

namespace CLAM
{
    namespace VM
    {
		enum WhichPlot { FundamentalPlot=0, SinTracksPlot };

		/**
		 * Allows viewing several qtplots simultaneously.
		 *
		 * @ingroup QTVM
		 */

		class QtPlotter
		{
			typedef std::map<std::string,int> PlotMap;
			typedef std::vector<QtPlot*> PlotList;
			typedef std::vector<bool> MultiPlot;
			typedef int PlotIndex;
			typedef bool HaveWorkspace;
			typedef QWorkspace* Workspace;

		public:
			static void Add(const std::string& plotKey, const Audio& data);
			static void Add(const std::string& plotKey, const Audio& leftChannel, const Audio& rightChannel);
			static void Add(const std::string& plotKey, const Segment& data, WhichPlot type);
			static void Add(const std::string& plotKey, const Spectrum& data);
			static void Add(const std::string& plotKey, const Spectrum& spec, const SpectralPeakArray& peaks);
			static void Add(const std::string& plotKey, const Array<Spectrum>& data, const TData& duration);
			static void Add(const std::string& plotKey, const std::string& dataKey, const DataArray& data);
			static void Add(const std::string& plotKey, 
							const Array<SpectralPeakArray>& data, 
							const TData& sampleRate, 
							const TData& duration);

			static void SetLabel(const std::string& plotKey, const std::string& label);
			static void SetMarks(const std::string& plotKey, std::vector<unsigned>& marks);

			static void Run();

			// only for QtMultiPlot
			static void AddData(const std::string& plotKey, const std::string& dataKey, const DataArray& data);
			static void AddData(const std::string& plotKey, const std::string& dataKey, const BPF& data,int samples=100);
			static void SetDataColor(const std::string& plotKey, const std::string& dataKey, Color c);
			static void SetXRange(const std::string& plotKey, const TData& xmin, const TData& xmax);
			static void SetYRange(const std::string& plotKey, const TData& ymin, const TData& ymax);
			static void SetUnits(const std::string& plotKey, const std::string& xunits, const std::string& yunits);
			static void SetToolTips(const std::string& plotKey,const std::string& xtooltip,const std::string& ytooltip);
				
		private:				
			static Workspace     ws;
			static PlotMap       mPlotMap;
			static PlotList      mPlotList;
			static MultiPlot     mMultiPlot;
			static PlotIndex     mPlotIndex;
			static HaveWorkspace mHaveWorkspace;

			static bool IsMultiPlot(const int& index);
			static bool Exist(const std::string& key);
			static void CreateWorkspace();
			static void Register(const std::string& pKey, QtPlot* plot, bool isMultiPlot=false);

			static void NonExistentKeyMsg(const std::string& methodName, const std::string& plotKey);
			static void DuplicatedKeyMsg(const std::string& methodName, const std::string& plotKey);
			static void NonMultiPlotMsg(const std::string& methodName, const std::string& plotKey);

		};
    }
}

#endif

