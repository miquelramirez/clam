/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Rappid.hxx"
#include "RappidControlTester.hxx"

#include <iostream>
#include <pthread.h>

int main(int argc, char* argv[])
{
	try {

		RappidConfig cfg;

		cfg.parse_command_line(argc,argv);

		Rappid app(cfg);

		app.StartRunning();

		if (cfg.GetDoControlTest())
		{
			RappidControlTester test(app,cfg.GetControlTestSleep());
			test.Start();
		}

		app.WaitWhileProcessing();

	}
	catch(ErrProcessingObj& er) {
		er.Print();
		return 1;
	}
	catch(Err er) {
		er.Print();
		return 1;
	}
	catch(...) {
		std::cout << "Unhandled exception!!!!" << std::endl;
		return 1;
	}
	return 0;
}

