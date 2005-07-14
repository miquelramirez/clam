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

#include "Factory.hxx"
#include "Oscillator.hxx"
#include "SimpleOscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "AudioMixer.hxx"
#include "AutoPanner.hxx"
#include "FFT_rfftw.hxx"
#include "IFFT_rfftw.hxx"
#include "Controller.hxx"
#include "ADSR.hxx"


/**
 * In this cxx are defined the objects that will be in the factory by default. Including it 
 * in a project will let create objects of these types without include all the processings
 * headers. If a project needs some processings not included, another .cxx can be created
 * to register more processings (with the condition of avoid duplicated keys).
 * 
 * The way processings are registered in factory could be changed in a future.
 * Warning: This file has a lot of dependencies.
 */
typedef CLAM::Factory<Processing> ProcessingFactory;

static ProcessingFactory::Registrator<CLAM::SimpleOscillator> regtSimpleOscillator( "SimpleOscillator" );
static ProcessingFactory::Registrator<CLAM::Oscillator> regtOscillator( "Oscillator" );
static ProcessingFactory::Registrator<CLAM::AudioMultiplier> regtAudioMultiplier( "AudioMultiplier" );
static ProcessingFactory::Registrator<CLAM::AudioAdder> regtAudioAdder( "AudioAdder" );
static ProcessingFactory::Registrator<CLAM::AudioMixer> regtAudioMixer( "AudioMixer" );
static ProcessingFactory::Registrator<CLAM::FFT_rfftw> regtFFT_rfftw( "FFT_rfftw" );
static ProcessingFactory::Registrator<CLAM::IFFT_rfftw> regtIFFT_rfftw( "IFFT_rfftw" );
static ProcessingFactory::Registrator<CLAM::AutoPanner> regtAutoPanner( "AutoPanner" );
static ProcessingFactory::Registrator<CLAM::Controller> regtController( "Controller" );
static ProcessingFactory::Registrator<CLAM::ADSR> regtADSR( "ADSR" );

