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

#include "ClassesPanel.hxx"
#include "GridLayout.hxx"
#include "ObjectIcon.hxx"
#include "PORepository.hxx"
#include "CBL.hxx"
#include "DebuggerGUI.hxx"
#include <list>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using namespace CLAMGUI;


void ClassesPanel::BuildWidgets( const Rect<int>& scrspace )
{
	GridLayout layout( 1, 1, scrspace );

	Rect<int> scrollLogicalCoords( 0, 0, 1, 1 );
	Rect<int> physCoords;

	mWindow = new Fl_Double_Window( scrspace.GetX(), scrspace.GetY(), scrspace.GetW(), scrspace.GetH(), "Classes present" );

	physCoords = layout << scrollLogicalCoords;

	mScroll = new ObjectScroller( 8, physCoords, "Classes present" );

	mWindow->end();
}


ClassesPanel::~ClassesPanel()
{

	if ( mRepository!=NULL )
		 delete mRepository;

	if ( mWindow!= NULL )
	{
		delete mWindow;
	}
}


void ClassesPanel::ShowAll()
{
	ShowPresentClasses();
	mWindow->show();
	mScroll->show();
}

void ClassesPanel::HideAll()
{
	mWindow->hide();
	mScroll->hide();
}

void ClassesPanel::ShowPresentClasses()
{
	if ( mRepository == NULL )
	{
		mRepository = new PORepository;
		mRepository->AcquireSystem();
	}
	else
		mRepository->RefreshRepository();

	const std::list< std::string >& names = mRepository->GetClassNames();

	std::list< std::string >::const_iterator i = names.begin();
	mScroll->RemoveAllObjects();
	ObjectIcon* placeHolder;

	while ( i != names.end() )
	{
		placeHolder = new ObjectIcon;
		placeHolder->SetObjectName( (*i).c_str() );
		placeHolder->SetActionCallback( CBL::makeFunctor( (CBL::Functor1<std::string& >*)0, &DebuggerGUI::MakePOBrowserFor ) );
		placeHolder->SetSelectionCallback( CBL::makeFunctor( (CBL::Functor1<std::string& >*)0, *mScroll, &ObjectScroller::HandleLighting ) );
		mScroll->AddObject(placeHolder);
		i++;
	}
}
