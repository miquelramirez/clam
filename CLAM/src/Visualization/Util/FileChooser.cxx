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
