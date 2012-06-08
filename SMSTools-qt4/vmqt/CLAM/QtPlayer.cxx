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
#include <qlayout.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <CLAM/IconData.hxx>
#include <CLAM/QtPlayer.hxx>

namespace CLAM
{
    namespace VM
    {
		QtPlayer::QtPlayer(QWidget* parent) 
			: QWidget(parent)
		{	       
			InitWidgets();
			SetColorMap(CLAM::VM::WindowMode);
		}
		
		QtPlayer::~QtPlayer()
		{
		}

		void QtPlayer::InitWidgets()
		{
			winBackground = this->paletteBackgroundColor();

			layout = new QHBoxLayout(this);
	    
			container = new QFrame(this);
			container->setFixedSize(75,30);

			mPlay = new QPushButton(container);
			mPlay->setFixedSize(25,20);
			mPlay->setGeometry(0,5,25,20);
			QToolTip::add(mPlay,"Play");

			mPause = new QPushButton(container);
			mPause->setFixedSize(25,20);
			mPause->setGeometry(mPlay->width(),mPlay->y(),25,20);
			QToolTip::add(mPause,"Pause");

			mStop = new QPushButton(container);
			mStop->setFixedSize(25,20);
			mStop->setGeometry(mPause->x()+mPause->width(),mPlay->y(),25,20);
			QToolTip::add(mStop,"Stop");

			layout->addWidget(container);

			// Connections
			connect(mPlay,SIGNAL(clicked()),this,SLOT(play()));
			connect(mPause,SIGNAL(clicked()),this,SLOT(pause()));
			connect(mStop,SIGNAL(clicked()),this,SLOT(stop()));
		}

		void QtPlayer::SetColorMap(ColorMap map)
		{
			switch(map)
			{
				case CLAM::VM::WindowMode:
					WindowModeCM();
					break;
				case CLAM::VM::BlackBackground:
					BlackBackgroundCM();
					break;
				case CLAM::VM::WhiteBackground:
					WhiteBackgroundCM();
					break;
				default:
					break;
			}
		}

		void QtPlayer::WindowModeCM()
		{
			this->setPaletteBackgroundColor(winBackground);
			container->setPaletteBackgroundColor(winBackground);
			mPlay->setPaletteBackgroundColor(winBackground);
			mPause->setPaletteBackgroundColor(winBackground);
			mStop->setPaletteBackgroundColor(winBackground);

			QPixmap iconPlay((const char**)icon_play);
			QPixmap iconPause((const char**)icon_pause);
			QPixmap iconStop((const char**)icon_stop);

			mPlay->setPixmap(iconPlay);
			mPause->setPixmap(iconPause);
			mStop->setPixmap(iconStop);
		}

		void QtPlayer::BlackBackgroundCM()
		{
			QColor bkcolor(0,0,0);
			this->setPaletteBackgroundColor(bkcolor);
			container->setPaletteBackgroundColor(bkcolor);
			mPlay->setPaletteBackgroundColor(bkcolor);
			mPause->setPaletteBackgroundColor(bkcolor);
			mStop->setPaletteBackgroundColor(bkcolor);

			QPixmap iconPlay((const char**)icon_play_cyan);
			QPixmap iconPause((const char**)icon_pause_cyan);
			QPixmap iconStop((const char**)icon_stop_cyan);

			mPlay->setPixmap(iconPlay);
			mPause->setPixmap(iconPause);
			mStop->setPixmap(iconStop);
		}

		void QtPlayer::WhiteBackgroundCM()
		{
			QColor bkcolor(255,255,255);
			this->setPaletteBackgroundColor(bkcolor);
			container->setPaletteBackgroundColor(bkcolor);
			mPlay->setPaletteBackgroundColor(bkcolor);
			mPause->setPaletteBackgroundColor(bkcolor);
			mStop->setPaletteBackgroundColor(bkcolor);

			QPixmap iconPlay((const char**)icon_play);
			QPixmap iconPause((const char**)icon_pause);
			QPixmap iconStop((const char**)icon_stop);

			mPlay->setPixmap(iconPlay);
			mPause->setPixmap(iconPause);
			mStop->setPixmap(iconStop); 
		}

		void QtPlayer::RemoveButton(int id)
		{
			switch(id)
			{
				case PAUSE:
					mPause->hide();
					container->setFixedSize(50,30);
					mStop->setGeometry(mPlay->width(),mPlay->y(),25,20);
					break;
				case STOP:
					mStop->hide();
					break;
				default:
					break;
			}
		}
		
    }
}

// END

