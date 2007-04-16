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

#ifndef __PROCESSINGKERNEL__
#define __PROCESSINGKERNEL__

namespace CLAM
{

class Thread;

class ProcessingKernel
{

public:
	ProcessingKernel();

	virtual ~ProcessingKernel() {}


	//MRJ: For some reason with Visual Studio 6.0 ( cl version 12.00.8168 )
	//     is not able to resolve the correct pointer for a derived class
	//     when accessing to a BaseClass::PureVirtualMethod implemented in
	//     the derived. Possible culprits:
	//          + Most surely, since the functor constructor is a virtual one
	//            through a tricky template function, *IS* possible that during
	//            some compile-time conversion, the compiler discards qualifiers
	//            over the reference to the derived class, thus producing a 
	//            "real pointer" (vcall) which seems to be some kind of compiler
	//            generated member pointer to identify pure virtual calls. Strange,
	//            huh?  Nonethless, this works right for patched Micro$oft compilers and
	//            gcc...

	virtual void ProcessingCleanup(){};

	virtual void ProcessingLoop(){};

	void AttachToThread( Thread& att_thread );

protected:

	bool LoopCondition();

	virtual bool UserCondition();

	void Cancel();

private:
	bool  mShouldRun;

};

}

#endif // ProcessingKernel.hxx

