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

#include "Factory.hxx"
#include "ProcessingConfigPresentation.hxx"

// concrete configs
#include "MonoAudioFileWriterConfigPresentation.hxx"
#include "MultiChannelAudioFileWriterConfigPresentation.hxx"
#ifndef WIN32
#include "LadspaLoaderConfigPresentation.hxx"
#endif
#include "ConfigPresentationTmpl.hxx"
#include "AudioFileConfig.hxx"
#include "AutoPanner.hxx"
#include "AudioIO.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "AudioMixer.hxx"
#include "BinaryAudioOp.hxx"
#include "SimpleOscillator.hxx"
#include "Oscillator.hxx"
#include "ADSR.hxx"
#include "FFTConfig.hxx"
#include "IFFTConfig.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "SMSAnalysisConfig.hxx"
#include "SMSSynthesisConfig.hxx"
#include "SpectralPeakDetectConfig.hxx"
#include "FundFreqDetectConfig.hxx"
#include "SinTrackingConfig.hxx"
#include "SynthSineSpectrumConfig.hxx"
#include "PhaseManagementConfig.hxx"
#include "WindowGeneratorConfig.hxx"
#include "CircularShiftConfig.hxx"

// local processings
#include "FlagControl.hxx"
#include "Random.hxx"
#include "OneOverF.hxx"
#include "OutControlSender.hxx"
#include "SquareWave.hxx"

// plots
#include "PortMonitor.hxx"
#include "AudioPlotProcessing.hxx"
#include "SpectrumPlotProcessing.hxx"
#include "PeaksPlotProcessing.hxx"
#include "FundPlotProcessing.hxx"
#include "SinTracksPlotProcessing.hxx"

//SMSTransformations
#include "SMSFreqShift.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SimpleOscillatorConfig> > 
	regtSimpleOscillatorCfg( "SimpleOscillatorConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::OscillatorConfig> > 
	regtOscillatorCfg( "OscillatorConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::ADSRConfig> > 
	regtADSRCfg( "ADSRConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> > 
	regtAutoPannerCfg( "AutoPannerConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioIOConfig> > 
	regtAudioOutCfg( "AudioIOConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioAdderConfig> > 
	regAudioAdderCfg( "AudioAdderConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMixerConfig> > 
	regAudioMixerCfg( "AudioMixerConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::BinaryAudioOpConfig> > 
	regtBinaryAudioOpCfg( "BinaryAudioOpConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMultiplierConfig> > 
	regtAudioMultiplierCfg( "AudioMultiplierConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::FFTConfig> > 
	regtFFTCfg( "FFTConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::IFFTConfig> > 
	regtIFFTCfg( "IFFTConfig" );

#ifndef WIN32
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::LadspaLoaderConfigPresentation >
	regtLadspaLoaderCfg( "LadspaLoaderConfig" );
#endif

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MonoAudioFileReaderConfig> > 
	regtMonoAudioFileReaderCfg( "MonoAudioFileReaderConfig" );
	
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::MonoAudioFileWriterConfigPresentation> 
	regtMonoAudioFileWriterCfg( "MonoAudioFileWriterConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MultiChannelAudioFileReaderConfig> > 
	regtMultiChannelAudioFileReaderCfg( "MultiChannelAudioFileReaderConfig" );
	
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::MultiChannelAudioFileWriterConfigPresentation> 
	regtMultiChannelAudioFileWriterCfg( "MultiChannelAudioFileWriterConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SMSAnalysisConfig> > 
	regtSMSAnalysisCfg( "SMSAnalysisConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SMSSynthesisConfig> > 
	regtSMSSynthesisCfg( "SMSSynthesisConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::SpectralPeakDetectConfig> > 
	regtSpectralPeakDetectCfg( "SpectralPeakDetectConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::FundFreqDetectConfig> > 
	regtFundFreqDetectCfg( "FundFreqDetectConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::SinTrackingConfig> > 
	regtSinTrackingCfg( "SinTrackingConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::SynthSineSpectrumConfig> > 
	regtSynthSineSpectrumCfg( "SynthSineSpectrumConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::PhaseManagementConfig> > 
	regtPhaseManagementCfg( "PhaseManagementConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::WindowGeneratorConfig> > 
	regtWindowGeneratorCfg( "WindowGeneratorConfig" );

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::ConfigPresentationTmpl<CLAM::CircularShiftConfig> > 
	regtCircularShiftCfg( "CircularShiftConfig" );

// in draft
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::FlagControlConfig> > 
	regtFlagControlCfg( "FlagControlConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::RandomConfig> > 
	regtRandomCfg( "RandomConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::OneOverFConfig> > 
	regtOneOverFCfg( "OneOverFConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SquareWaveConfig> > 
	regtSquareWaveCfg( "SquareWaveConfig" );

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
 
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SMSTransformationConfig> >
       regtSMSTransformationConfig("SMSTransformationConfig");


