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

#ifndef __DEBUGGERGUI__
#define __DEBUGGERGUI__

#include <FL/Fl.H>
#include "DebuggerCP.hxx"
#include "BreakPanel.hxx"
#include "ClassesPanel.hxx"
#include "POBrowser.hxx"
#include <string>
#include <vector>
#ifndef WIN32
#include <functional>
#endif
#include <algorithm>
using std::find_if;
using std::for_each;

namespace CLAM
{

class Processing;

}

namespace CLAMGUI
{

class PORepository;

class Debugger;

class DebuggerGUI
{
private:

	struct ShowPanel
	{
		void operator()( Panel* p )
		{
			p->ShowAll();
		} 
	};

	struct HidePanel
	{
		void operator()( Panel* p )
		{
			p->HideAll();
		}
	};

	struct ExistsPanel
		: public std::unary_function< POBrowser*, bool >
	{

		const std::string& cmp_str;

		ExistsPanel( const std::string& str )
			: cmp_str( str )
		{
		}

		bool operator() ( POBrowser* p )
		{
			return p->GetType() == cmp_str;
		}
	};

public:
	typedef enum
		{ Next=0, Continue, Exit, Nothing } UserAction;

	typedef std::vector< POBrowser* > BrowserVec;

	DebuggerGUI();

	DebuggerGUI( Debugger* dbg );

	~DebuggerGUI();

	void MakeControlPanel();

	void MakeClassesPanel();

	void MakeBreakPanel();

	UserAction Run()
	{
		RefreshBreakPanel();
		mTask = Nothing;
		mControlPanel.ShowAll();
		mBreakPanel.ShowAll();
		mClass.ShowAll();
		ShowAllBrowsers();

		while ( mTask == Nothing )
		{
			Fl::wait();
		}

		mControlPanel.HideAll();
		mBreakPanel.HideAll();
		mClass.HideAll();
		HideAllBrowsers();

		return mTask;
	}

	void RefreshBreakPanel();

	void ShowAllBrowsers();

	void HideAllBrowsers();

	static void MakeViewFor( CLAM::Processing* p_obj );

	static void MakePOBrowserFor( const std::string& str );

protected:

	void OnNext();

	void OnContinue();

	void OnExit();

	void NullOp( const std::string& s)
	{
	}

private:
	UserAction          mTask;
	DebuggerCP          mControlPanel;
	BreakPanel          mBreakPanel;
	ClassesPanel        mClass;
	Debugger*           mDbg;
	BrowserVec          mPOBrowsers;
};

}

#endif // DebuggerGUI.hxx
