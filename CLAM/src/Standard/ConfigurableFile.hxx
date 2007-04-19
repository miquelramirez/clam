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

#ifndef CLAM_FILE_HXX
#define CLAM_FILE_HXX

#include "Component.hxx"
#include "Filename.hxx"
#include <list>
#include <string>
#include <utility>

namespace CLAM
{

typedef std::pair<std::string, std::string> FileFormatFilter;
typedef std::list<FileFormatFilter> FileFormatFilterList;

/**
 * The ConfigurableFile class provides an abstract interface for CLAM 
 * file management, providing methods that permit a generic user interface 
 * to work with different classes of files.  In contrast, the Filename 
 * class may be preferred to handle names of internal files, which the user 
 * does not directly see or configure.
 *
 * See the AudioFile, AudioFileTarget, and AudioFileSource classes for 
 * examples that show how to handle a new class of files.
 */
class ConfigurableFile : public Component
{
public:
	ConfigurableFile();
	ConfigurableFile( const ConfigurableFile& );
	const ConfigurableFile& operator=( const ConfigurableFile& );
	virtual ~ConfigurableFile();

	void SetLocation(const std::string& uri);

	virtual const char* GetClassName() const = 0;

	/**
	 * The group name defines the class of files (e.g. "audio", "video",
	 * etc.).  The returned string may be used to prompt the user.
	 */
	virtual const char *GetGroupName() const = 0;

	/**
	 * This method returns the file direction for the given instance. 
	 * Individual instances are restricted to be either in input or output
	 * mode at any one time; two subclasses (or instances) must be created 
	 * to support simultaneous input and output.
	 */
	virtual bool GetWriteMode() = 0;

	/**
	 * The GetFormatFilterList() interface allows subclasses to return a 
	 * FileFormatFilterList that can be used to restrict a user interface
	 * to display a subset of file name extensions (e.g. *.txt or *.wav).
	 *
	 * While this method is pure virtual, a default implementation is 
	 * available * to call when an empty filter list is required.
	 * This signfies that no reasonable filtering can be performed, and
	 * all files should be presented to the user.
	 */
	virtual const FileFormatFilterList &GetFormatFilterList() const = 0;

protected:
	/// subclasses may override this method
	virtual void LocationUpdated();
	Filename mLocation;
};

}

#endif // CLAM_FILE_HXX

