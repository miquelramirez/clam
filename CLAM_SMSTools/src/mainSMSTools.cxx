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
 * This program is distributed in thFl_Smart_Tile.e hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "SMSTools.hxx"
#include "Err.hxx"
#include <exception>
#include <iostream>
#include <cstdlib>

using CLAM::SMSFreqShift;
using CLAM::Err;

using CLAMGUI::SMSTools;

int main(void)
{
	try
	{
		SMSTools example;
		example.Run();
	}
	catch(Err& error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
		abort();
	}
	catch (std::bad_cast& e)
	{
		std::cout << e.what() << std::endl;
		abort();
	}
	catch (std::exception& e)
	{
		std::cout<<typeid(e).name();
		std::cout << e.what() << std::endl;
		std::cerr << "Abnormal Program Termination" << std::endl;
		abort();
	}
	catch ( ... )
	{
		std::cerr << "If you are under windows this can be a major memory corruption... DEBUG!" << std::endl;
		std::cerr << "Abnormal Program Termination" << std::endl;
		abort();
	}
	
	std::clog << "Finished successfully!"<<std::endl;
	return 0;
}
