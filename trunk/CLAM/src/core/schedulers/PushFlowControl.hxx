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


#ifndef  _PushFlowControl_hxx_
#define  _PushFlowControl_hxx_

#include "FlowControl.hxx"
#include <list>

namespace CLAM
{

class Processing;

/// @deprecated This flow control do not fulfils the Network::Do() call-back based semantics 
///  to be more specific: consume all audio from its sources and produce to its sinks if possible
class PushFlowControl : public FlowControl
{
public:
	PushFlowControl();
	virtual ~PushFlowControl(){}

	void ProcessingAddedToNetwork( Processing & added );
	void ProcessingRemovedFromNetwork( Processing & removed );

	void Do();

private:
	void AddNewPossibleProcessingsToDo( Processing * father, 
					    std::list<Processing*> &);
	std::list< Processing* > mGenerators;


};

} // namespace CLAM

#endif //  _PushFlowControl_hxx_

