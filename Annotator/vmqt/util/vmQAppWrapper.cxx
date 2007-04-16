/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <QtGui/QApplication>
#include "Assert.hxx"
#include "vmQAppWrapper.hxx"

namespace CLAM
{
	namespace VM
	{
		QAppWrapper::Wrapper QAppWrapper::mQApp = 0;

		void QAppWrapper::Init()
		{
			if(Initiated()) return;
			int argc = 1;
			mQApp = new QApplication(argc,0);
		}

		void QAppWrapper::Quit()
		{
			CLAM_ASSERT(Initiated(), 
						"QAppWrapper::Quit: wrapper has not been initiated yet, please call QAppWrapper::Init() first");
			mQApp->quit();
		}

		int QAppWrapper::Run()
		{
			CLAM_ASSERT(Initiated(), 
						"QAppWrapper::Run: wrapper has not been initiated yet, please call QAppWrapper::Init() first");
			mQApp->connect(mQApp,SIGNAL(lastWindowClosed()),mQApp,SLOT(quit()));
			return mQApp->exec();
		}

		bool QAppWrapper::Initiated()
		{
			return (mQApp != 0);
		}
	}
}

// END

