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

#ifndef __MODELCONTROLLER__
#define __MODELCONTROLLER__

#include "ModelAdapter.hxx"

namespace CLAMVM
{

/** \class ModelController
 *  \brief Base class for all ModelControllers
 *
 *  A ModelController is a ModelAdapter, that besides publishing
 *  a model object internal state, also allows to modify it.
 *
 *   @see ModelAdapter
 */
class ModelController
		: public ModelAdapter
{
public:
		/** \brief Destructor
		 * 
		 *  Class destructor
		 */
		virtual ~ModelController();

		/** \brief Returns object class name
		 *
		 *  \see ModelAdapter::GetClassName
		 */
		virtual const char* GetClassName() const = 0;

		/** \brief Publishes adapted model object internal state
		 *
		 *  @see ModelAdapter::Publish
		 */
		virtual bool Publish() = 0;

		/** \brief Modifies adapted model object internal state
		 *
		 *  This method tries to apply changes into adapted model object
		 *  internal state.
		 *
		 *  @returns A boolean indicating wether it has been possible 
		 *           to update the model object associated to this controller
		 */
		virtual bool Update() = 0;
};

}
#endif // ModelController.hxx

