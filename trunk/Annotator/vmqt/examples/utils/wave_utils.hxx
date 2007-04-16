/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __WAVE_UTILS__
#define __WAVE_UTILS__

#include "Audio.hxx"

using CLAM::TData;
using CLAM::Audio;

namespace qtvm_examples_utils
{
    void chirp(const double& freq0, const double& freq1, const double& duration, const double& sampleRate, Audio& out);
    void sine(const TData& freq, const TData& amplitude, const TData& duration, const TData& sampleRate, Audio& out);
}

#endif

