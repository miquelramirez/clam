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

#include "FileChooser.hxx"
#include <FL/fl_file_chooser.H>

namespace CLAMVM
{
	FileChooserDialog::FileChooserDialog()
		: mDefaultFName(""), mSelectedFilename(""), mDialogTitle( "Untitled file chooser dialog" )
	{
	}

	FileChooserDialog::~FileChooserDialog()
	{
	}

	void FileChooserDialog::AddFileType( const char* wildcard )
	{
		mFilterFiletypes.push_back( wildcard );
	}

	void FileChooserDialog::ClearFileTypes()
	{
		mFilterFiletypes.clear();
	}

	bool FileChooserDialog::Show()
	{
		std::string wildcardList;

		if ( mFilterFiletypes.empty() )
			wildcardList = "*.*";
		else
		{
			// It's not the most efficient to do it but...
			wildcardList += "{";
			std::list<std::string>::iterator i = mFilterFiletypes.begin();
			wildcardList += *i;
			i++;

			while( i != mFilterFiletypes.end() )
			{
				wildcardList += "|";
				wildcardList += *i;
				i++;
			}

			wildcardList+="}";		
		}

		const char* dfltFname = NULL;

		if ( mDefaultFName != "" )
			dfltFname = mDefaultFName.c_str();

		const char* choosedFilename = fl_file_chooser( mDialogTitle.c_str(),
							       wildcardList.c_str(),
							       dfltFname );

		Fl::flush();

		if ( choosedFilename == NULL )
			return false;
		
		mSelectedFilename = choosedFilename;
		
		return true;
	}
}
