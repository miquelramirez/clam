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

#include "ProcessingController.hxx"
#include "Processing.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
	using CLAM::Processing;

	ProcessingController::ProcessingController()
	{
	}

	ProcessingController::~ProcessingController()
	{
		DeleteSubAdapters();
	}

	bool ProcessingController::Publish()
	{
		iterator i = mChildren.begin();

		while ( i != mChildren.end() )
		{
			ControlAdapter* adap = static_cast<ControlAdapter* >( i->second );
			CLAM_ASSERT( adap!=NULL, "The static cast failed: invariant does not hold!" );
			adap->Publish();

			i++;
		}

		return true;
	}

	bool ProcessingController::Update()
	{
		iterator i = mChildren.begin();

		while ( i != mChildren.end() )
		{
			ControlAdapter* adap = static_cast<ControlAdapter* >( i->second );
			CLAM_ASSERT( adap!=NULL, "The static cast failed: invariant does not hold!" );
			adap->Update();

			i++;
		}
				
		return true;
	}


	bool ProcessingController::BindTo( Processing& procObj )
	{

		CLAM::InControlRegistry::Iterator i = procObj.GetInControls().Begin();

		while ( i!=procObj.GetInControls().End() )
		{
			ControlAdapter* adap = new ControlAdapter();
						
			Insert( (*i)->GetName(), *adap   );
			adap->BindTo( *(*i) );
						
			i++;
		}

		return true;
	}

	bool ProcessingController::Unbind()
	{
		DeleteSubAdapters();
		mChildren.clear();
				
		return true;
	}

	void ProcessingController::DeleteSubAdapters()
	{
		iterator i = mChildren.begin();

		while ( i != mChildren.end() )
		{
			delete i->second;
				
			i++;
		}				

	}
}

