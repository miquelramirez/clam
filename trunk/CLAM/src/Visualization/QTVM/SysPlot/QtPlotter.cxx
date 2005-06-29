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

#include <iostream>
#include <qworkspace.h>
#include "QtAppWrapper.hxx"
#include "QtPlotter.hxx"

namespace CLAM
{
    namespace VM
    {
		QtPlotter::PlotMap QtPlotter::mPlotMap;
		QtPlotter::PlotList QtPlotter::mPlotList;
		QtPlotter::MultiPlot QtPlotter::mMultiPlot;
		QtPlotter::PlotIndex QtPlotter::mPlotIndex = 0;
		QtPlotter::Workspace QtPlotter::ws;
		QtPlotter::HaveWorkspace QtPlotter::mHaveWorkspace = false;
		const unsigned wflags = Qt::WStyle_Customize | Qt::WStyle_Title | Qt::WStyle_SysMenu | Qt::WStyle_Minimize;

		void QtPlotter::Add(const std::string& plotKey,  const Audio& data)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey,  const Audio& data)",plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtAudioPlot* plot = new QtAudioPlot(ws,0,wflags);
			plot->SetData(data);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot);
		}

		void QtPlotter::Add(const std::string& plotKey, const Audio& leftChannel, const Audio& rightChannel)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey, const Audio& leftChannel, const Audio& rightChannel)", plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtAudioStereoPlot* plot = new QtAudioStereoPlot(ws,0,wflags);
			plot->SetData(leftChannel,rightChannel);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot);
		}

		void QtPlotter::Add(const std::string& plotKey, const Segment& data, WhichPlot type)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg(":Add(const std::string& plotKey, const Segment& data, WhichPlot type)",plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			if(type == CLAM::VM::FundamentalPlot)
			{
				QtFundFreqPlot* plot = new QtFundFreqPlot(ws,0,wflags);
				plot->SetData(data);
				plot->SwitchDisplayColors(true);
				Register(plotKey,plot);
			}
			else if(type == CLAM::VM::SinTracksPlot)
			{
				QtSinTracksPlot* plot = new QtSinTracksPlot(ws,0,wflags);
				plot->SetData(data);
				plot->SwitchDisplayColors(true);
				Register(plotKey,plot);
			}
		}

		void QtPlotter::Add(const std::string& plotKey, const Spectrum& data)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey, const Spectrum& data)",plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtSpectrumPlot* plot = new QtSpectrumPlot(ws,0,wflags);
			plot->SetData(data);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot);
		}

		void QtPlotter::Add(const std::string& plotKey, const Spectrum& spec, const SpectralPeakArray& peaks)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey, const Spectrum& spec, const SpectralPeakArray& peaks)", plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtSpectrumPlot* plot = new QtSpectrumPlot(ws,0,wflags);
			plot->SetData(spec,peaks);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot);
		}

		void QtPlotter::Add(const std::string& plotKey, const Array<Spectrum>& data, const TData& duration)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey, const Array<Spectrum>& data, const TData& duration)", plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtSpectrogram* plot = new QtSpectrogram(ws,0,wflags);
			plot->SetData(data,duration);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot);
		}

		void QtPlotter::Add(const std::string& plotKey, const std::string& dataKey, const DataArray& data)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey, const std::string& dataKey, const DataArray& data)", plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtMultiPlot* plot = new QtMultiPlot(ws,0,wflags);
			plot->AddData(dataKey,data);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot,true);
		}

		void QtPlotter::Add(const std::string& plotKey, 
							const Array<SpectralPeakArray>& data, 
							const TData& sampleRate, 
							const TData& duration)
		{
			if(Exist(plotKey))
			{
				DuplicatedKeyMsg("Add(const std::string& plotKey, const Array<SpectralPeakArray>& data, const TData sampleRate, const TData& duration)",plotKey);
				return;
			}
			QtAppWrapper::Init();
			if(!mHaveWorkspace) CreateWorkspace();
			QtSinTracksPlot* plot = new QtSinTracksPlot(ws,0,wflags);
			plot->SetData(data,sampleRate,duration);
			plot->SwitchDisplayColors(true);
			Register(plotKey,plot);
		}

		void QtPlotter::SetLabel(const std::string& plotKey, const std::string& label)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetLabel(const std::string& plotKey, const std::string& label)",plotKey);
				return;
			}
			mPlotList[mPlotMap[plotKey]]->Label(label);
		}

		void QtPlotter::SetMarks(const std::string& plotKey, std::vector<unsigned>& marks)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetMarks(const std::string& plotKey, std::vector<unsigned>& marks)",plotKey);
				return;
			}
			mPlotList[mPlotMap[plotKey]]->SetMarks(marks);
		}

		void QtPlotter::Run()
		{
			ws->show();
			ws->cascade();
			QtAppWrapper::Run();
		}

		void QtPlotter::AddData(const std::string& plotKey, const std::string& dataKey, const DataArray& data)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("AddData(const std::string& plotKey, const std::string& dataKey, const DataArray& data)",plotKey);
				return;
			}
			if(!IsMultiPlot(mPlotMap[plotKey]))
			{
				NonMultiPlotMsg("AddData(const std::string& plotKey, const std::string& dataKey, const DataArray& data)", plotKey);
				return;
			}
			((QtMultiPlot*)mPlotList[mPlotMap[plotKey]])->AddData(dataKey,data);
		}

		void QtPlotter::SetDataColor(const std::string& plotKey, const std::string& dataKey, Color c)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetDataColor(const std::string& plotKey, const std::string& dataKey, Color c)",plotKey);
				return;
			}
			if(!IsMultiPlot(mPlotMap[plotKey]))
			{
				NonMultiPlotMsg("SetDataColor(const std::string& plotKey, const std::string& dataKey, Color c)",plotKey);
				return;
			}
			((QtMultiPlot*)mPlotList[mPlotMap[plotKey]])->SetColor(dataKey,c);
		}

		void QtPlotter::SetXRange(const std::string& plotKey, const TData& xmin, const TData& xmax)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetXRange(const std::string& plotKey, const TData& xmin, const TData& xmax)",plotKey);
				return;
			}
			if(!IsMultiPlot(mPlotMap[plotKey]))
			{
				NonMultiPlotMsg("SetXRange(const std::string& plotKey, const TData& xmin, const TData& xmax)",plotKey);
				return;
			}
			((QtMultiPlot*)mPlotList[mPlotMap[plotKey]])->SetXRange(xmin,xmax);
		}

		void QtPlotter::SetYRange(const std::string& plotKey, const TData& ymin, const TData& ymax)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetYRange(const std::string& plotKey, const TData& ymin, const TData& ymax)",plotKey);
				return;
			}
			if(!IsMultiPlot(mPlotMap[plotKey]))
			{
				NonMultiPlotMsg("SetYRange(const std::string& plotKey, const TData& ymin, const TData& ymax)",plotKey);
				return;
			}
			((QtMultiPlot*)mPlotList[mPlotMap[plotKey]])->SetYRange(ymin,ymax);
		}

		void QtPlotter::SetUnits(const std::string& plotKey, const std::string& xunits, const std::string& yunits)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetUnits(const std::string& plotKey, const std::string& xunits, const std::string& yunits)",plotKey);
				return;
			}
			if(!IsMultiPlot(mPlotMap[plotKey]))
			{
				NonMultiPlotMsg("SetUnits(const std::string& plotKey, const std::string& xunits, const std::string& yunits)",plotKey);
				return;
			}
			((QtMultiPlot*)mPlotList[mPlotMap[plotKey]])->SetUnits(xunits,yunits);
		}

		void QtPlotter::SetToolTips(const std::string& plotKey, const std::string& xtooltip, const std::string& ytooltip)
		{
			if(!Exist(plotKey))
			{
				NonExistentKeyMsg("SetToolTips(const std::string& plotKey, const std::string& xtooltip, const std::string& ytooltip)",plotKey);
				return;
			}
			if(!IsMultiPlot(mPlotMap[plotKey]))
			{
				NonMultiPlotMsg("SetToolTips(const std::string& plotKey, const std::string& xtooltip, const std::string& ytooltip)",plotKey);
				return;
			}
			((QtMultiPlot*)mPlotList[mPlotMap[plotKey]])->SetToolTips(xtooltip,ytooltip);
		}

		bool QtPlotter::IsMultiPlot(const int& index)
		{
			return mMultiPlot[index];
		}

		bool QtPlotter::Exist(const std::string& key)
		{
			return mPlotMap.find(key) != mPlotMap.end();
		}

		void QtPlotter::CreateWorkspace()
		{
			ws = new QWorkspace(0);
			ws->setCaption("QtPlotter");
			ws->setScrollBarsEnabled(true);
			mHaveWorkspace = true;
		}

		void QtPlotter::Register(const std::string& pKey, QtPlot* plot, bool isMultiPlot)
		{
			plot->setMinimumSize(500,200);
			mPlotMap[pKey] = mPlotIndex++;
			mPlotList.push_back(plot);
			mMultiPlot.push_back(isMultiPlot);
		}

		void QtPlotter::NonExistentKeyMsg(const std::string& methodName, const std::string& plotKey)
		{
			std::cout << "QtPlotter::" << methodName << ": nonexistent key " << plotKey << "." << std::endl;
		}

		void QtPlotter::DuplicatedKeyMsg(const std::string& methodName, const std::string& plotKey)
		{
			std::cout << "QtPlotter::" << methodName << ": duplicated key " << plotKey << "." << std::endl;
		}

		void QtPlotter::NonMultiPlotMsg(const std::string& methodName, const std::string& plotKey)
		{
			std::cout << "QtPlotter::" << methodName << ": " << plotKey << " is not a QtMultiPlot object." << std::endl;
		}
    }
}

// END

