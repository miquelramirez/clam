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
