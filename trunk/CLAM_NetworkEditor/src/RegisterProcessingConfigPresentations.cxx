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
#include "ConfigPresentationTmpl.hxx"

// concrete configs
#include "MonoAudioFileWriterConfigPresentation.hxx"
#include "MultiChannelAudioFileWriterConfigPresentation.hxx"
#ifndef WIN32
#include "LadspaLoaderConfigPresentation.hxx"
#endif
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
#include "NullProcessingConfig.hxx"

// local processings
#include "FlagControl.hxx"
#include "Random.hxx"
#include "OneOverF.hxx"
#include "SquareWave.hxx"

#if USE_OSCPACK
#include "OSCSender.hxx"
#endif

// Controls
#include "Fundamental2Control.hxx"
#include "ControlPrinter.hxx"
#include "ControlScaler.hxx"
#include "ControlMapper.hxx"
#include "ExternInControl.hxx"
#include "ExternOutControl.hxx"

//MIDI
#include "MIDIKeyboard.hxx"
#include "MIDIIOConfig.hxx"
#include "MIDIDispatcher.hxx"

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
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SMSAnalysisConfig> > 
	regtSmsAnalysisCfg("SMSAnalysisConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SMSSynthesisConfig> > 
	regtSmsSynthesisCfg("SMSSynthesisConfig");
	
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::MultiChannelAudioFileWriterConfigPresentation> 
	regtMultiChannelAudioFileWriterCfg( "MultiChannelAudioFileWriterConfig" );

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

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::FlagControlConfig> > 
	regtFlagControlCfg( "FlagControlConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::RandomConfig> > 
	regtRandomCfg( "RandomConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::OneOverFConfig> > 
	regtOneOverFCfg( "OneOverFConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SquareWaveConfig> > 
	regtSquareWaveCfg( "SquareWaveConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::NullProcessingConfig> > 
	regtNullProcessingCfg( "NullProcessingConfig" );
	

#if USE_OSCPACK
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::OSCSenderConfig> > 
	regtOSCSenderCfg( "OSCSenderConfig" );
#endif

// Controls
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::Fundamental2ControlConfig> >
       regtFundamental2ControlConfig("Fundamental2ControlConfig");
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::ControlPrinterConfig> >
       regtControlPrinterConfig("ControlPrinterConfig");
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::ControlScalerConfig> >
       regtControlScalerConfig("ControlScalerConfig");
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::ExternInControlConfig> >
       regtExternInControlConfig("ExternInControlConfig");
// Please note that ExterOutControl uses an already-registered NullProcessingConfig

// MIDI

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MIDIKeyboardConfig> >
       regtMIDIKeyboardConfig("MIDIKeyboardConfig");

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MIDIIOConfig> >
       regtMIDIIOConfig("MIDIIOConfig");
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MIDIDispatcherConfig> >
       regtMIDIDispatcherConfig("MIDIDispatcherConfig");

