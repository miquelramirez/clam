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
#include "OscillatorConfigPresentation.hxx"
#include "ConfigPresentationTmpl.hxx"
#include "AudioFile.hxx"
#include "AutoPanner.hxx"
#include "AudioIO.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "BinaryAudioOp.hxx"
#include "SimpleOscillator.hxx"
#include "Oscillator.hxx"
#include "ADSR.hxx"

// in draft/NewProcessings
#include "FlagControl.hxx"
#include "Random.hxx"
#include "OneOverF.hxx"
//#include "LadspaLoader.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;


typedef NetworkGUI::ConfigPresentationTmpl<CLAM::OscillatorConfig> OscillatorConfigPresentationTmpl;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::SimpleOscillatorConfig> SimpleOscillatorConfigPresentationTmpl;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> AutoPannerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioFileConfig> AudioFileConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioIOConfig> AudioIOConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::BinaryAudioOpConfig> BinaryAudioOpConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::ADSRConfig> ADSRConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> AutoPannerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMultiplierConfig> AudioMultiplierConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioAdderConfig> AudioAdderConfigPresentation;
// in draft
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::FlagControlConfig> FlagControlConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::RandomConfig> RandomConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::OneOverFConfig> OneOverFConfigPresentation;
//typedef NetworkGUI::ConfigPresentationTmpl<CLAM::LadspaLoaderConfig> LadspaLoaderConfigPresentation;

static ProcessingConfigPresentationFactory::Registrator<SimpleOscillatorConfigPresentationTmpl> regtSimpleOscillatorCfg( "SimpleOscillatorConfig" );
static ProcessingConfigPresentationFactory::Registrator<OscillatorConfigPresentationTmpl> regtOscillatorCfg( "OscillatorConfig" );
static ProcessingConfigPresentationFactory::Registrator<AutoPannerConfigPresentation> regtAutoPannerCfg( "AutoPannerConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioFileConfigPresentation> regtAudioFileInCfg( "AudioFileConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioIOConfigPresentation> regtAudioOutCfg( "AudioIOConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioAdderConfigPresentation> regAudioAdderCfg( "AudioAdderConfig" );
static ProcessingConfigPresentationFactory::Registrator<BinaryAudioOpConfigPresentation> regtBinaryAudioOpCfg( "BinaryAudioOpConfig" );
static ProcessingConfigPresentationFactory::Registrator<ADSRConfigPresentation> regtADSRCfg( "ADSRConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioMultiplierConfigPresentation> regtAudioMultiplierCfg( "AudioMultiplierConfig" );

// in draft
static ProcessingConfigPresentationFactory::Registrator<FlagControlConfigPresentation> regtFlagControlCfg( "FlagControlConfig" );
static ProcessingConfigPresentationFactory::Registrator<RandomConfigPresentation> regtRandomCfg( "RandomConfig" );
static ProcessingConfigPresentationFactory::Registrator<OneOverFConfigPresentation> regtOneOverFCfg( "OneOverFConfig" );
//static ProcessingConfigPresentationFactory::Registrator<LadspaLoaderConfigPresentation> regtLadspaLoaderCfg( "LadspaLoaderConfig" );


// manual config presentation
//static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::OscillatorConfigPresentation> regOscillatorCfg( "OscillatorConfig" );
