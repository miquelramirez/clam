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

#ifndef _Multiplier_
#define _Multiplier_

#include "Processing.hxx"
#include "ProcessingData.hxx"

namespace CLAM
{

/** Configuration class for Multiplier objects
 *  @see Multiplier,ProcessingConfig
 */
class MultiplierConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MultiplierConfig, 1, ProcessingConfig);

	/** Dynamic attribute with the name of the ProcessingObject*/
	DYN_ATTRIBUTE (0, public, std::string, Name);
protected:
	void DefaultInit(void)
	{
		AddName();

		UpdateData();
	}
};


	/** This class is a ProcessingObject created to multiply a pair of audio chunks. Method Do receives two Audio objects of input, and returns another Audio object result of multiplying signals bin to bin.
	 *  @see Processing, Audio, MultiplierConfig
	 */
class Multiplier: public Processing
{
private:
	MultiplierConfig mConfig;
public:

	/** Constructor of the class, receives by parameter an instantiation of MultiplierConfig class
	 *  @param c Configuration of the new object, calls the base constructor by default
	 */
	Multiplier(const MultiplierConfig& c = MultiplierConfig())
	{
		Configure(c);
	}

	const char * GetClassName () const { return "Multiplier";}

	/** This method allows to obtain the MultiplierConfig object of the class
	 *  @return The configuration of the class
	 */
	const ProcessingConfig &GetConfig() const { return mConfig;}

	/** Configuration change method
	 *  @throw
	 *  bad_cast exception when the argument is not an MultiplierConfig object.
	 *  @param cfg The configuration object
	 *  @return A boolean value, true if method has been executed correctly
	 */
 	bool ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const MultiplierConfig&>(cfg);
				
		return true;
	}


	/** Supervised-mode Do function.Not implemented yet.
	 */
	bool Do(void) { return true; }

	/** Non-supervised-mode Do function. 
	 *  @param inA One of the Audio chunks to be multiplied
	 *  @param inB The other Audio object
	 *  @param out Audio instantiation with the results of the operation.
	 *  @return A boolean value, true if method has been executed correctly.
	 */ 
	bool Do(const Audio& inA,const Audio& inB,Audio& out)
	{
		TData* ptrA = inA.GetBuffer().GetPtr();
		TData* ptrB = inB.GetBuffer().GetPtr();
		TData* ptr = out.GetBuffer().GetPtr();
		for (int i=0;i<out.GetSize();i++)
		{
			(*ptr++) = (*ptrA++) * (*ptrB++);
		}
		return true;
	}

	/** Storage class, not implemented yet*/
	void StoreOn(Storage &s) {};
};
	
}

#endif


