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
#include "Controller.hxx"
#include "AudioFile.hxx"
#include "AutoPanner.hxx"
#include "AudioIO.hxx"
//#include "AudioMixer.hxx"
#include "BinaryAudioOp.hxx"
#include "Oscillator.hxx"
#include "FFTConfig.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;


typedef NetworkGUI::ConfigPresentationTmpl<CLAM::OscillatorConfig> OscillatorConfigPresentationTmpl;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::ControllerConfig> ControllerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> AutoPannerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioFileConfig> AudioFileConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioIOConfig> AudioIOConfigPresentation;
//typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMixerConfig> AudioMixerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::BinaryAudioOpConfig> BinaryAudioOpConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::FFTConfig> FFTConfigPresentation;

static ProcessingConfigPresentationFactory::Registrator<OscillatorConfigPresentationTmpl> regtOscillatorCfg( "OscillatorConfig" );
static ProcessingConfigPresentationFactory::Registrator<ControllerConfigPresentation> regtControllerCfg( "ControllerConfig" );
static ProcessingConfigPresentationFactory::Registrator<AutoPannerConfigPresentation> regtAutoPannerCfg( "AutoPannerConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioFileConfigPresentation> regtAudioFileInCfg( "AudioFileConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioIOConfigPresentation> regtAudioOutCfg( "AudioIOConfig" );
//static ProcessingConfigPresentationFactory::Registrator<AudioMixerConfigPresentation> regAudioMixerCfg( "AudioMixerConfig" );
static ProcessingConfigPresentationFactory::Registrator<BinaryAudioOpConfigPresentation> regtBinaryAudioOpCfg( "BinaryAudioOpConfig" );
static ProcessingConfigPresentationFactory::Registrator<FFTConfigPresentation> regtFFTCfg( "FFTConfig" );


// manual config presentation
//static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::OscillatorConfigPresentation> regOscillatorCfg( "OscillatorConfig" );
