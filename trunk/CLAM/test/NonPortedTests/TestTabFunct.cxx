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


#include "TabFunct.hxx"
#include "Assert.hxx"
#include <iostream>
#include <cmath>

using std::cout;

using namespace CLAM;

namespace CLAMTest{

class Log
{
public:
	TData operator() (const TData arg) { return log(arg); }
};

class Double
{
public:
	TData operator() (const TData arg) { return arg*2; }
};

}; //namespace


int TestTabFunction()
{
	TabFunct<CLAMTest::Log> f(100, TData(2.01), 100);
	TabFunct<CLAMTest::Double> g(3, 0, 100);
	
	TData maxerror=0;
	for (TData x=TData(2.01); x<=100; x+=5)	{
		TData newerror = std::abs(f.Log::operator() (x) - f(x));
		maxerror = maxerror < newerror?  newerror : maxerror;
		cout << f(x) << " (" << f.Log::operator() (x) <<") ";
	} 
	cout << "\nLog Max Error = "<<maxerror<<" \n\n";
	if (maxerror > 0.001) {
		cout << "Error too high\n";
		return -1;
	}
	
	maxerror=0;
	for (TData x=TData(0.01); x<=100; x+=5)	{
		TData newerror = std::abs(g.Double::operator() (x) - g(x));
		maxerror = maxerror < newerror?  newerror : maxerror;
		cout << g(x) << " (" << g.Double::operator() (x) <<") ";
	}
	cout << "\nDouble Max Error = "<<maxerror<<" \n\n";
	if (maxerror > 0.001) {
		cout << "Error too high\n";
		return -1;
	}
	
	
	return 0;
}


int main(void)
{

	return TestTabFunction();

}
 
