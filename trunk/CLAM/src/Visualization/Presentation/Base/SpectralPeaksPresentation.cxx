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

#include "SpectralPeaksPresentation.hxx"
#include "SpectrumModel.hxx"
#include "SpectralPeaksModel.hxx"

namespace CLAMVM
{

	SpectrumPlusPeaksPresentation::SpectrumPlusPeaksPresentation()
	{
		SetSpectrum.Wrap( this, &SpectrumPlusPeaksPresentation::OnNewSpectrum );
		SetPartials.Wrap( this, &SpectrumPlusPeaksPresentation::OnNewPeakArray );
	}
	
	SpectrumPlusPeaksPresentation::~SpectrumPlusPeaksPresentation()
	{
	}

	void SpectrumPlusPeaksPresentation::AttachTo( SpectrumModel& specModel, SpectralPeaksModel& peaksModel )
	{
		specModel.ObjectPublished.Connect( SetSpectrum );
		
		peaksModel.ObjectPublished.Connect( SetPartials );
		
	}
	
	void SpectrumPlusPeaksPresentation::Detach()
	{
		SetSpectrum.Unbind();
		SetPartials.Unbind();
	}
	
}



