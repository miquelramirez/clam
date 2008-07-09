/*
 * Copyright (c) 2004 UNIVERSITAT POMPEU FABRA
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

#include "AudioMultiplier.hxx"
#include "AudioAmplifier.hxx"
#include "Oscillator.hxx"
#include "ProcessingClass2Ladspa.hxx"
#include "NetworkLADSPAPlugin.hxx"

static CLAM::LadspaLibrary library;

extern "C" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	static CLAM::LadspaProcessingExporter<CLAM::Oscillator> a1(library, 3000);
	static CLAM::LadspaProcessingExporter<CLAM::AudioMultiplier> a2(library, 3001);
	static CLAM::LadspaProcessingExporter<CLAM::AudioAmplifier> a3(library, 3002);
	static CLAM::LadspaNetworkExporter n1(library);
	return library.pluginAt(index);
}






