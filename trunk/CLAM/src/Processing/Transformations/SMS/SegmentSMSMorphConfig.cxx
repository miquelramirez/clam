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

#include "SegmentSMSMorphConfig.hxx"

namespace CLAM
{
	void SegmentSMSMorphConfig::DefaultInit()
	{
		AddAll();
		RemoveFileName();
		UpdateData();
		DefaultValues();
	}

	void SegmentSMSMorphConfig::DefaultValues()
	{
		BPF tmpBPF(2);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(1,1);
		tmpBPF.SetXValue(1,1);
		SetHybBPF(tmpBPF);

		SetInterpolateFrame(true);
		SetHybSinFreq(GetHybBPF());
		SetHybSinAmp(GetHybBPF());
		SetHybPitch(GetHybSinFreq());

		SetHybResAmp(GetHybBPF());

		SetSynchronizeTime(tmpBPF);

		BPF defaultWeights;
		defaultWeights.Insert( 0.0, 0.5 );
		defaultWeights.Insert( 1.0, 0.5 );

		SetHybSinShapeW1( defaultWeights );
		SetHybSinShapeW2( defaultWeights );
		
		SetHybResShapeW1( defaultWeights );
		SetHybResShapeW2( defaultWeights );

		BPF defaultShapeWeights;
		defaultShapeWeights.Insert( 0.0, 1.0 );
		defaultShapeWeights.Insert( 1.0, 1.0 );


		SetHybSinSpectralShape( defaultShapeWeights );
		SetHybResSpectralShape( defaultShapeWeights );

		SetUseSpectralShapes ( false );

		SetSamplingRate(44100);
	}
}

