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

#include <qwt/qwt_scale.h>
#include "YRuler.hxx"

namespace CLAM
{
	namespace VM
	{
		YRuler::YRuler(QWidget* parent) 
			: QwtScale(QwtScale::Left,parent,"y ruler")
		{
			Init();
		}

		YRuler::~YRuler()
		{
		}

		void YRuler::SetScaleDiv(QwtScaleDiv div)
		{
			setScaleDiv(div);
			emit scaleDiv(div);
		}

		void YRuler::Init()
		{
			QFont f("Sans",7);
			f.setBold(true);

			QwtScaleDiv div;

			setMinimumWidth(50);
			setFont(f);
			div.rebuild(-1.0,1.0,1,0,false,1.0);
			setScaleDiv(div);
		}

		void YRuler::SetLabelFormat(char f,int prec,int fieldWidth)
		{
			setLabelFormat(f,prec,fieldWidth);
		}
	}
}

// END

