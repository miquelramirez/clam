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
#include "ConfigPresentationTmpl.hxx"
#include "AudioFileConfig.hxx"
#include "AutoPanner.hxx"
#include "AudioIO.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "BinaryAudioOp.hxx"
#include "SimpleOscillator.hxx"
#include "Oscillator.hxx"
#include "ADSR.hxx"
#include "LadspaLoader.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"

// local processings
#include "FlagControl.hxx"
#include "Random.hxx"
#include "OneOverF.hxx"
#include "OutControlSender.hxx"
#include "SquareWave.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::SimpleOscillatorConfig> > 
	regtSimpleOscillatorCfg( "SimpleOscillatorConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::OscillatorConfig> > 
	regtOscillatorCfg( "OscillatorConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::ADSRConfig> > 
	regtADSRCfg( "ADSRConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> > 
	regtAutoPannerCfg( "AutoPannerConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioFileConfig> > 
	regtAudioFileInCfg( "AudioFileConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioIOConfig> > 
	regtAudioOutCfg( "AudioIOConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioAdderConfig> > 
	regAudioAdderCfg( "AudioAdderConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::BinaryAudioOpConfig> > 
	regtBinaryAudioOpCfg( "BinaryAudioOpConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMultiplierConfig> > 
	regtAudioMultiplierCfg( "AudioMultiplierConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::LadspaLoaderConfig> > 
	regtLadspaLoaderCfg( "LadspaLoaderConfig" );

static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MonoAudioFileReaderConfig> > 
	regtMonoAudioFileReaderCfg( "MonoAudioFileReaderConfig" );
	
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::MonoAudioFileWriterConfigPresentation> 
	regtMonoAudioFileWriterCfg( "MonoAudioFileWriterConfig" );
	
static ProcessingConfigPresentationFactory::Registrator< NetworkGUI::ConfigPresentationTmpl<CLAM::MultiChannelAudioFileReaderConfig> > 
	regtMultiChannelAudioFileReaderCfg( "MultiChannelAudioFileReaderConfig" );
	
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::MultiChannelAudioFileWriterConfigPresentation> 
	regtMultiChannelAudioFileWriterCfg( "MultiChannelAudioFileWriterConfig" );

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

