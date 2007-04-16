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

#include "TickSequenceTrackerConfig.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{

		void TickSequenceTrackerConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
			//default values
			SetSampleRate(44100.0);
			SetComputeBeats(true);
			SetNTrans(500);
			SetTransHop(450);
			SetThreshold_IOIHistPeaks(0.0);
			SetTempoLimInf(50);
			SetTempoLimSup(200);
			SetTickLimInf(0.6);
			SetTickLimSup(0.1);
			SetDeviationPenalty(1);
			SetOverSubdivisionPenalty(8);
			SetGaussianWindowSize(0.25);
			SetScope(0.10);
			SetTickAdjustForSwing(false);
			SetAdjustWithOnsets(true);
		}

	}
}

