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

#ifndef _FlagControl_hxx_
#define _FlagControl_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <string>

namespace CLAM
{

class FlagControlConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FlagControlConfig, 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, bool, Flag );
protected:
	void DefaultInit(void);
};

class FlagControl : public Processing
{
private:
	
	FlagControlConfig mConfig;
	OutControl mFlagOutControl;

public:
	FlagControl();
	FlagControl( const FlagControlConfig & );

	bool Do();
	virtual ~FlagControl(){}
	const char * GetClassName() const {return "FlagControl";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif
