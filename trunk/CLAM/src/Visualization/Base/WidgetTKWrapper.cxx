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

#include "WidgetTKWrapper.hxx"
#include "ErrGUI.hxx"
#include "SupportedToolkits.hxx"

namespace CLAMVM
{

Enum::tEnumValue EWidgetTK::sEnumValues[] =
{
	{ EWidgetTK::FLTK, "FLTK" },
	{ EWidgetTK::Qt,   "Qt" },
	{ 0, NULL }
};

Enum::tValue EWidgetTK::sDefault = EWidgetTK::FLTK;

		WidgetTKWrapper::WidgetTKWrapper()
		{
		}

		WidgetTKWrapper::~WidgetTKWrapper()
		{
		}

WidgetTKWrapper& WidgetTKWrapper::GetWrapperFor( std::string toolkit )
{
	EWidgetTK requestedTK( toolkit );

	if ( requestedTK.GetValue() == EWidgetTK::FLTK )
	{
		return FLTKWrapper::GetInstance();
	}
	else
	{
		CLAM_ASSERT(false, "None of supported toolkits was chosen. Check documentation\n"
		      "about CLAM supported toolkits\n" );
		return *((WidgetTKWrapper*)NULL);
	}

}

}
