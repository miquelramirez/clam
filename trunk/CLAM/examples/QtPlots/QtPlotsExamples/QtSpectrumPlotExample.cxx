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
#include "Spectrum.hxx"
#include "XMLStorage.hxx"
#include "QtAppWrapper.hxx"
#include "QtSpectrumPlot.hxx"
using CLAM::Spectrum;
using CLAM::XMLStorage;
using CLAM::VM::QtSpectrumPlot;
int main()
{
	FILE *fp = NULL;

	CLAM::VM::QtAppWrapper::Init();
	if((fp=fopen("../../data/spectrum_data.xml","r"))==NULL)
	{
		QMessageBox message("Required file not found",
		                "spectrum_data.xml not found!",
						QMessageBox::Critical,
						QMessageBox::Ok,
						QMessageBox::NoButton,
						QMessageBox::NoButton); 
		message.exec();
		CLAM::VM::QtAppWrapper::Quit();
		return 0;
	}
	else
	{
		fclose(fp);
	}
	Spectrum spec;
	XMLStorage::Restore(spec,"../../data/spectrum_data.xml");
	
	QtSpectrumPlot specPlot;
	specPlot.Label("Spectrum");
	specPlot.Geometry(100,100,500,225);
	specPlot.SetData(spec);
	specPlot.SetForegroundColor(CLAM::VM::VMColor::Blue());
	specPlot.Show();
	return CLAM::VM::QtAppWrapper::Run();
}
// END

