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
#include "ConfiguratorLauncher.hxx"

// concrete configs
//#include "MonoAudioFileWriterConfigPresentation.hxx" // QT4PORT
//#include "MultiChannelAudioFileWriterConfigPresentation.hxx" // QT4PORT
#ifndef WIN32
//#include "LadspaLoaderConfigPresentation.hxx" // QT4PORT
#endif
#include <CLAM/AudioFileConfig.hxx>
#include <CLAM/AutoPanner.hxx>
#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioMultiplier.hxx>
#include <CLAM/AudioAdder.hxx>
#include <CLAM/AudioMixer.hxx>
#include <CLAM/BinaryAudioOp.hxx>
#include <CLAM/SimpleOscillator.hxx>
#include <CLAM/Oscillator.hxx>
#include <CLAM/ADSR.hxx>
#include <CLAM/FFTConfig.hxx>
#include <CLAM/IFFTConfig.hxx>
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/SpectralAnalysisConfig.hxx>
#include <CLAM/SpectralSynthesisConfig.hxx>
#include <CLAM/OverlapAddConfig.hxx>
#include <CLAM/SMSAnalysisConfig.hxx>
#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/SpectralPeakDetectConfig.hxx>
#include <CLAM/FundFreqDetectConfig.hxx>
#include <CLAM/SinTrackingConfig.hxx>
#include <CLAM/SynthSineSpectrumConfig.hxx>
#include <CLAM/PhaseManagementConfig.hxx>
#include <CLAM/WindowGeneratorConfig.hxx>
#include <CLAM/CircularShiftConfig.hxx>
#include <CLAM/NullProcessingConfig.hxx>
#include <CLAM/OutControlSender.hxx>

// local processings
#include <CLAM/FlagControl.hxx>
//#include "Random.hxx" // QT4PORT
//#include "OneOverF.hxx" // QT4PORT
//#include "SquareWave.hxx" //QT4PORT

#if USE_OSCPACK
#include <CLAM/OSCSender.hxx>
#endif

// Controls
#include <CLAM/Fundamental2Control.hxx>
#include <CLAM/ControlPrinter.hxx>
#include <CLAM/ControlScaler.hxx>
#include <CLAM/ControlMapper.hxx>
#include <CLAM/ExternInControl.hxx>
#include <CLAM/ExternOutControl.hxx>

//MIDI
#include <CLAM/MIDIKeyboard.hxx>
#include <CLAM/MIDIIOConfig.hxx>
#include <CLAM/MIDIDispatcher.hxx>

typedef CLAM::Factory<ConfiguratorLauncher> ProcessingConfigPresentationFactory;

// Convenient macros (Not to use widely)
#define STANDARD_PROCESSING_CONFIG_REGISTER(configName) \
   	static ProcessingConfigPresentationFactory::Registrator<  \
		TypedConfiguratorLauncher< CLAM::configName > > reg##configName(#configName)
#define SPECIAL_PROCESSING_CONFIG_REGISTER(configName, confingurator) \
	static ProcessingConfigPresentationFactory::Registrator<configurator> \
		regt##configName( #configName)

STANDARD_PROCESSING_CONFIG_REGISTER(SimpleOscillatorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OscillatorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ADSRConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AutoPannerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioIOConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioAdderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioMixerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(BinaryAudioOpConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioMultiplierConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(FFTConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(IFFTConfig);
#ifndef WIN32
//SPECIAL_PROCESSING_CONFIG_REGISTER(LadspaLoaderConfig, NetworkGUI::LadspaLoaderConfigPresentation);
#endif
STANDARD_PROCESSING_CONFIG_REGISTER(MonoAudioFileReaderConfig);
//SPECIAL_PROCESSING_CONFIG_REGISTER(MonoAudioFileWriterConfig, NetworkGUI::MonoAudioFileWriterConfigPresentation);
STANDARD_PROCESSING_CONFIG_REGISTER(MultiChannelAudioFileReaderConfig);
//SPECIAL_PROCESSING_CONFIG_REGISTER(MultiChannelAudioFileWriterConfigPresentation, NetworkGUI::MultiChannelAudioFileWriterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SpectralAnalysisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SpectralSynthesisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OverlapAddConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SMSAnalysisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SMSSynthesisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SpectralPeakDetectConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(FundFreqDetectConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SinTrackingConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SynthSineSpectrumConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(PhaseManagementConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(WindowGeneratorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(CircularShiftConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(FlagControlConfig);
//STANDARD_PROCESSING_CONFIG_REGISTER(RandomConfig) // QT4PORT;
//STANDARD_PROCESSING_CONFIG_REGISTER(OneOverFConfig); // QT4PORT
//STANDARD_PROCESSING_CONFIG_REGISTER(SquareWaveConfig); // QT4PORT
STANDARD_PROCESSING_CONFIG_REGISTER(NullProcessingConfig);
#if USE_OSCPACK
STANDARD_PROCESSING_CONFIG_REGISTER(OSCSenderConfig);
#endif

// Controls
STANDARD_PROCESSING_CONFIG_REGISTER(Fundamental2ControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlPrinterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlScalerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ExternInControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ExternOutControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OutControlSenderConfig);
// MIDI
STANDARD_PROCESSING_CONFIG_REGISTER(MIDIKeyboardConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MIDIIOConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MIDIDispatcherConfig);

