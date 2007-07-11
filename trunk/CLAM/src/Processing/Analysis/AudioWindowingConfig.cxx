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

#include "AudioWindowingConfig.hxx"
#include "CLAM_Math.hxx"

#include <iostream>

namespace CLAM
{

void AudioWindowingConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetSamplingRate(44100);
	SetWindowSize(1025);
	SetWindowType(EWindowType::eBlackmanHarris92);
	SetHopSize((GetWindowSize()-1)/2);
	SetFFTSize(1024); // the greater or equal power of 2 of windowsize-1
	SetDoHalfWindowShift( true );
}

} // namespace CLAM

