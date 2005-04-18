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

#include <qmessagebox.h>
#include "Audio.hxx"
#include "AudioFileLoader.hxx"
#include "QtAudioPlot.hxx"
#include "QtAppWrapper.hxx"

using CLAM::VM::QtAudioPlot;

int main()
{
	AudioFileLoader fLoader;
	Audio audio;

	CLAM::VM::QtAppWrapper::Init();

	int err = fLoader.Load("../../data/birthday.wav",audio);
	if(err)
	{
		QMessageBox message("File Error",
							"Unable to open \'birthday.wav\'",
							QMessageBox::Critical,
							QMessageBox::Ok,
							QMessageBox::NoButton,
							QMessageBox::NoButton); 
		message.exec();
		CLAM::VM::QtAppWrapper::Quit();
		return 0;
	}	

	QtAudioPlot aPlot;
	aPlot.Label("Audio");
	aPlot.Geometry(100,100,500,220);
	aPlot.SetData(audio);
	aPlot.SetForegroundColor(CLAM::VM::VMColor::Blue());
	aPlot.Show();
       
	return CLAM::VM::QtAppWrapper::Run();
	
}


// END
