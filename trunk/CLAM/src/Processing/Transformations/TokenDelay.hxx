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

// TokenDelay.hxx

#ifndef _TokenDelay_
#define _TokenDelay_

#include "Processing.hxx"
//#include "ProcessingData.hxx"
#include "InControl.hxx"
#include "Port.hxx"
#include "Component.hxx"
#include "Enum.hxx"
#include <vector>
#include <string>

using std::string;

namespace CLAM  
{


/**
* Configuration of the TokenDelay processing object. It includes tree
* dynamic type attributes: Name, Delay and MaxDelay
*/
class TokenDelayConfig : public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (TokenDelayConfig, 3, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, unsigned, Delay);
	DYN_ATTRIBUTE (2, public, unsigned, MaxDelay );
protected:
	/** 
	* Dynamic type initialization: all attributes are instantiated.
	* ActualDelay is set to 0 and MaxDelay to a default value (i.e. 1000 tokens)
	* 
	*/
	void DefaultInit(void)
	{
		AddName();
		AddDelay();
		AddMaxDelay();
		UpdateData();
		SetName("");
		SetDelay(0);
		SetMaxDelay(1000);  //this value has no importance. It should be re-set by the user.	
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
	InPortTmpl<T> mInput;
	OutPortTmpl<T> mOutput;
	InControlTmpl< TokenDelay<T> > mDelayControl;

	/** Control change callback function */
	int ChangeDelay(TControlData d);

public:
	TokenDelay(const TokenDelayConfig& cfg = TokenDelayConfig() ) :
		mInput ("In", this, 1),
		mOutput ("Out", this, 1),
		mDelayControl("Delay Control", this, &CLAM::TokenDelay<T>::ChangeDelay),
		mLast(0),
		mFirst(0),
		mInstantToken(0)
	{
		Configure(cfg);
	}
	virtual ~TokenDelay<T>() {};
	
	/**
	* Concrete configuration change method. The generic  calls
	* this one. It will accept such changes when not in running mode.
	* @throw bad_cast exception when the argument is not a TokenDelayConfig object.
	*/
	bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast);

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
	virtual bool Do(T & in, T* & out);

	

	/** 
	*	Debug method for viewing all the relevant parameter */
	void Debug() const;
	/**
	* It cheks many invariants restrictions of the object and throws an error if they 
	* are not fulfilled. Very usefull when developing or maintaining this class. */
	void FulfillsInvariant() const;


	const char *GetClassName() {return "TokenDelay";}

	unsigned RealDelay() const;


private:
// Circular buffer interface:
	T* PopFirst();
	void PushLast( T* in);
// Implementation Details:

	std::vector<T*> mVector;
	unsigned mLast;
	unsigned mFirst;
	unsigned mCapacity;
	T* mInstantToken;

};



/////////////////////////////////////////////////////////////////////////////////////////777
// Class implementation


#include "Err.hxx"
	
// Control change callback function 
template <class T> 
int TokenDelay<T>::ChangeDelay(TControlData d)
{
	unsigned i;

	if (d >= mCapacity) {
		d = TControlData(mCapacity);
//		std::cout << "Token Delay: Maximum delay reached. Can't delay more than "<< d << " tokens.\n";
		return -1;
	}
	long incr =(long int) d-RealDelay();
	if (incr > 0) ;// mLast will increment step by step and mFirst will keep the same till the delay is reached.
	//	if (incr + mLast < mCapacity) mLast += incr;
	//	else mLast = incr - mCapacity - 1 + mLast;
	else 
		// the delay has decremented. So it's necessary to delete some unusefull data
		if ( int(mLast+incr) >= 0) // mLast >= decrement
		{
			for (i=mLast+incr; i<mLast; i++)  delete mVector[i];
			mLast += incr;
		}
		else
		{ // the same, but turning the vector.
			for (i=mLast; i>0; i--) delete mVector[i];
			mLast = mCapacity + incr + mLast;
			for (i=mCapacity-1; i>mLast; i--) delete mVector[i];
		};

	return 0;
}

template <class T> 
bool TokenDelay<T>::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const TokenDelayConfig&>(c);
	mCapacity = mConfig.GetMaxDelay();
	mVector.resize(mCapacity);
	mDelayControl.DoControl(TControlData(mConfig.GetDelay()));
	return true;
}


template <class T> 
bool TokenDelay<T>::Do(void)
{
	throw Err("TokenDelay : the Do() with parameters should be used instead of this one.");
	return false;
}

template <class T> 
bool TokenDelay<T>::Do(T& in, T* & out)
// implementation using the supervised-mode Do
{
	if (mDelayControl.GetLastValue()>0 || RealDelay()>0) {
		
		out = PopFirst();
		PushLast(&in);
		
		mInstantToken=0;
		if (!out) mInstantToken=out=&in;
	} else mInstantToken = out = &in;

	//For debugging :
	//Debug();
	//FulfillsInvariant();

	return true;
}


// Circular buffer interface:
template <class T> 
T* TokenDelay<T>::PopFirst()
{
	unsigned givenDelay = unsigned(mDelayControl.GetLastValue());
	unsigned realDelay = unsigned(RealDelay());
	T* ret;

#ifdef HAVE_STANDARD_VECTOR_AT
	if (mInstantToken) mVector.at(mFirst) = mInstantToken;
	if (givenDelay) ret = mVector.at(mFirst);
	else throw Err("TokenDelay at PopFirst() : givenDelay==0");
#else
	if (mInstantToken) mVector[mFirst] = mInstantToken;
	if (givenDelay) ret = mVector[mFirst];
	else throw Err("TokenDelay at PopFirst() : givenDelay==0");
#endif

	if (realDelay == givenDelay) 
	{
		if (mFirst < mCapacity-1) mFirst++; 
		else mFirst = 0;
		return ret;
	}
	else if (realDelay > givenDelay) throw Err("TokenDelay at PopFirst() : realDelay>givenDelay");
	return ret;
}

template <class T> 
void TokenDelay<T>::PushLast(T* in)
{
	// provisional test:
	if(mLast <= mCapacity-1) 
#		ifdef HAVE_STANDARD_VECTOR_AT
			mVector.at(mLast) = in;
#		else
			mVector[mLast] = in;
#		endif
	else throw Err("TokenDelay at PushLast: mLast >= mCapacity");

		if(mLast == mCapacity-1)
		{ // making the turn of the circular buffer.
			if( mFirst==0 ) throw Err("Token Delay at PushLast : Limit delay");
			mLast=0;
		} 
		else mLast++;
}

template <class T> 
unsigned TokenDelay<T>::RealDelay() const
{
	/** mLast points to the next place to write. and mFirst is the next element to be pop.
	*/
	return (mLast>=mFirst) ? mLast - mFirst : (mCapacity - mFirst + mLast);
}

template <class T> 
void TokenDelay<T>::Debug() const
{
	unsigned given = unsigned(mDelayControl.GetLastValue());
	unsigned real = RealDelay();
	unsigned size = mVector.size();
	unsigned cap = mVector.capacity();

	std::cout << "\n-- " << mConfig.GetName() << "\n-- (first,last)=(" << mFirst <<","<< mLast <<")\n-- "\
		<< "(given,real delay)=("<< given <<"," << real << ")\n-- (vector size,capacity;mCapacity)=("\
		<< size <<","<< cap <<";"<< mCapacity <<")\n-- (mInstantToken,mVector[mFirst])=("<< mInstantToken\
		<<","<< mVector[mFirst] <<")\n";

}

template <class T> 
void TokenDelay<T>::FulfillsInvariant() const
{
	unsigned real = RealDelay(), given=unsigned(mDelayControl.GetLastValue());
	
	if (mVector.capacity() < mCapacity) throw Err("TokenDelay : invariant not fullfilled!: vector capacity < req. capacity");
	if (real && mFirst==mLast) throw Err("TokenDelay : invariant not fullfilled!: there is a 'real' delay and mFirst==mLast");
	if (real > mCapacity) throw Err("TokenDelay : invariant not fullfilled!: real delay > mCapacity");
	if (given > mCapacity) throw Err("TokenDelay : invariant not fullfilled!: given (by control) delay > mCapacity");
	if (mFirst <0 || mLast<0 || mCapacity <= 0)  throw Err("TokenDelay : invariant not fullfilled!: some very bad thing...");

}

// Control Enumeration

class ETokenDelayControls
: public Enum
{

public:
	static tEnumValue sEnumValues[];
	static tValue     sDefault;

	ETokenDelayControls()
		: Enum( sEnumValues, sDefault )
	{
	}

	ETokenDelayControls( tValue v )
		: Enum( sEnumValues, v )
	{
	}
	
	ETokenDelayControls( string s )
			: Enum( sEnumValues, s )
	{
	}

	~ETokenDelayControls()
	{
	}

	Component* Species() const
	{
		return new ETokenDelayControls;
	}

	typedef enum 
		{ 
			delay = 0
		} tEnum;

};

}; //namespace CLAM



#endif //_TokenDelay_
