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

#ifndef _DIRECTORYBASEDMETADATAREADER_HXX
#define _DIRECTORYBASEDMETADATAREADER_HXX

#include "SampleMetadata.hxx"
#include "MetadataReader.hxx"
#include <vector>
#include <string>
#include <exception>

namespace CLAM {

/**
* This exception is thrown if the file name given to the DirectoryBasedMetadataReader
* is invalid.
*/
class BadFilenameException: public std::exception
{
};

/**
* This class is used to load a collection of xml files from a directory that define
* a patch for a synthesizer. Given a directory path, this class reads in all of the
* XML files in that particular directory.and converts them into SampleMetadata
* objects. The XML files must be in the serialization format of the SampleMetadata
* class. See that class for more information.
*
* TODO define a DTD for the XML files
*
* \author greg kellum [gkellum@iua.upf.edu] 7/15/07
* \since CLAM v1.1
*/
class DirectoryBasedMetadataReader
{
public:
	DirectoryBasedMetadataReader();
	~DirectoryBasedMetadataReader();

	/**
	* This method reads in all of the XML files in the given directory path,
	* deserializes SampleMetadata objects from that, and copies them into the
	* given vector.
	*
	* @throws BadFilenameException if the given directory path is invalid
	* TODO add an exception for invalid XML formats
	*/
	void ReadMetadata(std::string& directoryPath, std::vector<CLAM::SampleMetadata>& argMetadataVector);
};

} // end namespace CLAM

#endif
