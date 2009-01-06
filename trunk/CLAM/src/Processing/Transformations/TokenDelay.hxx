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

// TokenDelay.hxx

#ifndef _TokenDelay_
#define _TokenDelay_

#include "Processing.hxx"
#include "InControl.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Component.hxx"
#include "Enum.hxx"
#include <string>
#include <deque>

#include "Array.hxx"

namespace CLAM  
{


/**
* Configuration of the TokenDelay processing object. It includes tree
* dynamic type attributes: Name, Delay and MaxDelay
*/
class TokenDelayConfig : public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (TokenDelayConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, unsigned, Delay);
	DYN_ATTRIBUTE (1, public, unsigned, MaxDelay );
protected:
	void DefaultInit(void)
	{
		AddDelay();
		AddMaxDelay();
		UpdateData();
		SetDelay(0);
		SetMaxDelay(1);  
	}

};

/**
 * A Processing Object that delays a certain number of ProcessingDataObjects
 * wherever the specific object it is. <b> This class is only usefull
 * in the non supervised context</b> so it hasn't the generic Do() implemented
 * It works buffering these ProcessingDataObjects, feeded by 
 * using the arguments of the Do(...) method.
 * <b> Important: </b> the processig data objects (tokens) that flows through 
 * the  token delay can only be deleted once have got out from the delay.
 * It can also happen that decrementing the delay value, some tokens will never
 * get out. And these tokens will be deleted by the delay object.
 * 
 */
template <class T> class TokenDelay : public Processing
{

private:
	/** TokenDelay Configuration */
	TokenDelayConfig mConfig;
	// Ports and control
	InPort<T> mInput;
	OutPort<T> mOutput;
	FloatInControl mDelayControl;

public:
	TokenDelay(const TokenDelayConfig& cfg = TokenDelayConfig() ) :
		mInput ("In", this),
		mOutput ("Out", this),
		mDelayControl("Delay Control", this),
		mCapacity(0)
	{
		Configure(cfg);
	}
	virtual ~TokenDelay<T>() {};
	
	/**
	* Concrete configuration change method. The generic  calls
	* this one. It will accept such changes when not in running mode.
	* @pre argument should be a TokenDelayConfig
	*/
	bool ConcreteConfigure(const ProcessingConfig& c);

	/**
	* Configuration access
	*/
	virtual const ProcessingConfig &GetConfig() const { return mConfig;}
	/**
	* Supervised-mode execution method. Because this class only works in non-supervised
	* mode, it hrows an error
	*/
	virtual bool Do(void);

	/**
	* Non-supervised-mode execution method.
	* @param in Reference to the input ProcessingData object.
	* @param out Pointer to a ProcessingData that has previously been entered.
	* this object pointed by out parameter can be safely deleted only when 
	* the last call to this Do method has output a different object. In others 
	* words: when the delay value gets increased, the out object can be the same
	* for a number of Do executions.
	*/
	virtual bool Do(const T & in, T& out);

	

	/** 
	*	Debug method for viewing all the relevant parameter */
	void Debug() const;
	/**
	* It cheks many invariants restrictions of the object and throws an error if they 
	* are not fulfilled. Very usefull when developing or maintaining this class. */
	void FulfillsInvariant() const;

	const char *GetClassName() const {return "TokenDelay";}

	/**
	 * Informative value about the current delay applied (different from the requested)
	 */
	TSize RealDelay() const {
		return mTokenQueue.size();
	} 

	unsigned MaxDelay() const {
		return mCapacity;
	}

	unsigned GivenDelay() {

		return CastDelayControlValue(mDelayControl.GetLastValue());
	}	

private:
	/** This method is applyed to every token discarded when the decreasing the delay amount*
	 *  In this class the implementation is just "delete toDiscard"  <br/>
	 *  But we can take different approaches by deriving from TokenDelay<T> and overriding 
	 *  this method. For example we could use a token pool for reusing them.
	 */
	virtual void Discard(T* toDiscard);

	/**
	 * Returns a valid delay value given a control data value
	 */
	unsigned CastDelayControlValue(TControlData readControlValue);
	/** 
	 * Adjusts the internal circular buffer to adapt to the new
	 * delay.
	 */
	void UpdateBuffersToDelay();
	void WriteNewSample(const T& in);
	void ReadCurrentSample(T& out);
	
// Implementation Details:

	std::vector<T> mTokenQueue;
	unsigned mReadPointer;
	unsigned mWritePointer;
	/** The maximun number of elements in the queue. */
	unsigned mCapacity;
	/** The control value read on the last started Do */
	unsigned mGivenDelay;
	/** The control value read on the previous Do */
	unsigned mLastDelay;

};



/////////////////////////////////////////////////////////////////////////////////////////777
// Class implementation


#include "Err.hxx"

template <class T> 
void TokenDelay<T>::Discard(T* toDiscard) {
	CLAM_ASSERT(toDiscard, "TokenDelay: Discarding a null pointer");
	delete toDiscard;
}
	
template <class T> 
bool TokenDelay<T>::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	// TODO: Put some sanity checks on the configuration here
	mCapacity = mConfig.GetMaxDelay();
	mDelayControl.DoControl(TControlData(mConfig.GetDelay()));
	mGivenDelay = CastDelayControlValue(mDelayControl.GetLastValue());
	mTokenQueue.resize(mCapacity);
	mWritePointer = 0;
	mReadPointer = -mGivenDelay;
	return true;
}


template <class T> 
bool TokenDelay<T>::Do(void)
{
	Do(mInput.GetData(), mOutput.GetData());
	mInput.Consume();
	mOutput.Produce();
	return false;
}

template <class T>
unsigned TokenDelay<T>::CastDelayControlValue(TControlData readControlValue) {
	if (readControlValue > mCapacity) return mCapacity;
	if (readControlValue < 0) return 0;
	return unsigned(readControlValue);
}

template <class T> 
bool TokenDelay<T>::Do(const T& in, T& out)
// implementation using the supervised-mode Do
{
	// @todo debug
	mLastDelay = mGivenDelay;
	mGivenDelay = CastDelayControlValue(mDelayControl.GetLastValue());
	// If the value is different make the difference efective
	if (mLastDelay != mGivenDelay)
		UpdateBuffersToDelay();
	WriteNewSample(in);
	ReadCurrentSample(out);
	return true;
}

template <class T> 
void TokenDelay<T>::WriteNewSample(const T& in)
{
	mTokenQueue[mWritePointer++] = in;
	if(mWritePointer==mCapacity) mWritePointer=0;

}


template <class T>
void TokenDelay<T>::ReadCurrentSample(T& out)
{
	CLAM_DEBUG_ASSERT(mReadPointer>=0, "Trying to read outside buffer");
	out = mTokenQueue[mReadPointer++];
	if(mReadPointer==mCapacity) mReadPointer=0;
}



// Delay change
template <class T> 
void TokenDelay<T>::UpdateBuffersToDelay()
{
	mWritePointer =  mReadPointer + CastDelayControlValue(mGivenDelay);
	if(mWritePointer>=mCapacity) mWritePointer-=mCapacity;
	return;
}


template <class T> 
void TokenDelay<T>::Debug() const
{
	/*
	unsigned real = RealDelay();
	unsigned size = mVector.size();
	unsigned cap = mVector.capacity();

	std::cout << "\n-- " << mConfig.GetClassName() << "\n-- (first,last)=(" << mFirst <<","<< mLast <<")\n-- "\
		<< "(given,real delay)=("<< mGivenDelay <<"," << real << ")\n-- (vector size,capacity;mCapacity)=("\
		<< size <<","<< cap <<";"<< mCapacity <<")\n-- (mInstantToken,mVector[mFirst])=("<< mInstantToken\
		<<","<< mVector[mFirst] <<")\n";
	*/

}

template <class T> 
void TokenDelay<T>::FulfillsInvariant() const
{/*
	unsigned real = RealDelay();
	
	if (mVector.capacity() < mCapacity) throw Err("TokenDelay : invariant not fullfilled!: vector capacity < req. capacity");
	if (real && mFirst==mLast) throw Err("TokenDelay : invariant not fullfilled!: there is a 'real' delay and mFirst==mLast");
	if (real > mCapacity) throw Err("TokenDelay : invariant not fullfilled!: real delay > mCapacity");
	if (mGivenDelay > mCapacity) throw Err("TokenDelay : invariant not fullfilled!: given (by control) delay > mCapacity");
	if (mFirst <0 || mLast<0 || mCapacity <= 0)  throw Err("TokenDelay : invariant not fullfilled!: some very bad thing...");
*/
}

}; //namespace CLAM



#endif //_TokenDelay_
