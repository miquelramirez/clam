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

#ifndef __OUTCONTROLADAPTER_HXX__
#define __OUTCONTROLADAPTER_HXX__

#include "ModelAdapter.hxx"
#include "ControlModel.hxx"

namespace CLAM
{
	class OutControl;
}
namespace CLAMVM
{

class OutControlAdapter : public ModelAdapter, public ControlModel
{
private:
	const CLAM::OutControl* mObserved;
public:
	OutControlAdapter();
	virtual ~OutControlAdapter();
	virtual const char* GetClassName() const
	{
		return "OutControlAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::OutControl& obj );	
};

} // namespace CLAMVM

#endif // __INCONTROLMODEL_HXX__
