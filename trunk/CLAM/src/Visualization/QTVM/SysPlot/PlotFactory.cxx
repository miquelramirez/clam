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

#include "QtAppWrapper.hxx"
#include "PlotFactory.hxx"

namespace CLAM
{
	namespace VM
	{
		QtAudioPlot* PlotFactory::GetAudioPlot(	const Audio& audio, 
												const std::string& label,
												int x, int y, int w, int h,
												bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtAudioPlot*> (create(audio,label,x,y,w,h,sc));
		}

		QtStereoAudioPlot* PlotFactory::GetStereoAudioPlot(	std::vector<Audio> data,
															const std::string& label,
															int x, int y, int w, int h,
															bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtStereoAudioPlot*> (create(data,label,x,y,w,h,sc));
		}

		QtFundFreqPlot* PlotFactory::GetFundFreqPlot(const Segment& segment,
													const std::string& label,
													int x, int y, int w, int h,
													bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtFundFreqPlot*> (create(segment,label,x,y,w,h,sc));
		}

		QtSinTracksPlot* PlotFactory::GetSinTracksPlot(	const Segment& segment,
														const std::string& label,
														int x, int y, int w, int h,
														bool sc )
		{
			QtAppWrapper::Init();
			return static_cast<QtSinTracksPlot*> (create(segment,label,x,y,w,h,sc,true));
		}


		QtSinTracksPlot* PlotFactory::GetSinTracksPlot(	const Array< SpectralPeakArray >& peakMtx,
														const TData& sr, const TData& dur,
														const std::string& label,
														int x, int y, int w, int h,
														bool sc )
		{
			QtAppWrapper::Init();
			return static_cast<QtSinTracksPlot*> (create(peakMtx,sr,dur,label,x,y,w,h,sc));
		}

		QtSpectrumPlot* PlotFactory::GetSpectrumPlot(const Spectrum& spec,
													const std::string& label,
													int x, int y, int w, int h,
													bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtSpectrumPlot*> (create(spec,label,x,y,w,h,sc));
		}

		QtSpectrumAndPeaksPlot* PlotFactory::GetSpectrumAndPeaksPlot(const Spectrum& spec,
																	const SpectralPeakArray& peaks,
																	const std::string& label,
																	int x, int y, int w, int h,
																	bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtSpectrumAndPeaksPlot*> (create(spec,peaks,label,x,y,w,h,sc));
		}

		QtMultiPlot* PlotFactory::GetMultiPlot(	const std::string& label,
												int x, int y, int w, int h,
												bool sc )
		{
			QtAppWrapper::Init();
			return static_cast<QtMultiPlot*> (create(label,x,y,w,h,sc));
		}

		QtAudioPlot* PlotFactory::GetAudioPlot(	QWidget* parent,
												const Audio& audio, 
												const std::string& label,
												int x, int y, int w, int h,
												bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtAudioPlot*> (create(audio,label,x,y,w,h,sc,parent));
		}

		QtStereoAudioPlot* PlotFactory::GetStereoAudioPlot(	QWidget* parent,
															std::vector<Audio> data,
															const std::string& label,
															int x, int y, int w, int h,
															bool sc )
		{
			QtAppWrapper::Init();
			return static_cast<QtStereoAudioPlot*> (create(data,label,x,y,w,h,sc,parent));
		}

		QtFundFreqPlot* PlotFactory::GetFundFreqPlot(QWidget* parent,
													const Segment& segment,
													const std::string& label,
													int x, int y, int w, int h,
													bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtFundFreqPlot*> (create(segment,label,x,y,w,h,sc,false,parent));
		}

		QtSinTracksPlot* PlotFactory::GetSinTracksPlot(	QWidget* parent,
														const Segment& segment,
														const std::string& label,
														int x, int y, int w, int h,
														bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtSinTracksPlot*> (create(segment,label,x,y,w,h,sc,true,parent));
		}

		QtSinTracksPlot* PlotFactory::GetSinTracksPlot(	QWidget* parent,
														const Array< SpectralPeakArray >& peakMtx,
														const TData& sr,
														const TData& dur,
														const std::string& label,
														int x, int y, int w, int h,
														bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtSinTracksPlot*> (create(peakMtx,sr,dur,label,x,y,w,h,sc,parent));
		}

		QtSpectrumPlot* PlotFactory::GetSpectrumPlot(QWidget* parent,
													const Spectrum& spec,
													const std::string& label,
													int x, int y, int w, int h,
													bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtSpectrumPlot*> (create(spec,label,x,y,w,h,sc,parent));
		}

		QtSpectrumAndPeaksPlot* PlotFactory::GetSpectrumAndPeaksPlot(QWidget* parent,
																	const Spectrum& spec,
																	const SpectralPeakArray& peaks,
																	const std::string& label,
																	int x, int y, int w, int h,
																	bool sc)
		{
			QtAppWrapper::Init();
			return static_cast<QtSpectrumAndPeaksPlot*> (create(spec,peaks,label,x,y,w,h,sc,parent));
		}

		QtMultiPlot* PlotFactory::GetMultiPlot(	QWidget* parent,
												const std::string& label,
												int x,int y, int w, int h,
												bool sc )
		{
			QtAppWrapper::Init();
			return static_cast<QtMultiPlot*> (create(label,x,y,w,h,sc,parent));
		}

		//////////////////////////////////////////////////////////////////////
		//                                                                  //
		//                      Private methods                             //
		//                                                                  //
		//////////////////////////////////////////////////////////////////////

		QtPlot* PlotFactory::create(const Audio& audio,
									const std::string& label,
									int x, int y, int w, int h,
									bool sc, QWidget* parent)
		{
			QtAudioPlot* plot = new QtAudioPlot(parent);
			plot->SetData(audio);
			plot->Label(label);
			plot->Geometry(x,y,w,h);
			plot->SwitchDisplayColors(sc);
			return plot;
		}

		QtPlot* PlotFactory::create(std::vector<Audio> data,
									const std::string& label,
									int x, int y, int w, int h,
									bool sc, QWidget* parent)
		{
			QtStereoAudioPlot* plot = new QtStereoAudioPlot(parent);
			plot->SetData(data);
			plot->Label(label);
			plot->Geometry(x,y,w,h);
			plot->SwitchDisplayColors(sc);
			return plot;
		}

		QtPlot* PlotFactory::create(const Segment& segment,
									const std::string& label,
									int x, int y, int w, int h,
									bool sc, bool st,
									QWidget* parent )
		{
			if(!st)
			{
				QtFundFreqPlot* plot = new QtFundFreqPlot(parent);
				plot->SetData(segment);
				plot->Label(label);
				plot->Geometry(x,y,w,h);
				plot->SwitchDisplayColors(sc);
				return plot;
			}
			else
			{
				QtSinTracksPlot* plot = new QtSinTracksPlot(parent);
				plot->SetData(segment);
				plot->Label(label);
				plot->Geometry(x,y,w,h);
				plot->SwitchDisplayColors(sc);
				return plot;
			}
		}

		QtPlot* PlotFactory::create(const Spectrum& spec,
									const std::string& label,
									int x, int y, int w, int h,
									bool sc, QWidget* parent)
		{
			QtSpectrumPlot* plot = new QtSpectrumPlot(parent);
			plot->SetData(spec);
			plot->Label(label);
			plot->Geometry(x,y,w,h);
			plot->SwitchDisplayColors(sc);
			return plot;
		}

		QtPlot* PlotFactory::create(const Spectrum& spec,
								    const SpectralPeakArray& peaks,
									const std::string& label,
									int x, int y, int w, int h, 
									bool sc, QWidget* parent)
		{
			QtSpectrumAndPeaksPlot* plot = new QtSpectrumAndPeaksPlot(parent);
			plot->SetData(spec,peaks);
			plot->Label(label);
			plot->Geometry(x,y,w,h);
			plot->SwitchDisplayColors(sc);
			return plot;
		}

		QtPlot* PlotFactory::create(const Array< SpectralPeakArray >& peakMtx,
									const TData& sr,
									const TData& dur,
									const std::string& label,
									int x, int y, int w, int h,
									bool sc, QWidget* parent )
		{
			QtSinTracksPlot* plot = new QtSinTracksPlot(parent);
			plot->SetData(peakMtx,sr,dur);
			plot->Label(label);
			plot->Geometry(x,y,w,h);
			plot->SwitchDisplayColors(sc);
			return plot;
		}

		QtPlot* PlotFactory::create(const std::string& label,
									int x, int y, int w, int h,
									bool sc, QWidget* parent)
		{
			QtMultiPlot* plot = new QtMultiPlot(parent);
			plot->Label(label);
			plot->Geometry(x,y,w,h);
			plot->SwitchDisplayColors(sc);
			return plot;
		}
	}
}

// END

