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

#include "ErrGUI.hxx"
#include <iostream>

namespace CLAMVM {

	ErrGUI::ErrGUI( char* mMainMsg, char* filename, int line) : 
		Err( "GUI Error:" ), 
		mGUIErrMsg(mMainMsg), 
		GLErrMsg(NULL),
		mFilename(filename), 
		mLineNum(line),
		mIsGL(false)
	{
	}

	ErrGUI::ErrGUI( unsigned char* mGlMsg, char* filename, int line) : 
		Err("OpenGL Error:"),
		mGUIErrMsg(NULL), 
		GLErrMsg(mGlMsg),
		mFilename(filename), 
		mLineNum(line), 
		mIsGL(true)
	{
	}

	void ErrGUI::Print(void)
	{
		Err::Print();
			
		if (mIsGL)
		{
			std::cout << " " << GLErrMsg << " at file " << mFilename << ", at line " << mLineNum << std::endl;
		}
		else
		{
			std::cout << " " << mGUIErrMsg << " at file " << mFilename << ", at line " << mLineNum << std::endl;
		}
	}

	
}

