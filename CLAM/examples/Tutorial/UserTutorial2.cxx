/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

/** @file UserTutorial2.cxx
 * Tutorial example ilustrating the creation and configuration
 * of processing data objects, with basic usage of dynamic
 * attributes, and also using the flags attribute.
 */

#include <Spectrum.hxx>
#include <SpectrumConfig.hxx>
#include <iostream>

int main()
{
	// We just need to declare the data object
	CLAM::Spectrum spec;
	// And we can start accessing its attributes
	spec.SetSpectralRange(4000);
	if ( spec.GetScale() != CLAM::EScale::eLinear )
	{
		std::cout << "This can not happen!" << std::endl
				  << "Spectrums are linear by default!" << std::endl;
	}

	// SECOND _PART_

	// This "tags" the "Complex" dynamic attribute for addition.
	spec.AddComplexArray();

	// This "tags" the "Polar" dynamic attribute for addition.
	spec.AddPolarArray();

	// This "tags" the default Magnitude and phase attributes
	// for removal.
	spec.RemoveMagBuffer();
	spec.RemovePhaseBuffer();

	// This actually performs the previous addition/removal 
	// operations.
	spec.UpdateData();

	// Finally, we set the size for the spectrum object, so that all
	// the added attributes (ComplexArray and PolarArray, in this
	// example) get resized.
	spec.SetSize(1024);

	// THIRD _PART_

	// SECOND ALTERNATIVE: creation of the spec2 object using type flags.

	// First we specify the desired attributes.
	CLAM::SpecTypeFlags flags;
	flags.bComplex=1;
	flags.bPolar=1;
	flags.bMagPhase=0;

	// Now we build the settings object
	CLAM::SpectrumConfig cfg2;
	cfg2.SetType(flags);
	cfg2.SetSize(1024);
	cfg2.SetSpectralRange(4000);

	// spec2 will have the same attributes as spec.
	// Note that when using this method the added attributes
	// already have the right size.
	CLAM::Spectrum spec2(cfg2);

	return 0;
}

