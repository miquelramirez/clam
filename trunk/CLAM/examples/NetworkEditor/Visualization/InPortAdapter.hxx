/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __INPORTADAPTER_HXX__
#define __INPORTADAPTER_HXX__

#include "ModelAdapter.hxx"
#include "PortModel.hxx"

namespace CLAM
{
	class InPort;
}
namespace CLAMVM
{

class InPortAdapter : public ModelAdapter, public PortModel
{
private:
	const CLAM::InPort* mObserved;
public:
	InPortAdapter();
	virtual ~InPortAdapter();
	virtual const char* GetClassName() const
	{
		return "InPortAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::InPort& obj );	
};

} // namespace CLAMVM

#endif // __INPORTMODEL_HXX__
