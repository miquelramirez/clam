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

#ifndef __QTFUNDPLAYER__
#define __QTFUNDPLAYER__

#include "Segment.hxx"
#include "QtPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtFundPlayer : public QtPlayer
		{
			public:
				QtFundPlayer(QWidget* parent=0);
				virtual ~QtFundPlayer();

				void SetData(const Segment& segment);
		};
	}
}

#endif
