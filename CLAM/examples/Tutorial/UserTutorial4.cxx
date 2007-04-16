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

/** @file UserTutorial4.cxx CLAM tutorial, example 4.
 * This example illustrates:
 *   * a more efficient way to change the value of a dynamic attribute
 *     such as an array of samples.
 *   * How to synchronize different dynamic attributes in a data object.
 */
#include <Spectrum.hxx>
#include <SpectrumConfig.hxx>
#include <Array.hxx>
#include <Complex.hxx>

int main()
{
	using namespace CLAM;

	SpecTypeFlags flags;
	flags.bMagPhase=false;
	flags.bComplex=true;
	SpectrumConfig spec_cfg;
	spec_cfg.SetSize(513);
	spec_cfg.SetType(flags);

	Spectrum spec(spec_cfg);

	// Instead of creating and initialize our own array, like in
	// example 3, we get a reference to the uninitialized complex
	// array (which already has the right size)
	Array<Complex > &cplx = spec.GetComplexArray();

	// ... and we initialize it directly. 
	for (int i=0; i<513; i++)
	{
		cplx[i].SetReal(1.0);
		cplx[i].SetImag(1.0);
	}

	// We now add the Polar array attribute using the dynamic type
	// mechanism.
	spec.AddPolarArray();
	spec.UpdateData();

	// Beware: The following is easy to forget.
	// When adding an attribute with the AddXXX() method,
	// its size is not initialised. You must do it manually:
	spec.SetSize(513);

	// We finally force the spectrum object to set the new attribute
	// to the right values. The flags argument has the bComplex member
	// set to "true", indicating that the ComplexArray attribute is
	// the one from which values will be taken.
	spec.SynchronizeTo(flags);

	return 0;
}

