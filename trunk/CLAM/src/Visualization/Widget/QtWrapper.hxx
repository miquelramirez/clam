/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __QTWRAPPER__
#define __QTWRAPPER__

#include "WidgetTKWrapper.hxx"
#include "Assert.hxx"

class QApplication;

namespace CLAMVM
{

class QtAppSignaler;

class QtWrapper
	: public WidgetTKWrapper
{

	friend class WidgetTKWrapper;

public:

	virtual ~QtWrapper();

	bool IsClosing() const;

	void Tick() const;

	void Run() const;

private:

	inline static WidgetTKWrapper* GetInstance()
	{
		return new QtWrapper;
	}

	QtWrapper();

	QApplication*       mqApp;
	QtAppSignaler*      mSignaler;
};

}


#endif // QtWrapper.hxx

