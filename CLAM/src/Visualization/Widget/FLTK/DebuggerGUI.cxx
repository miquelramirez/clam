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

#include "DebuggerGUI.hxx"
#include "GeometryKit.hxx"
#include "Debugger.hxx"
#include "GridLayout.hxx"
#include "ObjectScroller.hxx"
#include "CBL.hxx"
#include "Breakpoint.hxx"

#include <iostream>
using std::cout;
using std::endl;

using namespace CLAMGUI;

DebuggerGUI::DebuggerGUI()
	: mTask( Nothing ), mDbg( NULL )
{
}

DebuggerGUI::DebuggerGUI( Debugger* dbg )
	: mTask( Nothing ), mDbg( dbg )
{
}

DebuggerGUI::~DebuggerGUI()
{
	BrowserVec::iterator vec_it = mPOBrowsers.begin();

	while ( vec_it != mPOBrowsers.end() )
	{
		delete (*vec_it);
	}

	mPOBrowsers.clear();
}

void DebuggerGUI::MakeBreakPanel()
{

	unsigned int screen_w = Fl::w();
	unsigned int screen_h = Fl::h();

	Rect<int> screenRect( 0, 0, screen_w, screen_h );
 
	GridLayout layoutBreak( 46, 34, screenRect );

	Rect<int> panelcoords = layoutBreak << Rect<int>( 1, 1, 8, 6 );

	mBreakPanel.BuildWidgets( panelcoords );

}


void DebuggerGUI::MakeControlPanel()
{
	unsigned int screen_w = Fl::w();
	unsigned int screen_h = Fl::h();

	Rect<int> screenRect( 0, 0, screen_w, screen_h );

	GridLayout layout( 14, 10, screenRect );

	Rect<int> panelcoords = layout << Rect<int>( 1, 7, 4, 1 );

	mControlPanel.BuildWidgets( panelcoords );

	// Callback initialization

	mControlPanel.mNextBtn->SetReleaseCb( CBL::makeFunctor( (CBL::Functor0*)0, *this, &DebuggerGUI::OnNext ));
	mControlPanel.mContBtn->SetReleaseCb( CBL::makeFunctor( (CBL::Functor0*)0, *this, &DebuggerGUI::OnContinue ));
	mControlPanel.mExitBtn->SetReleaseCb( CBL::makeFunctor( (CBL::Functor0*)0, *this, &DebuggerGUI::OnExit ));
}

void DebuggerGUI::MakeClassesPanel()
{
	unsigned int screen_w = Fl::w();
	unsigned int screen_h = Fl::h();

	Rect<int> screenRect( 0, 0, screen_w, screen_h );
 
	GridLayout layoutBreak( 75, 50, screenRect );

	Rect<int> panelcoords = layoutBreak << Rect<int>( 20, 10, 12, 10);

	mClass.BuildWidgets( panelcoords );

}

void DebuggerGUI::OnNext()
{
	mTask = Next;
}

void DebuggerGUI::OnContinue()
{
	mTask = Continue;
}

void DebuggerGUI::OnExit()
{
	mTask = Exit;
}

void DebuggerGUI::RefreshBreakPanel()
{
	typedef std::vector< Breakpoint* >::iterator Iterator;

	std::vector< Breakpoint* > clients = mDbg->GetClients();
	Iterator                   client_it;
	ObjectIcon*                placeholder;

	mBreakPanel.mScroll->RemoveAllObjects();

	for ( client_it = clients.begin(); client_it != clients.end(); client_it++ )
	{
		placeholder = new ObjectIcon;
		placeholder->SetObjectName( ((*client_it)->GetName()).c_str() );

		if ( (*client_it)->IsBreaking() )
		{
			placeholder->Highlight();
			placeholder->SetActionCallback( CBL::makeFunctor( (CBL::Functor1<std::string& >*)0, *this, &DebuggerGUI::NullOp ) );
			placeholder->SetSelectionCallback( CBL::makeFunctor( (CBL::Functor1<std::string& >*)0, *this, &DebuggerGUI::NullOp ) );
		}
		mBreakPanel.mScroll->AddObject( placeholder );		 
	}
}

void DebuggerGUI::MakeViewFor( CLAM::Processing* p_obj )
{
	// :TODO: esta noche lo soñamos y mañana lo hacemos?

	cout << "Vista hecha :) " << endl;
}

void DebuggerGUI::MakePOBrowserFor( const std::string& str )
{
	Debugger* pDbg = Debugger::GetInstance();
		 
	DebuggerGUI* pDbgGUI = pDbg->GetGUI();


	BrowserVec::iterator vec_it = find_if( pDbgGUI->mPOBrowsers.begin(), pDbgGUI->mPOBrowsers.end(), DebuggerGUI::ExistsPanel( str ) );

	if ( vec_it == pDbgGUI->mPOBrowsers.end() ) // It's not already represented by a browser
	{

		POBrowser* placeholder = new POBrowser;

		placeholder->SetType( str );

		unsigned int screen_w = Fl::w();
		unsigned int screen_h = Fl::h();

		Rect<int> screenRect( 0, 0, screen_w, screen_h );

		GridLayout layoutBreak( 75, 50, screenRect );

		Rect<int> panelcoords = layoutBreak << Rect<int>( 50, 10, 10, 10);


		placeholder->BuildWidgets( panelcoords );

		pDbgGUI->mPOBrowsers.push_back( placeholder );
		placeholder->ShowAll();
	}
}

void DebuggerGUI::ShowAllBrowsers()
{
	for_each( mPOBrowsers.begin(), mPOBrowsers.end(), DebuggerGUI::ShowPanel() );
}

void DebuggerGUI::HideAllBrowsers()
{
	for_each( mPOBrowsers.begin(), mPOBrowsers.end(), DebuggerGUI::HidePanel() );
}

