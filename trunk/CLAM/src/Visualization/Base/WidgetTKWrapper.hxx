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

#ifndef __WIDGETTKWRAPPER__
#define __WIDGETTKWRAPPER__

#include "Component.hxx"
#include "Enum.hxx"
#include "ErrGUI.hxx"

#include <string>

using std::string;

using CLAM::Component;
using CLAM::Enum;


namespace CLAMVM
{

/** \class EWidgetTK
 *  \brief Possible productions of the WidgetTKWrapper
 *
 *  This CLAM::Enum identifies the toolkits that CLAM is
 *  able to wrap. 
 */
class EWidgetTK
	: public Enum
{
public:
	static tEnumValue sEnumValues[];
	static tValue     sDefault;

	EWidgetTK()
		: Enum( sEnumValues, sDefault )
	{
	}

	EWidgetTK( tValue v )
		: Enum( sEnumValues, v )
	{
	}

	EWidgetTK( string s )
		: Enum( sEnumValues, s )
	{
	}

	~EWidgetTK()
	{
	}

	Component* Species() const
	{
		return new EWidgetTK;
	}

	typedef enum 
	{ 
		FLTK = 0,
		Qt
	} tEnum;
};

		/**  \class WidgetTKWrapper
		 *   \brief GUI Toolkit low-level functionality wrapper
		 *
		 *   This class is a Creator/Singleton object whose products
		 *   are abstract wrappers for accessing a GUI Toolkit low-level
		 *   functionality such as triggering the event loop, triggering the
		 *   execution of a single iteration of the event loop or setting the
		 *   refresh rate for graphic displays.
		 *
		 *   <em>NOTE:</em> This class will be refactored soon, so keep in mind
		 *   slight interface changes in the near future.
		 */
class WidgetTKWrapper
{

public:

		/** \brief Virtual constructor
		 *
		 *  This is the typical GetInstance-like method that ensures the
		 *  class to be a Singleton.
		 *  
		 *  \params   The string that identifies the toolkit for which a Wrapper is requested
		 *  \returns  A reference to a toolkit wrapper
		 */
		static WidgetTKWrapper& GetWrapperFor( std::string toolkit );
		
		/** \brief Destructor
		 *
		 *  Class destructor
		 */
		virtual ~WidgetTKWrapper();
		
		/** \brief Disable DrawingSurfaces asynchronous refresh
		 *
		 *  This method disables the automatic ( timer-driven ) refresh
		 *  of the system DrawingSurfaces. Asyncrhonous Refresh is activated by default.
		 */
		virtual void DisableAsynchronousRefresh() = 0;
		
		/** \brief Tells if there are any widgets active
		 *
		 *  Sometimes, when the application loop is not implicit in the Toolkit 
		 *  event loop, it is important to know when all the widgets that compose
		 *  the application GUI have been hidden/destroyed.
		 *
		 *  \return True when there are no active widgets. False when there are still 
		 *          active widgets.
		 */
		virtual bool IsClosing() const = 0;
		
		/** \brief Forces a GUI Toolkit event loop iteration to execute
		 *
		 *  This method forces the underlying GUI Toolkit event loop just to execute
		 *  one step. Typically, GUI Toolkits event loops consist basically of three
		 *  operations:
		 *  <ul>
		 *     <li>Route user-events ( mouse moving, window hiding, etc. ) to the interested widgets</li>
		 *     <li>Assure every widget has dispatched the received events</li>
		 *     <li>Redraw ( as necessary ) currently visible widgets</li>
		 *  </ul>
		 *
		 *  Using Tick() one can have an application main loop which keeps widgets up-to-date while
		 *  keeps "background" processes working.
		 *
		 *  This function returns control to caller as soon as these three operations are completed.
		 */
		virtual void Tick() const = 0;
		
		/** \brief Cedes control to GUI Toolkit event loop
		 *
		 *  This method launches the GUI toolkit event loop not returning control to the caller 
		 *  until the GUI Toolkit sees fit to do so. This usually happens when there are no widgets 
		 *  left on the screen.
		 */
		virtual void Run() const = 0;
		
protected:
		
		/** \brief Constructor
		 *
		 *  Note that the constructor is protected because this class is a singleton.
		 */
		WidgetTKWrapper();
		
};

}

#endif // WidgetTKWrapper.hxx

