/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

//////////////////////////////////////////////////////////////////////
// Class ControlLinker
// 

#ifndef _ControlLinker_
#define _ControlLinker_

#include <vector>
#include <string>
#include "Assert.hxx"

namespace CLAM {

// forward declarations.
class OutControl;
class InControl;


/**
 * This class is not used by now. 25 March 2003. As a result of a refactoring
 * Is expected that will realive (or some similar class) when we have to deal 
 * with networks that exports controls and ports.
 *
 *
 * This class is a base class of InPort and OutPort and also 
 * can be instantiated. Instances of ControlLinker are used as
 * control input/output of the groups of processing objects: it holds
 * a Linker among the outPorts (or other ControlLinker that acts as outPort)
 * of the outter world of the group and the inner InPorts (or ControlLinkers
 * acting as InPorts). And the same on the other direction: we use the
 * ControlLinkers to export a number of OutPorts (or CLs) to the outter world
 * of the group.
 * \see InPort 
 * \see OutPort

 * This class provides an interface to obtain all the "concrete" InPorts and
 * OutPort connected, along 
 * with functions for modifying the links between this ControlLinker and 
 * others. 
 * \warning This class is just temptative. It is likely to be
 *		soon depracated
 */
class ControlLinker
{
// Attributes:
private:
	std::vector<ControlLinker*> mInLinks;
	std::vector<ControlLinker*> mOutLinks;

// Iterator types
protected:
	typedef std::vector<OutControl*>::iterator OutControlIterator;
	typedef std::vector<InControl*>::iterator InControlIterator;

public:
	/**
	 * Creates a Link between an InControl (or ControlLinker acting as in 
	 * control) to this one. 
	 * <b>Important: 
	 * <li> If the ControlLinker passed is an OutControl object, no action is
	 * taken.
	 * <li> If this ControlLinker acts as an out control, the parameter
	 * should be an in control / Linker placed outside the group. If it acts as an
	 * in control, the parameter represent in control / Linker placed inside
	 * the group. </b>
	 * \warning This method belongs to a temptative class. And it is likely to be
	 *		soon depracated
	 */
	inline void AddInLink(ControlLinker* in);
	/**
	 * Creates a Link between an OutControl (or ControlLinker acting as out 
	 * control) to this one. 
	 * <b>Important: 
	 * <li> If the ControlLinker passed is an InControl object, no action is
	 * taken.
	 * <li> If this ControlLinker acts as an out control, the parameter
	 * should be an out control / Linker placed into the group. If it acts as an
	 * in control, the parameter represent out control / Linker placed outside
	 * the group. </b>
	 * \warning This method belongs to a temptative class. And it is likely to be
	 *		soon depracated
	 */
	inline void AddOutLink(ControlLinker* out);
	/**
	 * It cheks if the pointer passed is into the aggregate of input ControlLinkers
	 * and in that case it is removed
	 * \warning This method belongs to a temptative class. And it is likely to be
	 *		soon depracated
	 */
	inline void RemoveInLink(ControlLinker* in);
	/**
	 * It cheks if the pointer passed is into the aggregate of output ControlLinkers
	 * and in that case it is removed
	 * \warning This method belongs to a temptative class. And it is likely to be
	 *		soon depracated
	 */
	inline void RemoveOutLink(ControlLinker* out);
	

// Construction/Destruction:
	//ControlLinker(std::string name);
	ControlLinker() {};
	ControlLinker(const ControlLinker& arg) {
		CLAM_ASSERT(false, " A control class is being copied. This is not allowed because "
			"the controls are published to a processing object, which is NOT managing lost "
			"references nor copies");
	}

	~ControlLinker(){};

};



void ControlLinker::AddOutLink(ControlLinker* out) 
{
	
}

void ControlLinker::AddInLink(ControlLinker* out) 
{
}

void ControlLinker::RemoveOutLink(ControlLinker* out) 
{
}

void ControlLinker::RemoveInLink(ControlLinker* out) 
{
}



}; // namespace CLAM
#endif //_ControlLinker_
