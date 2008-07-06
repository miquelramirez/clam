/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 * Copyright (c) 2007 Superlucidity Services, LLC and Zachary T Welch
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef CONTROL_TRACE_HXX
#define CONTROL_TRACE_HXX

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControlArray.hxx"
#include "OutControlArray.hxx"
#include "Filename.hxx"
#include <vector>

namespace CLAM
{

class ControlTraceEvent : public Component {
public:
	typedef std::vector<TControlData> ValueArray;

	ControlTraceEvent();
	ControlTraceEvent(const InControlArray &inputs);
	ControlTraceEvent(const ControlTraceEvent &event);
	const ControlTraceEvent& operator=( const ControlTraceEvent& );
	virtual ~ControlTraceEvent() { }

	const char *GetClassName() const { return "ControlTraceEvent"; }

	void LoadFrom( Storage& storage);
	void StoreOn( Storage& storage ) const;

	void WasRepeated() { mRepeats++; }
	unsigned int Repeats() const { return mRepeats; }

	size_t Size() const { return mValues.size(); }

	void UpdateControls(OutControlArray &array) const;

	bool ValuesEqual(const ControlTraceEvent &rhs) const;

private:
	unsigned int mRepeats;
	ValueArray mValues;
};

class ControlTraceData : public Component {
	static const unsigned int DumpVersion;
public:
	typedef std::list<ControlTraceEvent> EventList;
	virtual ~ControlTraceData() { }

	ControlTraceData();
	ControlTraceData(const ControlTraceData &obj);

	const char *GetClassName() const { return "ControlTraceData"; }

	unsigned int GetVersion() const { return mVersion; }

	void LoadFrom( Storage& storage);
	void StoreOn( Storage& storage ) const;

	size_t GetNumberOfControls() const { return mEvents.front().Size(); }

	void Append(const ControlTraceEvent &data);

	EventList::iterator Begin() { return mEvents.begin(); }
	EventList::iterator End() { return mEvents.end(); }

	void Clear() { mEvents.clear(); }

private:
	unsigned int mVersion;
	EventList mEvents;
};


class ControlTraceInFilename : public InFilename
{
public:
	ControlTraceInFilename(const std::string & s="") : InFilename(s) {}
	ControlTraceInFilename(const char * s) : InFilename(s) {}
	const char* TypeFamily() const;
	const Filter * Filters() const;
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, ControlTraceInFilename);

class ControlTraceReaderConfig  : public ProcessingConfig {
	DYNAMIC_TYPE_USING_INTERFACE
	( ControlTraceReaderConfig, 1, ProcessingConfig );

	DYN_ATTRIBUTE( 0, public, ControlTraceInFilename, TraceFile );

protected:
	void DefaultInit();
};

class ControlTraceReader : public Processing
{
public:
	ControlTraceReader();
	ControlTraceReader( const ProcessingConfig& cfg );
	virtual ~ControlTraceReader();

	const char* GetClassName() const { return "ControlTraceReader"; }
	const ProcessingConfig& GetConfig() const { return mConfig; }

	bool Do();

protected: // methods
	bool ConcreteConfigure( const ProcessingConfig& cfgObject );
	bool ConcreteStart();
	void RemoveOldControls();
	
protected: // attributes
	ControlTraceReaderConfig mConfig;
	ControlTraceData mTrace;
	ControlTraceData::EventList::iterator mIterator;
	unsigned int mRepeatCounter;
	OutControlArray mOutputs;
};


class ControlTraceOutFilename : public OutFilename
{
public:
	ControlTraceOutFilename(const std::string & s="") : OutFilename(s) {}
	ControlTraceOutFilename(const char * s) : OutFilename(s) {}
	const char* TypeFamily() const;
	const Filter * Filters() const;
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, ControlTraceOutFilename);

class ControlTraceWriterConfig  : public ProcessingConfig {
	DYNAMIC_TYPE_USING_INTERFACE
	( ControlTraceWriterConfig, 2, ProcessingConfig );

	DYN_ATTRIBUTE( 0, public, ControlTraceOutFilename, TraceFile );
	DYN_ATTRIBUTE( 1, public, TData, NumberOfInputs );

protected:
	void DefaultInit();
};

class ControlTraceWriter : public Processing
{
public:
	ControlTraceWriter();
	ControlTraceWriter( const ProcessingConfig& cfg );
	virtual ~ControlTraceWriter();

	const char* GetClassName() const { return "ControlTraceWriter"; }
	const ProcessingConfig& GetConfig() const { return mConfig; }

	bool Do();

protected: // methods
	bool ConcreteConfigure( const ProcessingConfig& cfgObject );
	bool ConcreteStop();
	void RemoveOldControls();
	
protected: // attributes
	ControlTraceWriterConfig mConfig;
	ControlTraceData mTrace;
	InControlArray mInputs;
};

} // CLAM namespace

#endif
