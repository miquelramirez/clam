/*
 * Copyright (c) 2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _OutPortRegistry_hxx_
#define _OutPortRegistry_hxx_

#include <string>
#include <vector>

namespace CLAM
{

//forward declarations:
class OutPortBase;

class OutPortRegistry
{
	std::vector<OutPortBase*> mOutPorts;
public:
	typedef std::vector<OutPortBase*>::iterator Iterator;
	typedef std::vector<OutPortBase*>::const_iterator ConstIterator;

	OutPortBase & GetByNumber(int index) const;
	OutPortBase & Get(const std::string & name) const;
	bool Has(const std::string& name) const;
	int Size() const;
	bool AreReadyForWriting();
	void Publish( OutPortBase * );
	void Clear(){ mOutPorts.clear(); }
	Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;
private:
	std::string AvailableNames() const;
};

} // namespace CLAM

#endif

