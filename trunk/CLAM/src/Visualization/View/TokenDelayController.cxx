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

#include "TokenDelayController.hxx"
#include "Spectrum.hxx"
#include "TokenDelay.hxx"

namespace CLAMVM
{
	using CLAM::TData;

	SpectralTokenDelayController::SpectralTokenDelayController()
	{
	}

	SpectralTokenDelayController::~SpectralTokenDelayController()
	{
	}

	bool SpectralTokenDelayController::BindTo( Processing& proc )
	{
		ProcessingController::BindTo( proc );

		mObserved = dynamic_cast<TokenDelay<Spectrum>* >( &proc );

		CLAM_ASSERT( mObserved!=NULL, "Error: Trying to bind to a Processing that is not a TokenDelay<Spectrum>" );

		return true;
	}

	bool SpectralTokenDelayController::Publish()
	{

		ControlAdapter* adap = static_cast<ControlAdapter*> (mChildren["Delay Control"]);
				
		TData max = mObserved->MaxDelay();

		adap->SetValueRange( 0, max );
				
		ProcessingController::Publish();

		return true;
	}
}

