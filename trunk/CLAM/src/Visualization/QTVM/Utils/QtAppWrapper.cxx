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

#include <qapplication.h>
#include "QtAppWrapper.hxx"

namespace CLAM
{
	namespace VM
	{
		QtAppWrapper::Wrapper QtAppWrapper::_app;

		void QtAppWrapper::Init()
		{
			if(!IsInitiated())
			{
				int argc=1;
				_app = new QApplication(argc,NULL);
			}
		}

		void QtAppWrapper::Quit()
		{
			_app->quit();
		}

		int QtAppWrapper::Run()
		{
			_app->connect(_app,SIGNAL(lastWindowClosed()),_app,SLOT(quit()));
			return _app->exec();
		}

		bool QtAppWrapper::IsInitiated()
		{
			return _app;
		}

	}
}

// END

