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

#include "POBrowser.hxx"
#include "GridLayout.hxx"
#include "POIcon.hxx"
#include "PORepository.hxx"
#include "DebuggerGUI.hxx"
#include "CBL.hxx"
#include "Processing.hxx"
#include <list>
#include <iostream>
using std::cout;
using std::endl;
using namespace CLAMGUI;

POBrowser::POBrowser()
: mWindow(NULL), mScroll(NULL ), mRepository( NULL )
{
}

POBrowser::~POBrowser()
{
	if ( mWindow!= NULL )
		delete mWindow;
	if ( mRepository != NULL )
		delete mRepository;
}

void POBrowser::SetType( const std::string& s )
{
	mPOType = s;
}

void POBrowser::BuildWidgets( const Rect<int>& scrspace )
{
	GridLayout layout( 1, 1, scrspace );

	Rect<int> scrollLogicalCoords( 0, 0, 1, 1 );
	Rect<int> physCoords;

	mWindow = new Fl_Double_Window( scrspace.GetX(), scrspace.GetY(), scrspace.GetW(), scrspace.GetH(), mPOType.c_str() );

	physCoords = layout << scrollLogicalCoords;

	mScroll = new ObjectScroller( 6, physCoords, mPOType.c_str() );

	mWindow->end();
}

void POBrowser::ShowAll()
{
	ShowPresentPO();
	mWindow->show();
	mScroll->show();
}

void POBrowser::HideAll()
{
	mWindow->hide();
	mScroll->hide();
}

void POBrowser::ShowPresentPO()
{
	if ( mRepository == NULL )
	{
		mRepository = new PORepository;
		mRepository->AcquireSystem();
	}
	else
		mRepository->RefreshRepository();


	// TODO: Create POIcons as needed

	const	std::list< Processing* >& po_list = mRepository->GetThese( mPOType.c_str() );

	std::list< Processing* >::const_iterator i = po_list.begin();

	POIcon* placeholder;

	mScroll->RemoveAllObjects();

	while ( i != po_list.end() )
	{
		placeholder = new POIcon;
		placeholder->SetObjectName( (*i)->GetName().c_str() );
		placeholder->SetAvatar( (*i) );
		placeholder->SetActionCallback( CBL::makeFunctor( (CBL::Functor1< Processing* >*)0, &DebuggerGUI::MakeViewFor ));
		placeholder->SetSelectionCallback( CBL::makeFunctor( (CBL::Functor1<std::string& >*)0, *mScroll, &ObjectScroller::HandleLighting ) );
		mScroll->AddObject( placeholder );
		i++;
	}

}
