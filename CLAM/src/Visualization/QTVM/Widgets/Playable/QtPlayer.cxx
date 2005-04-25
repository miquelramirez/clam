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
#include "IconData.hxx"
#include "QtPlayer.hxx"

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
	   
	    _play = new QPushButton(container);
	    _play->setFixedSize(25,20);
	    _play->setGeometry(0,5,25,20);
	    QToolTip::add(_play,"Play");

	    _pause = new QPushButton(container);
	    _pause->setFixedSize(25,20);
	    _pause->setGeometry(_play->width(),_play->y(),25,20);
	    QToolTip::add(_pause,"Pause");

	    _stop = new QPushButton(container);
	    _stop->setFixedSize(25,20);
	    _stop->setGeometry(_pause->x()+_pause->width(),_play->y(),25,20);
	    QToolTip::add(_stop,"Stop");

	    layout->addWidget(container);

	    // Connections
	    connect(_play,SIGNAL(clicked()),this,SLOT(play()));
	    connect(_pause,SIGNAL(clicked()),this,SLOT(pause()));
	    connect(_stop,SIGNAL(clicked()),this,SLOT(stop()));
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
	    _play->setPaletteBackgroundColor(winBackground);
	    _pause->setPaletteBackgroundColor(winBackground);
	    _stop->setPaletteBackgroundColor(winBackground);

	    QPixmap icon_play((const char**)icon_play);
	    QPixmap icon_pause((const char**)icon_pause);
	    QPixmap icon_stop((const char**)icon_stop);

	    _play->setPixmap(icon_play);
	    _pause->setPixmap(icon_pause);
	    _stop->setPixmap(icon_stop);
	}

	void QtPlayer::BlackBackgroundCM()
	{
	    QColor bkcolor(0,0,0);
	    this->setPaletteBackgroundColor(bkcolor);
	    container->setPaletteBackgroundColor(bkcolor);
	    _play->setPaletteBackgroundColor(bkcolor);
	    _pause->setPaletteBackgroundColor(bkcolor);
	    _stop->setPaletteBackgroundColor(bkcolor);

	    QPixmap icon_play((const char**)icon_play_cyan);
	    QPixmap icon_pause((const char**)icon_pause_cyan);
	    QPixmap icon_stop((const char**)icon_stop_cyan);

	    _play->setPixmap(icon_play);
	    _pause->setPixmap(icon_pause);
	    _stop->setPixmap(icon_stop);
	}

	void QtPlayer::WhiteBackgroundCM()
	{
	    QColor bkcolor(255,255,255);
	    this->setPaletteBackgroundColor(bkcolor);
	    container->setPaletteBackgroundColor(bkcolor);
	    _play->setPaletteBackgroundColor(bkcolor);
	    _pause->setPaletteBackgroundColor(bkcolor);
	    _stop->setPaletteBackgroundColor(bkcolor);

	    QPixmap icon_play((const char**)icon_play);
	    QPixmap icon_pause((const char**)icon_pause);
	    QPixmap icon_stop((const char**)icon_stop);

	    _play->setPixmap(icon_play);
	    _pause->setPixmap(icon_pause);
	    _stop->setPixmap(icon_stop); 
	}
		
    }
}

// END

