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

#ifndef DataDelay_hxx
#define DataDelay_hxx

/*#include "Processing.hxx"
#include "InControl.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Component.hxx"
#include "Enum.hxx"
*/
#include "TokenDelay.hxx"
#include <string>
#include <vector>

using std::string;

namespace CLAM  
{

//typedef TokenDelay<TData> DataDelay;

class DataDelay:public TokenDelay<TData>
{
	const char *GetClassName() const {return "DataDelay";}	
	
};

};

/**
* Configuration of the DataDelay processing object. It includes tree
* dynamic type attributes: Name, Delay and MaxDelay
*/

#if 0
class DataDelayConfig : public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DataDelayConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, unsigned, Delay);
	DYN_ATTRIBUTE (1, public, unsigned, MaxDelay );
protected:
	/** 
	* Dynamic type initialization: all attributes are instantiated.
	* ActualDelay is set to 0 and MaxDelay to a default value (i.e. 1000 tokens)
	* 
	*/
	void DefaultInit(void)
	{
		AddDelay();
		AddMaxDelay();
		UpdateData();
		SetDelay(0);
		SetMaxDelay(10);  
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
class DataDelay : public Processing
{

private:
	/** DataDelay Configuration */
	DataDelayConfig mConfig;
	// Ports and control
	InPort<TData> mInput;
	OutPort<TData> mOutput;
	FloatInControl mDelayControl;

public:
	DataDelay(const DataDelayConfig& cfg = DataDelayConfig() ) :
		mInput ("In", this),
		mOutput ("Out", this),
		mDelayControl("Delay Control", this),
		mCapacity(0)
	{
		Configure(cfg);
	}
	virtual ~DataDelay() {};
	
	/**
	* Concrete configuration change method. The generic  calls
	* this one. It will accept such changes when not in running mode.
	* @pre argument should be a DataDelayConfig
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
//	virtual bool Do(void);

	/**
	* Non-supervised-mode execution method.
	* @param in Reference to the input ProcessingData object.
	* @param out Pointer to a ProcessingData that has previously been entered.
	* this object pointed by out parameter can be safely deleted only when 
	* the last call to this Do method has output a different object. In others 
	* words: when the delay value gets increased, the out object can be the same
	* for a number of Do executions.
	*/
//	virtual bool Do(const TData & in, TData& out);

	

	/** 
	*	Debug method for viewing all the relevant parameter */
//	void Debug() const;
	/**
	* It cheks many invariants restrictions of the object and throws an error if they 
	* are not fulfilled. Very usefull when developing or maintaining this class. */
//	void FulfillsInvariant() const;


	const char *GetClassName() const {return "DataDelay";}

	unsigned MaxDelay() const {
		return mCapacity;
	}

	unsigned GivenDelay() {

		return CastDelayControlValue(mDelayControl.GetLastValue());
	}	

private:
	void WriteNewData(TData in);
	TData ReadCurrentData();
	/**
	 * Returns a valid delay value given a control data value
	 */
//	unsigned CastDelayControlValue(TControlData readControlValue);
	/** 
	 * Adjusts the internal circular buffer to adapt to the new
	 * delay.
	 */
//	void UpdateBuffersToDelay();


// Implementation Details:

//	std::vector<TData> mBuffer;
	/** The maximun number of elements in the queue. */
//	unsigned mCapacity;
	/** The control value read on the last started Do */
//	TControlData mGivenDelay;
	/** The control value read on the previous Do */
//	TControlData mLastDelay;

//	unsigned mReadPointer;
//	unsigned mWritePointer;

//};

// Control Enumeration

class EDataDelayControls
: public Enum
{

public:
	EDataDelayControls() : Enum( ValueTable(), delay ) { } 
	EDataDelayControls( tValue v ) : Enum( ValueTable(), v ) { } 
	EDataDelayControls( string s ) : Enum( ValueTable(), s ) { } 
	~EDataDelayControls() { } 
	Component* Species() const
	{
		return new EDataDelayControls;
	}

	typedef enum 
	{ 
		delay = 0
	} tEnum;
	static tEnumValue * ValueTable()
	{
		static tEnumValue sEnumValues[] =
		{
			{ delay, "delay" },
			{ 0, NULL }
		};
		return sEnumValues;
	}

};

}; //namespace CLAM

#endif //commented out code

#endif //DataDelay_hxx

