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

#ifndef __PLOTS__
#define __PLOTS__

/**
 *  @file Plots.hxx
 *  
 *  Plots provide to developers means for adding
 *  simple but rapid, data visualization services,
 *  to their applications. Their main purpose is to
 *  become a debugging aid, so they should not be used
 *  in "production" code.
 *
 *  This header is provided just for convenience: advanced users,
 *  concerned with build times, should just include in their
 *  code the interfaces for the plots they are going to use.
 *  Also there is included the Plot interface offered by previous
 *  CLAM versions - marked as 'deprecated'. This means that this 
 *  interface will disappear in next CLAM releases - so don't expect
 *  any maintenance to be done on that.
 */

#include "Plot.hxx"
#include "MultiPlot.hxx"
#include "AudioPlot.hxx"
#include "SpectrumPlot.hxx"
#include "SpectrumAndPeaksPlot.hxx"
#include "SinTracksPlot.hxx"
#include "FundFreqPlot.hxx"
#include "SystemPlots.hxx"

/** @deprecated CLAM version <= 0.5.3 plots
 */
#include "OldPlots.hxx"


#endif // Plots.hxx
