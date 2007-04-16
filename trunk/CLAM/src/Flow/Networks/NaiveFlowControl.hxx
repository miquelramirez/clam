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


#ifndef  _NaiveFlowControl_hxx_
#define  _NaiveFlowControl_hxx_

#include "FlowControl.hxx"
#include <list>

namespace CLAM
{

class Processing;

class NaiveFlowControl : public FlowControl
{
public:
	NaiveFlowControl();
	virtual ~NaiveFlowControl(){}

	void ProcessingAddedToNetwork( Processing & added );
	void ProcessingRemovedFromNetwork( Processing & removed );

	void Do();

private:
	typedef std::list< Processing* > ProcessingList;
	ProcessingList mSources;
	ProcessingList mSinks;
	ProcessingList mGenerators;
	ProcessingList mNormalProcessings; // not Sources not Sinks


};

} // namespace CLAM

#endif //  _NaiveFlowControl_hxx_

