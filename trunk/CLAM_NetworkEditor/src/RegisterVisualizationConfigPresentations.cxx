/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <CLAM/Factory.hxx>
#include "ProcessingConfigPresentation.hxx"
#include "ConfigPresentationTmpl.hxx"

// Graphical Controllers
#include <CLAM/OutControlSender.hxx>

// Graphical Monitoring
#include <CLAM/PortMonitor.hxx>
#include <CLAM/AudioPlotProcessing.hxx>
#include <CLAM/SpectrumPlotProcessing.hxx>
#include <CLAM/PeaksPlotProcessing.hxx>
#include <CLAM/FundPlotProcessing.hxx>
#include <CLAM/SinTracksPlotProcessing.hxx>
#include <CLAM/FundTrackPlotProcessing.hxx>
#include <CLAM/SpecgramPlotProcessing.hxx>
#include <CLAM/AudioBuffPlotProcessing.hxx>


typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::OutControlSenderConfig> > 
	regtOutControlSenderCfg( "OutControlSenderConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::PortMonitorConfig> >
       regtPortMonitorCfg("PortMonitorConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioPlotProcessingConfig> >
	regtAudioPlotProcessingCfg("AudioPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SpectrumPlotProcessingConfig> >
	regtSpectrumPlotProcessingCfg("SpectrumPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::PeaksPlotProcessingConfig> >
       regtPeaksPlotProcessingCfg("PeaksPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::FundPlotProcessingConfig> >
       regtFundPlotProcessingCfg("FundPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SinTracksPlotProcessingConfig> >
       regtSinTracksPlotProcessingCfg("SinTracksPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::FundTrackPlotProcessingConfig> >
       regtFundTrackPlotProcessingCfg("FundTrackPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SpecgramPlotProcessingConfig> >
       regtSpecgramPlotProcessingCfg("SpecgramPlotProcessingConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioBuffPlotProcessingConfig> >
       regtAudioBuffPlotProcessingCfg("AudioBuffPlotProcessingConfig");
 
