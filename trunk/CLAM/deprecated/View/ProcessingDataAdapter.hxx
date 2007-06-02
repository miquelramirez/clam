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

#ifndef __PROCESSINGDATAADPATER__
#define __PROCESSINGDATAADPATER__

#include "ModelAdapter.hxx"

namespace CLAM
{

class ProcessingData;

}

namespace CLAMVM
{

	using CLAM::ProcessingData;

/**
 *  This abstract class expresses the required interface
 *  for all Views that observe CLAM ProcessingData's.
 */

class ProcessingDataAdapter : public ModelAdapter
{
public:

		virtual ~ProcessingDataAdapter()
		{
		}

		/**
		 *  @see ModelAdapter::GetClassName
		 */	
		virtual const char* GetClassName() const  = 0;


		/**
		 *  @see ModelAdapter::Refresh
		 */
		virtual bool Publish() = 0;

		/**
		 *  This method stores a reference to the observed model object,
		 *  checking that the provided object is consistent ( from the
		 *  model adapter point of view, of course). Consistency is enforced depending
		 *  on the cost of the operations required to achieve such consistency.
		 *  @param Pointer to the processing data to be observed
		 *  @return A boolean telling us if it was possible to bind 
		 *          the view with the ProcessingData provided
		 */
		virtual bool BindTo( const ProcessingData& procDataObj ) = 0;

};

}

#endif // ProcessingDataAdapter.hxx

