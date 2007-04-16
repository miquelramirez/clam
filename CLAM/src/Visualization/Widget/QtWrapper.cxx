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

#include "QtWrapper.hxx"
#include "QtAppSignaler.hxx"
#include <qapplication.h>

using namespace CLAMVM;




QtWrapper::~QtWrapper()
{
	delete mSignaler;
	mqApp->quit();
}

bool QtWrapper::IsClosing() const
{
	return mSignaler->is_finalizing;
}

void QtWrapper::Tick() const
{
	// Why sixty milliseconds? Because Windoze launches WM_PAINT 
	// approximately 30 times/sec, so this way it is more likely
	// to have consistently updated the screen...
	// X11 users: don't worry, it won't harm you by any means :)
	mqApp->processEvents(60);
}

void QtWrapper::Run() const
{
	mqApp->exec();
}



QtWrapper::QtWrapper()
	: mqApp(  NULL )
{
	int argc = 1;
	mqApp = new QApplication( argc, NULL );
	mSignaler = new QtAppSignaler( mqApp );

}

