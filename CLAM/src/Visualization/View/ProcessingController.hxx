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

#ifndef __PROCESSINGCONTROLLER__
#define __PROCESSINGCONTROLLER__

#include <map>
#include "ModelController.hxx"
#include "ControlAdapter.hxx"
#include "ProcessingInControlSet.hxx"

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	using CLAM::Processing;

	class ProcessingController
		: public ModelController, public ProcessingInControlSet
	{

	protected:
		void DeleteSubAdapters();

	public:
			
		ProcessingController();

		virtual ~ProcessingController();

		virtual const char* GetClassName() const
		{
			return "ProcessingController";
		}

		virtual bool Publish();

		virtual bool Update();

		virtual bool BindTo( Processing& procObj );
				
		virtual bool Unbind();


	};
}

#endif // ProcessingController.hxx

