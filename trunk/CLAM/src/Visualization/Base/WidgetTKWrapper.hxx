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


namespace CLAMGUI
{

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

	/** Singleton */
class WidgetTKWrapper
{

public:

	static WidgetTKWrapper& GetWrapperFor( std::string toolkit );

	virtual ~WidgetTKWrapper()
	{
	}

	virtual void DisableAsynchronousRefresh() = 0;

	virtual bool IsClosing() const = 0;

	virtual void Tick() const = 0;

	virtual void Run() const = 0;

protected:

	WidgetTKWrapper()
	{
	}

protected:


};

}

#endif // WidgetTKWrapper.hxx

