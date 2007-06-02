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

#ifndef __PROCESSINGINCONTROLSET__
#define __PROCESSINGINCONTROLSET__

#include "InputControlModel.hxx"
#include <map>
#include <string>

namespace CLAMVM
{
	class ProcessingInControlSet
	{
	private:
		typedef std::map< std::string, InputControlModel* >   tChildTbl;
		typedef tChildTbl::value_type                         tChildvalue_type;
	protected:		

		tChildTbl                                             mChildren;
				
	public:
		typedef tChildTbl::iterator                           iterator;
		typedef tChildTbl::const_iterator                     const_iterator;

		ProcessingInControlSet();

		virtual ~ProcessingInControlSet();
				
		void Insert( std::string name, InputControlModel& a );
				
		InputControlModel& Retrieve( std::string name );

		iterator begin()
		{
			return mChildren.begin();
		}

		const_iterator begin() const
		{
			return mChildren.begin();
		}

		iterator end()
		{
			return mChildren.end();
		}

		const_iterator end() const
		{
			return mChildren.end();
		}

		Signalv1< std::string >         NamePublished;
	};

}

#endif // ProcessingInControlSet.hxx

