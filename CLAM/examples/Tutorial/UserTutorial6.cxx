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

/** @file UserTutorial6.cxx CLAM tutorial, example 6.
 *  Ilustrates how to use a processing object with controls.
 */
#include <FDFilterGen.hxx>
#include <Spectrum.hxx>

int main()
{
	using namespace CLAM;

	FDFilterGenConfig gconf;

	gconf.SetType(EFDFilterType::eLowPass);
	gconf.SetSpectralRange(4000.0);
	gconf.SetGain(1.0);
	gconf.SetStopBandSlope(12.0);

	FDFilterGen generator(gconf);
	
	Spectrum filter;
	filter.AddMagBPF();
	filter.AddPhaseBPF();
	filter.RemoveMagBuffer();
	filter.RemovePhaseBuffer();
	filter.UpdateData();
	
	generator.Start();

	generator.GetInControl("Low Cutoff Frecuency").DoControl(1000.0); // Low Cutoff

	generator.Do(filter);

	return 0;
}

