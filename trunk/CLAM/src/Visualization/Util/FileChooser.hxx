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

#ifndef __FILECHOOSER__
#define __FILECHOOSER__

#include <list>
#include <string>

namespace CLAMVM
{
	class FileChooserDialog
	{
	public:
		FileChooserDialog();
		~FileChooserDialog();

		void SetTitle( const char* titleStr );
		void AddFileType( const char* wildcard );
		void ClearFileTypes();
		void SetDefaultFilename( const char* defaultFname );
		bool Show();
		const std::string& GetSelectedFilename() const;
	protected:
		std::string mDefaultFName;
		std::string mSelectedFilename;
		std::string mDialogTitle;
		std::list<std::string> mFilterFiletypes;
	};

	inline void FileChooserDialog::SetTitle( const char* title )
	{
		mDialogTitle = title;
	}

	inline const std::string& FileChooserDialog::GetSelectedFilename() const
	{
		return mSelectedFilename;
	}
}

#endif // FileChooser.hxx
