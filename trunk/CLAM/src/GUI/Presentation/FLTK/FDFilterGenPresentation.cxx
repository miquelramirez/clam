/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "FDFilterGenPresentation.hxx"
#include "FDFilterGenView.hxx"
#include "GridLayout.hxx"
using namespace CLAMGUI;

void ProcObjPresentation<FDFilterGen>::UpdateFilterType( EFDFilterType type )
{
	mCurrentFilterType = type;
}

void ProcObjPresentation<FDFilterGen>::PublishLowPassCallbacks(Aspect& my_aspect)
{


	mPanel = new LPFPanel( mPresGeometry.GetX(), mPresGeometry.GetY(), mPresGeometry.GetW(), mPresGeometry.GetH() );
	
	mPanel->GetGainKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	mPanel->GetLowcutoffKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );


	mPanel->GetStopBandSlopeKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );


	ProcObjView<FDFilterGen>::Services s;
	
	s = ProcObjView< FDFilterGen >::Services::SetGain;
	my_aspect.AddRequest( s, &(mPanel->GetGainKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetStopBandSlope;
	my_aspect.AddRequest( s, &(mPanel->GetStopBandSlopeKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetLowCutoff;
	my_aspect.AddRequest( s, &(mPanel->GetLowcutoffKnob()->GetReceptionSignal()) );
	
}

void ProcObjPresentation<FDFilterGen>::PublishBandPassCallbacks( Aspect& my_aspect)
{


	mPanel = new BPFPanel( mPresGeometry.GetX(), mPresGeometry.GetY(), mPresGeometry.GetW(), mPresGeometry.GetH() );
	
	GridLayout gl( 6, 4, Rect<int>( 0, 0, mPresGeometry.GetW(), mPresGeometry.GetH() ) );
	
	
	

	mPanel->GetGainKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	mPanel->GetLowcutoffKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	mPanel->GetStopBandSlopeKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	mPanel->GetHighcutoffKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	mPanel->GetPassBandSlopeKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );

	
	ProcObjView<FDFilterGen>::Services s;
	
	s = ProcObjView< FDFilterGen >::Services::SetGain;
	my_aspect.AddRequest( s, &(mPanel->GetGainKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetHighCutoff;
	my_aspect.AddRequest( s, &(mPanel->GetHighcutoffKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetLowCutoff;
	my_aspect.AddRequest( s, &(mPanel->GetLowcutoffKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetPassBandSlope;
	my_aspect.AddRequest( s, &(mPanel->GetPassBandSlopeKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetStopBandSlope;
	my_aspect.AddRequest( s, &(mPanel->GetStopBandSlopeKnob()->GetReceptionSignal()) );
	
}

void ProcObjPresentation<FDFilterGen>::PublishHighPassCallbacks( Aspect& my_aspect)
{

	mPanel = new HPFPanel( mPresGeometry.GetX(), mPresGeometry.GetY(), mPresGeometry.GetW(), mPresGeometry.GetH() );
	
	mPanel->GetGainKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );

	mPanel->GetHighcutoffKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	mPanel->GetPassBandSlopeKnob()->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation< FDFilterGen >, NotifyView ) );
	
	
	ProcObjView<FDFilterGen>::Services s;
	
	s = ProcObjView< FDFilterGen >::Services::SetGain;
	my_aspect.AddRequest( s, &(mPanel->GetGainKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetHighCutoff;
	my_aspect.AddRequest( s, &(mPanel->GetHighcutoffKnob()->GetReceptionSignal()) );
	
	s = ProcObjView< FDFilterGen >::Services::SetPassBandSlope;
	my_aspect.AddRequest( s, &(mPanel->GetPassBandSlopeKnob()->GetReceptionSignal()) );
}

void ProcObjPresentation<FDFilterGen>::PublishCallbacks()
{
	
	
	Aspect cfg;
	
	switch ( mCurrentFilterType )
		{
		case EFDFilterType::eLowPass:
			PublishLowPassCallbacks(cfg);
			break;
		case EFDFilterType::eHighPass:
			PublishHighPassCallbacks(cfg);
			break;
		case EFDFilterType::eBandPass:
			PublishBandPassCallbacks(cfg);
			break;
		default:
			CLAM_ASSERT( false, "Unknown filter type" );
		}
	
	ProcObjView<FDFilterGen>::Services s;

	s = ProcObjView< FDFilterGen >::Services::GetGain;
	cfg.AddRequest( s, &mDrawGain );
	s = ProcObjView< FDFilterGen >::Services::GetHighCutoff;
	cfg.AddRequest( s, &mDrawHCutoff );
	s = ProcObjView< FDFilterGen >::Services::GetLowCutoff;
	cfg.AddRequest( s, &mDrawLCutoff );
	s = ProcObjView< FDFilterGen >::Services::GetPassBandSlope;
	cfg.AddRequest( s, &mDrawPBSlope );
	s = ProcObjView< FDFilterGen >::Services::GetStopBandSlope;
	cfg.AddRequest( s, &mDrawSBSlope );

	mLinkedView->Configure( cfg );
	
}

void ProcObjPresentation<FDFilterGen>::Show()
{
	
	mPanel->Show();
	
}

void ProcObjPresentation<FDFilterGen>::Init( const Geometry& g, const char* label)
{
	
	mPresGeometry = g;
	
	mDrawGain = makeMemberFunctor1( TData, *this, ProcObjPresentation<FDFilterGen>, GetGain );
	mDrawHCutoff = makeMemberFunctor1( TData, *this, ProcObjPresentation<FDFilterGen>, GetHCutoff );
	mDrawLCutoff = makeMemberFunctor1( TData, *this, ProcObjPresentation<FDFilterGen>, GetLCutoff );
	mDrawPBSlope = makeMemberFunctor1( TData, *this, ProcObjPresentation<FDFilterGen>, GetPBSlope );
	mDrawSBSlope = makeMemberFunctor1( TData, *this, ProcObjPresentation<FDFilterGen>, GetSBSlope );
	
	
}

void ProcObjPresentation<FDFilterGen>::GetGain( TData gain )
{
	
	GainKnob* pknob = mPanel->GetGainKnob();
	
	if ( pknob )
		{
			pknob->value( gain );
			pknob->Do();
		}
	
}

void ProcObjPresentation<FDFilterGen>::GetHCutoff( TData hcutoff )
{
	
	
	HighcutoffKnob* pknob = mPanel->GetHighcutoffKnob();
	
	if ( pknob )
		{
			pknob->value( hcutoff );
			pknob->Do();
		}
	
	
}

void ProcObjPresentation<FDFilterGen>::GetLCutoff( TData lcutoff )
{
	
	LowcutoffKnob* pknob = mPanel->GetLowcutoffKnob();
	if ( pknob )
		{
			pknob->value( lcutoff );
			pknob->Do();
		}
}

void ProcObjPresentation<FDFilterGen>::GetPBSlope( TData pbslope )
{
	
	SlopeKnob* pknob = mPanel->GetPassBandSlopeKnob();
	

	if ( pknob )
		{
			pknob->value( pbslope );
			pknob->Do();
		}
	
}

void ProcObjPresentation<FDFilterGen>::GetSBSlope( TData sbslope )
{
	
	SlopeKnob* pknob = mPanel->GetStopBandSlopeKnob();
	
	if ( pknob )
		{
			pknob->value( sbslope );
			pknob->Do();
		}
	
}

void ProcObjPresentation<FDFilterGen>::NotifyView()
{
	mLinkedView->NotifyUpdate();
}
