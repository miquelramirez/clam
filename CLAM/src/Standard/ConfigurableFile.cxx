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

#include "ConfigurableFile.hxx"
#include "XMLAdapter.hxx"

namespace CLAM {

ConfigurableFile::ConfigurableFile()
{
}
ConfigurableFile::ConfigurableFile(const ConfigurableFile &file)
	: mLocation(file.mLocation)
{
}
const ConfigurableFile& ConfigurableFile::operator=( const ConfigurableFile& rhs )
{
	mLocation = rhs.mLocation;
	return *this;
}
ConfigurableFile::~ConfigurableFile()
{
}

const std::string &ConfigurableFile::GetLocation() const {
	return mLocation;
}
void ConfigurableFile::SetLocation(const std::string& uri)
{
	mLocation = uri;
	LocationUpdated();
}

void ConfigurableFile::LoadFrom( Storage& storage )
{
	CLAM::XMLAdapter< Filename > xmlLocation( mLocation, "URI", true );
	storage.Load( xmlLocation );
	LocationUpdated();
}

void ConfigurableFile::StoreOn( Storage& storage ) const
{
	CLAM::XMLAdapter< Filename > xmlLocation( mLocation, "URI", true );
	storage.Store( xmlLocation );
}

void ConfigurableFile::LocationUpdated()
{
	// this is an empty default implementation
}

const FileFormatFilterList &ConfigurableFile::GetFormatFilterList() const
{
	static FileFormatFilterList filters;
	return filters;
}

}

