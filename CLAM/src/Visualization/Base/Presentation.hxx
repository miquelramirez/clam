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

#ifndef __PRESENTATION__
#define __PRESENTATION__

namespace CLAMVM
{

/** \class Presentation
 *  \brief Base class for concept representations
 *
 *  A Presentation presents some information, contained in the application
 *  model objects, to the user. This "information presentation" is accomplished,
 *  typically, through some sort of graphical metaphor.
 */
	class Presentation
	{
	public:
		
		/** \brief Destructor
		 *
		 *  Class destructor
		 */
		virtual ~Presentation();

		/** \brief 'Activates' presentation
		 *   
		 *  This methods makes the presentation to become visible to the user. If it
		 *  is a widget, then this method shows the widget on the screen.
		 */
		virtual void Show() = 0;

		/**  \brief 'Deactivates' presentation
		 *
		 *   This method makes the presentation to become hidden to the user. If we
		 *   are talking about widgets, this makes the widget to disappear from the screen.
		 */
		virtual void Hide() = 0;

	};

}



#endif // Presentation.hxx

