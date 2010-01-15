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

#ifndef _OutControl_
#define _OutControl_

#include <string>
#include <list>
#include <typeinfo>
#include "Assert.hxx"
#include "InControl.hxx"
#include "OutControlBase.hxx"

namespace CLAM {
	class Processing;
	
	/**
	* \brief Processing typed out control template class.
	*
	*/
	template<class ControlDataType>
	class OutControl : public OutControlBase
	{
		// This is required to solve the parsing problem with iterators.
		typedef InControl<ControlDataType> PeerInControl;
		typedef std::list< PeerInControl * > ProperInControlList;


	public:
		OutControl(const std::string &name = "unnamed typed in control", Processing * proc = 0);

		void SendControl(const ControlDataType& val);
		bool IsLinkable(const InControlBase& in);
		virtual const std::type_info & GetTypeId() const 
		{
			return typeid(ControlDataType);
		};
	};
	
	template<class ControlDataType>
	OutControl<ControlDataType>::OutControl(const std::string &name, Processing * proc)
		: OutControlBase(name,proc)
	{
	}

	template<class ControlDataType>
	void OutControl<ControlDataType>::SendControl(const ControlDataType& val)
	{
		typename std::list< InControlBase * >::iterator it;
		
		for (it=mLinks.begin(); it!=mLinks.end(); it++) 
		{
			InControl<ControlDataType>* control = static_cast<InControl<ControlDataType>*>(*it);
			control->DoControl(val);
		}
	}

	template<class ControlDataType>
	bool OutControl<ControlDataType>::IsLinkable(const InControlBase& in)
	{
		return typeid(ControlDataType) == in.GetTypeId();
		
	}

	typedef OutControl<float> FloatOutControl;

} // END NAMESPACE CLAM


#endif //_OutControl_

