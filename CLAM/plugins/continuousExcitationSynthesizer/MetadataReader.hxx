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

#ifndef _METADATAREADER_HXX
#define _METADATAREADER_HXX

#include "SampleMetadata.hxx"
#include <vector>

namespace CLAM {

/*
* It would be desirable to have one general interface to the metadata about samples
* regardless of whether this metadata be stored in XML files, an SQL database, or
* some other format, but the differences in the information needed to identify
* the data source as well as the means of querying the datasource makes this
* difficult. Providing a general interface between these sources, reduces the
* interface to the LCD of functionality which removes any motivation for even
* using different sorts of data sources.
*/
class MetadataReader
{
public:
	virtual void ReadMetadata(std::vector<SampleMetadata>& argMetadataVector)=0;
};

} // end namespace CLAM

#endif
