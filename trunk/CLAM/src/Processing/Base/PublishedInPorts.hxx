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

#ifndef _PublishedInPorts_hxx_
#define _PublishedInPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{
	
//forward declarations:
class InPortBase;

class PublishedInPorts
{
	std::vector<InPortBase*> mInPorts;
public:
	typedef std::vector<InPortBase*>::iterator Iterator;
	typedef std::vector<InPortBase*>::const_iterator ConstIterator;

	InPortBase& GetByNumber(int index) const;
	InPortBase& Get( const std::string & name ) const;
	int Size() const;
	bool AreReadyForReading();
	void Publish(InPortBase *);
	void Clear(){ mInPorts.clear(); }
	Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;

private:
	std::string AvailableNames() const;
};

} // namespace CLAM

#endif
