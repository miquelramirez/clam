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

#include "FDFilterController.hxx"
#include "FDFilterGen.hxx"


namespace CLAMVM
{
	FDFilterController::FDFilterController()
	{
	}

	FDFilterController::~FDFilterController()
	{
	}

	bool FDFilterController::BindTo( Processing& proc )
	{
		ProcessingController::BindTo( proc );

		mObserved = dynamic_cast< FDFilterGen* >( &proc );

		CLAM_ASSERT( mObserved!=NULL, "Error: Trying to bind to a Processing that is not a FDFilterGen" );

		return true;

	}

	bool FDFilterController::Publish()
	{
		// Control value ranges
		// Configuration retrieval
		const CLAM::FDFilterGenConfig& cfg = dynamic_cast< const CLAM::FDFilterGenConfig& >( mObserved->GetConfig() );
		ControlAdapter* hCutoffAdapt = static_cast<ControlAdapter*>( mChildren["High Cutoff Frequency"]);
		ControlAdapter* lCutoffAdapt = static_cast<ControlAdapter*>( mChildren["Low Cutoff Frequency"]);
		ControlAdapter* gAdapt = static_cast<ControlAdapter*>( mChildren["Gain"]);

		CLAM::TData srange = cfg.GetSpectralRange();
		hCutoffAdapt->SetValueRange( 0, srange  );
		lCutoffAdapt->SetValueRange( 0, srange );
		gAdapt->SetValueRange( -70, 0 );

		ProcessingController::Publish();

		return true;		
	}
}

