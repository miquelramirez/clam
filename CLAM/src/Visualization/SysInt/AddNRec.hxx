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

#ifndef __ADDNREC__
#define __ADDNREC__

#include <algorithm>
#include "PORepository.hxx"

namespace CLAMGUI
{

	class Add_and_Recurse
	{
	public:
		explicit Add_and_Recurse( PORepository& ref )
			: mRepository( ref )
		{
		}

		void operator()( CLAM::Processing* obj )
		{
			CLAM::ProcessingComposite* candidate = dynamic_cast< CLAM::ProcessingComposite* >( obj );

			if ( candidate == NULL )
			{
				// Not a composite itself so we don't have to recurse any more
				mRepository.AddPO( obj );
			}
			else
			{
				mRepository.AddPO( obj );
				std::for_each( candidate->composite_begin(), candidate->composite_end(), Add_and_Recurse( mRepository ) );
			}
		}

	private:
		PORepository& mRepository;
	};

}

#endif // AddNRec.hxx
