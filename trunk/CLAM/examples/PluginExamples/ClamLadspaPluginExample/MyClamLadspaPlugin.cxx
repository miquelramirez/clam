/*
 * Copyright (c) 2004 UNIVERSITAT POMPEU FABRA
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

#include <CLAM/AudioMultiplier.hxx>
#include <CLAM/AudioAmplifier.hxx>
#include <CLAM/Oscillator.hxx>
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaProcessingExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>

static CLAM::LadspaLibrary library;

CLAM_EXTERNAL_FILE_DATA(genderChange,"PluginExamples/ClamLadspaPluginExample/genderChange.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(wire,"PluginExamples/ClamLadspaPluginExample/wire.clamnetwork")

extern "C" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	static CLAM::LadspaProcessingExporter<CLAM::Oscillator> a1(library, 3000,
			"CLAM Team <clam@iua.upf.edu>", "GPL");
	static CLAM::LadspaProcessingExporter<CLAM::AudioMultiplier> a2(library, 3001,
			"CLAM Team <clam@iua.upf.edu>", "GPL");
	static CLAM::LadspaProcessingExporter<CLAM::AudioAmplifier> a3(library, 3002,
			"CLAM Team <clam@iua.upf.edu>", "GPL");
	static CLAM::LadspaNetworkExporter n1(library, genderChange, 3003,
			"clamGenderChange0", "CLAM Gender Change",
			"CLAM Team <clam@iua.upf.edu>", "GPL");
	static CLAM::LadspaNetworkExporter n2(library, wire, 3004,
			"clamWire0", "CLAM Wire",
			"CLAM Team <clam@iua.upf.edu>", "GPL");
	return library.pluginAt(index);
}



