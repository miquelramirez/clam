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

#ifndef _PORT_H
#define _PORT_H

#include <string>
#include "mtgsstream.h"
#include "Processing.hxx"
#include "Assert.hxx"
#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "Node.hxx"

namespace CLAM {

	class ProcessingData;
	class Spectrum;
	class Audio;
	class Envelope;
	class Peak;

	class DataVisitor
	{
		void ErrMsg(std::string&,const std::string&);
	public:
		virtual const std::string& ClassName() =0;
		virtual void Visit(ProcessingData& data);
		virtual void Visit(Spectrum& data);
		virtual void Visit(Audio& data);
		virtual void Visit(Envelope& data);
		virtual void Visit(Peak& data);
	};

	/** Processing object port base class */
	class Port
	{
		/** Port name */
		std::string mName;
		/** Processing object to which this port belongs */
		Processing *mObject;
		/** Number of data objects used in each Do */
		unsigned int mLength;
		/** Number of data objects discarded in each Do */
		unsigned int mHop;
	public:
		Port(const std::string &n, Processing *o, int l, int hop = 0) : 
			mName(n),
			mObject(o),
			mLength(l),
			mHop(hop?hop:l)
		{};

		virtual ~Port() {}
		virtual ProcessingData &GetProcessingData() = 0;
		virtual void Accept(DataVisitor&) = 0;
		unsigned int Length() const { return mLength; }
		unsigned int Hop() const { return mHop; }
		void SetParams(unsigned int length,
		               unsigned int hop = 0)
		{
			mLength = length;
			mHop = hop?hop:length;
		}
	};

	/** Input port */
	class InPort: public Port
	{
		/** True if the processing object may use the storage object
		 * in this port to write on it the data of an output port
		 */
		bool mCanDoInplace;

	public:
		InPort(const std::string &n, Processing *o, int length, int hop = 0, bool inplace=false);
		virtual ~InPort() {}

		virtual void Attach(ProcessingData& data)=0;
		
		bool CanDoInplace(void)  {return mCanDoInplace;}

	};

	class OutPort: public Port {
	public:
		OutPort(const std::string &n, Processing *o, int length, int hop = 0);
		
		virtual void Attach(ProcessingData& data)=0;

		virtual ~OutPort() {}
	};


	template<class T>
	class InPortTmpl : public InPort
	{
		ReadStreamRegion *mpRegion;
		Node<T> *mpNode;
		Array<T> mData;
		ProcessingData &GetProcessingData() { return mData[0]; }
	public:
		inline InPortTmpl(const std::string &n, Processing *o, int length, int hop = 0, bool inplace=false);
		inline T &GetData();
		inline void LeaveData();
		void Attach(ProcessingData& data);
		inline void Attach(T& data);
		inline void Attach(Node<T> &n);
		inline void Attach(InPortTmpl<T> &p); // For composites
		inline void Accept(DataVisitor&);
	};

	template<class T>
	class OutPortTmpl : public OutPort
	{
		WriteStreamRegion *mpRegion;
		Node<T> *mpNode;
		Array<T> mData;
		ProcessingData &GetProcessingData() { return mData[0]; }
	public:
		inline OutPortTmpl(const std::string &n, Processing *o, int length, int hop = 0);
		inline T &GetData();
		inline void LeaveData();
		void Attach(ProcessingData& data);
		inline void Attach(T& data);
		inline void Attach(Node<T> &n);
		inline void Attach(OutPortTmpl<T> &p); // For composites
		inline void Accept(DataVisitor&);
	};

	template<class T>
	class InPortArrayTmpl
	{
		Array<InPortTmpl<T>*> mArray;
		Array<T*> mDataPointers;
	public:
		inline InPortArrayTmpl(int size,
		                       const std::string &n,
		                       Processing *o,
		                       int length,
		                       int hop = 0,
		                       bool inplace=false);

		inline ~InPortArrayTmpl();

		Array<T*> &GetDataArray();
		void LeaveDataArray();

		void SetParams(unsigned int length,
		               unsigned int hop = 0);

		InPortTmpl<T>       &operator[](int i)        { return *mArray[i]; }
		const InPortTmpl<T> &operator[](int i) const  { return *mArray[i]; }
	};

	template<class T>
	class OutPortArrayTmpl
	{
		Array<OutPortTmpl<T>*> mArray;
		Array<T*> mDataPointers;
	public:
		inline OutPortArrayTmpl(int size,
		                        const std::string &n,
		                        Processing *o,
		                        int length,
		                        int hop,
		                        bool inplace=false);

		inline ~OutPortArrayTmpl();

		Array<T*> &GetDataArray();
		void LeaveDataArray();

		void SetParams(unsigned int length,
		               unsigned int hop = 0);

		OutPortTmpl<T>       &operator[](int i)        { return *mArray[i]; }
		const OutPortTmpl<T> &operator[](int i) const  { return *mArray[i]; }
	};

	// Template method implementations


	template<class T>
	InPortTmpl<T>::InPortTmpl(const std::string &n,
	                          Processing *o,
	                          int length,
	                          int hop,
	                          bool inplace)
		: InPort(n,o,length,hop,inplace),
		  mpRegion(0),
		  mpNode(0)
	{
		o->PublishInPort(this);
	}

	template<class T>
	T &InPortTmpl<T>::GetData()	
	{ 
		CLAM_ASSERT(mData.Size() || (mpNode && mpRegion),
		            "InPortTmpl::GetData(): No data atached to the port.");
		if (mpNode)
			mpNode->GetAndActivate(mpRegion,mData);
		return mData[0];
	}

	template<class T>
	void InPortTmpl<T>::LeaveData()	
	{
		if (mpNode)
			mpNode->LeaveAndAdvance(mpRegion);
	}

	template<class T>
	void InPortTmpl<T>::Attach(ProcessingData& data)
	{
		try{ 
			Attach(dynamic_cast<T&>(data));
		}
		catch (std::bad_cast){
			CLAM_ASSERT(false,"You are trying to attach a processing data that is not suitable for this port");
		}
	}	

	template<class T>
	void InPortTmpl<T>::Attach(T& data)
	{
		mData.SetPtr(&data,1);
	}	

	template<class T>
	void InPortTmpl<T>::Attach(Node<T>& node)
	{
		mData.SetPtr(0);
		mpNode = &node;
		mpRegion = node.NewReader(this, Hop(), Length());
	}	

	template<class T>
	void InPortTmpl<T>::Attach(InPortTmpl<T>& port)
	{
		if (port.mpNode)
			Attach(*port.mpNode);
		else
			Attach(*port.mpData);
	}	

	template<class T>
	void InPortTmpl<T>::Accept(DataVisitor& v)
	{
		int i;
		for (i=0; i<mData.Size(); i++)
			v.Visit(mData[i]);
	}

	template<class T>
	OutPortTmpl<T>::OutPortTmpl(const std::string &n,
	                            Processing *o,
	                            int length,
	                            int hop)
		: OutPort(n,o,length),
		  mpRegion(0),
		  mpNode(0)
	{
		o->PublishOutPort(this);
	}

	template<class T>
	void OutPortTmpl<T>::Attach(ProcessingData& data)
	{
		try{ 
			Attach(dynamic_cast<T&>(data));
		}
		catch (std::bad_cast){
			CLAM_ASSERT(false,"You are trying to attach a processing data that is not suitable for this port");
		}
	}	
	
	template<class T>
	void OutPortTmpl<T>::Attach(T& data)
	{
		mData.SetPtr(&data,1);
	}	

	template<class T>
	void OutPortTmpl<T>::Attach(Node<T>& node)
	{
		mData.SetPtr(0);
		mpNode = &node;
		mpRegion = node.NewWriter(this, Hop(), Length());
	}

	template<class T>
	void OutPortTmpl<T>::Attach(OutPortTmpl<T>& port)
	{
		if (port.mpNode)
			Attach(*port.mpNode);
		else
			Attach(*port.mpData);
	}	

	template<class T>
	T &OutPortTmpl<T>::GetData()	
	{ 
		CLAM_ASSERT(mData.Size() || (mpNode && mpRegion),
		            "OutPortTmpl::GetData(): No data atached to the port.");
		if (mpNode)
			mpNode->GetAndActivate(mpRegion,mData);
		return mData[0];
	}

	template<class T>
	void OutPortTmpl<T>::LeaveData()	
	{
		if (mpNode)
			mpNode->LeaveAndAdvance(mpRegion);
	}

	template<class T>
	void OutPortTmpl<T>::Accept(DataVisitor& v)
	{
		int i;
		for (i=0; i<mData.Size(); i++)
			v.Visit(mData[i]);
	}


	template<class T>
	InPortArrayTmpl<T>::InPortArrayTmpl(int size,
	                                    const std::string &n,
	                                    Processing *o,
	                                    int length,
	                                    int hop,
	                                    bool inplace)
	{
		mArray.Resize(size);
		mArray.SetSize(size);
		mDataPointers.Resize(size);
		mDataPointers.SetSize(size);
		for (int i=0; i<size; i++) {
			std::stringstream str(n);
			str << "_" << i;
			mArray[i] = new InPortTmpl<T>(str.str(),o,length,hop,inplace);
		}
	}

	template<class T>
	InPortArrayTmpl<T>::~InPortArrayTmpl()
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			delete mArray[i];
	}

	template<class T>
	void InPortArrayTmpl<T>::SetParams(unsigned int length,
	                                   unsigned int hop)
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			mArray[i]->SetParams(length,hop);
	}

	template<class T>
	Array<T*> &InPortArrayTmpl<T>::GetDataArray()
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			mDataPointers[i] = &mArray[i]->GetData();
		return mDataPointers;
	}

	template<class T>
	void InPortArrayTmpl<T>::LeaveDataArray()
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			mArray[i]->LeaveData();
	}

	template<class T>
	OutPortArrayTmpl<T>::OutPortArrayTmpl(int size,
	                                      const std::string &n,
	                                      Processing *o,
	                                      int length,
	                                      int hop,
	                                      bool inplace)
	{
		mArray.Resize(size);
		mArray.SetSize(size);
		mDataPointers.Resize(size);
		mDataPointers.SetSize(size);
		for (int i=0; i<size; i++) {
			std::stringstream str(n);
			str << "_" << i;
			mArray[i] = new OutPortTmpl<T>(str.str(),o,length,hop,inplace);
		}
	}

	template<class T>
	OutPortArrayTmpl<T>::~OutPortArrayTmpl()
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			delete mArray[i];
	}

	template<class T>
	void OutPortArrayTmpl<T>::SetParams(unsigned int length,
	                                    unsigned int hop )
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			mArray[i]->SetParams(length,hop);
	}

	template<class T>
	Array<T*> &OutPortArrayTmpl<T>::GetDataArray()
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			mDataPointers[i] = &mArray[i]->GetData();
		return mDataPointers;
	}

	template<class T>
	void OutPortArrayTmpl<T>::LeaveDataArray()
	{
		int size = mArray.Size();
		for (int i=0; i<size; i++)
			mArray[i]->LeaveData();
	}

}

// Port specializations
#include "AudioPort.hxx"


#endif // _PORT_H
