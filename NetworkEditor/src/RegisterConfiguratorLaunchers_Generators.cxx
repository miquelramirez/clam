
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
#include "RegisterConfiguratorLaunchers.hxx"

// generators
#include <CLAM/SimpleOscillator.hxx>
#include <CLAM/Oscillator.hxx>
#include <CLAM/ADSR.hxx>
#include "SquareWave.hxx"
#include <CLAM/WindowGeneratorConfig.hxx>
STANDARD_PROCESSING_CONFIG_REGISTER(SimpleOscillatorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OscillatorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ADSRConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(WindowGeneratorConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(SquareWaveConfig);
