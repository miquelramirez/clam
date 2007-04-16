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
#include <CLAM/MonoAudioFileWriterConfig.hxx>
#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileWriterConfig.hxx>
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
#include <CLAM/LPC_AutoCorrelation.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/MelFilterBank.hxx>
#include <CLAM/CepstralTransform.hxx>

// local processings
#include <CLAM/FlagControl.hxx>
#include "Random.hxx"
#include "OneOverF.hxx"
#include "SquareWave.hxx"

// Controls
#include <CLAM/Fundamental2Control.hxx>
#include <CLAM/ControlMapper.hxx>
#include <CLAM/ControlPrinter.hxx>
#include <CLAM/ControlScaler.hxx>
#include "ControlSurface.hxx"
#include <CLAM/ControlSource.hxx>
#include <CLAM/OutControlSender.hxx>

//MIDI
#include <CLAM/MIDIKeyboard.hxx>
#include <CLAM/MIDIIOConfig.hxx>
#include <CLAM/MIDIDispatcher.hxx>

// Examples
#include "FilenameUsageExample.hxx"

// concrete configs dialogs

#include "RegisterConfiguratorLaunchers.hxx"

// generators
STANDARD_PROCESSING_CONFIG_REGISTER(SimpleOscillatorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OscillatorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ADSRConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(WindowGeneratorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SquareWaveConfig);

// arithmetic
STANDARD_PROCESSING_CONFIG_REGISTER(AudioMultiplierConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioAdderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioMixerConfig);

// audio
STANDARD_PROCESSING_CONFIG_REGISTER(MonoAudioFileReaderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MonoAudioFileWriterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MultiChannelAudioFileReaderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MultiChannelAudioFileWriterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioIOConfig);

// analysis
STANDARD_PROCESSING_CONFIG_REGISTER(FFTConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(AudioWindowingConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SpectralAnalysisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SMSAnalysisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(LPCConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MelFilterBankConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(CepstralTransformConfig);

// systhesis
STANDARD_PROCESSING_CONFIG_REGISTER(IFFTConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SpectralSynthesisConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OverlapAddConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SMSSynthesisConfig);

// sms transforms
STANDARD_PROCESSING_CONFIG_REGISTER(SpectralPeakDetectConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(FundFreqDetectConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(CircularShiftConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(PhaseManagementConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SinTrackingConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SynthSineSpectrumConfig);

// misc transforms
STANDARD_PROCESSING_CONFIG_REGISTER(BinaryAudioOpConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(NullProcessingConfig);

// Controls
STANDARD_PROCESSING_CONFIG_REGISTER(AutoPannerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlPrinterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlScalerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlSourceConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlSurfaceConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(FlagControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(Fundamental2ControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OneOverFConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OutControlSenderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(RandomConfig);

// MIDI
STANDARD_PROCESSING_CONFIG_REGISTER(MIDIKeyboardConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MIDIIOConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MIDIDispatcherConfig);

#if USE_OSCPACK
#include <CLAM/OSCSender.hxx>
STANDARD_PROCESSING_CONFIG_REGISTER(OSCSenderConfig);
#endif

#ifndef WIN32
//#include "LadspaLoaderConfigPresentation.hxx" // QT4PORT
//SPECIAL_PROCESSING_CONFIG_REGISTER(LadspaLoaderConfig, NetworkGUI::LadspaLoaderConfigPresentation);
#endif

// Examples 
STANDARD_PROCESSING_CONFIG_REGISTER(FilenameUsageExampleConfig);

