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

/** @file UserTutorial3.cxx
 * Tutorial example ilustrating how to set and get the dynamic
 * attributes in a data object.
 */
#include <Audio.hxx>
#include <Array.hxx>
#include <iostream>
#include "CLAM_Math.hxx"

int main()
{
	// We specify this to avoid typing the CLAM:: prefix
	using namespace CLAM;

	try {
		// We first create and configure the data object.
		Audio au;
		au.SetSize(1024);
		au.SetSampleRate(8000);

		// Now we create a real array
		// We reserve a 2048 samples buffer, just in case
		Array<TData> array(2048);

		// But for now we set the logical size of the array
		// to just 1024 samples
		array.SetSize(1024);

		// We initialize it...
		int i;
		for (i=0; i<1024; i++)
			array[i]=sin(2*3.141592*10.5*i/(1024));

		// And we use it to set the real attribute 
		// in our Audio data object, by making a copy.
		au.SetBuffer(array);

		// Now, if we want, we can modify the data in the Audio object
		// (This does not change the original "array" object):
		for (i=0; i<1024; i++)
			au.GetBuffer()[i]*=1.5;

		// And, of course, we can copy it into a different array.
		Array<TData> array2 = au.GetBuffer();

	} 
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}

