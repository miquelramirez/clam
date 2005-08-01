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
#include <qworkspace.h>
#include "QtPlots.hxx"

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
			class PWorkspace : public QWorkspace
			{
			public:
				PWorkspace(QWidget* parent=0)
					: QWorkspace(parent)
					{}
				~PWorkspace(){}

			protected:
				void closeEvent(QCloseEvent *e){ e->accept(); QtPlotter::Clear(); }

			};

			typedef std::map<std::string,int> PlotMap;
			typedef std::map<std::string,int> BPFEditorMap;
			typedef std::vector<QtPlot*>      PlotList;
			typedef std::vector<BPFEditor*>   BPFEditorList;
			typedef std::vector<bool>         MultiPlot;
			typedef int                       PlotIndex;
			typedef int                       BPFEditorIndex;
			typedef bool                      HaveWorkspace;
			typedef PWorkspace*               Workspace;

		public:
			static void Add(const std::string& plotKey, const Audio& data);
			static void Add(const std::string& plotKey, const Audio& leftChannel, const Audio& rightChannel);
			static void Add(const std::string& plotKey, const Segment& data, WhichPlot type);
			static void Add(const std::string& plotKey, const Spectrum& data);
			static void Add(const std::string& plotKey, const Spectrum& spec, const SpectralPeakArray& peaks);
			static void Add(const std::string& plotKey, const Array<Spectrum>& data, const TData& duration);
			static void Add(const std::string& plotKey, const std::string& dataKey, const DataArray& data);
			static void Add(const std::string& plotKey, const BPF& data);
			static void Add(const std::string& plotKey, 
							const Array<SpectralPeakArray>& data, 
							const TData& sampleRate, 
							const TData& duration);
			
			static void SetLabel(const std::string& plotKey, const std::string& label);
			static void SetMarks(const std::string& plotKey, std::vector<unsigned>& marks);

			static void Run();

			// for QtMultiPlot only
			static void SetXRange(const std::string& plotKey, const TData& xmin, const TData& xmax);
			static void SetYRange(const std::string& plotKey, const TData& ymin, const TData& ymax);
			static void AddData(const std::string& plotKey, const std::string& dataKey, const DataArray& data);
			static void SetDataColor(const std::string& plotKey, const std::string& dataKey, const Color& c);
			static void SetUnits(const std::string& plotKey, const std::string& xunits, const std::string& yunits);
			static void SetToolTips(const std::string& plotKey,const std::string& xtooltip,const std::string& ytooltip);

			// for BPFEditor only
			static void SetXRange(const std::string& plotKey, const double& min, const double& max, const EScale& scale);
			static void SetYRange(const std::string& plotKey, const double& min, const double& max, const EScale& scale);
			static void SetXScale(const std::string& plotKey, const EScale& scale);
			static void SetYScale(const std::string& plotKey, const EScale& scale);
			static void AddData(const std::string& plotKey, const std::string& dataKey, const BPF& data);
			static void SetDataColor(const std::string& plotKey, 
									 const std::string& dataKey, 
									 const Color& lines_color,
									 const Color& handlers_color);
	
		private:				
			static Workspace      ws;
			static PlotMap        mPlotMap;
			static PlotList       mPlotList;
			static MultiPlot      mMultiPlot;
			static PlotIndex      mPlotIndex;
			static HaveWorkspace  mHaveWorkspace;
			
			static BPFEditorMap   mBPFEditorMap;
			static BPFEditorList  mBPFEditorList;
			static BPFEditorIndex mBPFEditorIndex;

			static bool IsMultiPlot(const int& index);
			static bool Exist(const std::string& key);
			static bool ExistBPF(const std::string& key);
			static void CreateWorkspace();
			static void Register(const std::string& pKey, QtPlot* plot, bool isMultiPlot=false);
			static void Register(const std::string& pKey, BPFEditor* plot);

			static void NonExistentKeyMsg(const std::string& methodName, const std::string& plotKey);
			static void DuplicatedKeyMsg(const std::string& methodName, const std::string& plotKey);
			static void NonMultiPlotMsg(const std::string& methodName, const std::string& plotKey);

			static void Clear();

		};
    }
}

#endif

