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

#include "InControl.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"

namespace CLAM {

InControl::InControl(const std::string &name, Processing* parent, const bool publish)
	: mLastValue(0)
	, mName(name)
	, mParent(parent)
	, mUpperBound(1)
	, mLowerBound(0)
	, mBounded(false)
	, mHasDefaultValue(false)

{
	if (parent && publish) parent->RegisterInControl(this);
}


InControl::~InControl() 
{
	while (!mLinks.empty())
		mLinks.front()->RemoveLink(*this);
}

bool InControl::IsConnectedTo( OutControl & out)
{
	return out.IsConnectedTo( *this );
}

bool InControl::IsConnected() const
{
	return !mLinks.empty();
}

bool InControl::IsBounded() const
{
	return mBounded;
}
bool InControl::HasDefaultValue() const
{
	return mHasDefaultValue;
}
TControlData InControl::UpperBound() const
{
	return mUpperBound;
}
TControlData InControl::LowerBound() const
{
	return mLowerBound;
}
void InControl::SetBounds( TControlData lower, TControlData upper )
{
	mLowerBound = lower;
	mUpperBound = upper;
	mBounded = true;
}
void InControl::SetDefaultValue( TControlData val )
{
	mDefaultValue = val;
	mHasDefaultValue = true;
}
TControlData InControl::DefaultValue() const
{
	if (mHasDefaultValue) return mDefaultValue;
	if (!mBounded) return 0;
	return (mUpperBound+mLowerBound)/2.f;
}

void InControl::OutControlInterface_AddLink(OutControl & outControl)
{
	mLinks.push_back(&outControl);
}

void InControl::OutControlInterface_RemoveLink(OutControl & outControl)
{
	mLinks.remove(&outControl);
}


}; // namespace CLAM

