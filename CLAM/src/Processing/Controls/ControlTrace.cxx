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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ControlTrace.hxx"
#include "Factory.hxx"
#include "XMLAdapter.hxx"
#include "XMLStorage.hxx"
#include "XMLIterableAdapter.hxx"
#include "XMLAdapter.hxx"
#include "XmlStorageErr.hxx"
#include <sstream>

namespace CLAM
{

ControlTraceEvent::ControlTraceEvent()
	: mRepeats(0)
{
}
ControlTraceEvent::ControlTraceEvent(const ControlTraceEvent &obj)
	: mRepeats(obj.mRepeats)
	, mValues(obj.mValues)
{
}
ControlTraceEvent::ControlTraceEvent(const InControlArray &inputs)
	: mRepeats(0)
{
	mValues.resize(inputs.Size());
	for (int i = 0; i < inputs.Size(); i++)
		mValues[i] = inputs[i].GetLastValue();
}

void ControlTraceEvent::UpdateControls(OutControlArray &outputs) const
{
	for (int i = 0; i < outputs.Size(); i++)
		outputs[i].SendControl(mValues[i]);
}

void ControlTraceEvent::LoadFrom( Storage& storage )
{
	XMLIterableAdapter<ValueArray> vAdapter(mValues, "x", "controls", true);
	storage.Load(vAdapter);

	XMLAdapter<unsigned int> sAdapter(mRepeats, "repeats", true);
	storage.Load(sAdapter);
}

void ControlTraceEvent::StoreOn( Storage& storage ) const
{
	XMLIterableAdapter<ValueArray> vAdapter(mValues, "x", "controls", true);
	storage.Store(vAdapter);

	if (mRepeats)
	{
		XMLAdapter<unsigned int> sAdapter(mRepeats, "repeats", true);
		storage.Store(sAdapter);
	}
}

const ControlTraceEvent& ControlTraceEvent::operator=(const ControlTraceEvent& rhs)
{
	mValues = rhs.mValues;
	return *this;
}

bool ControlTraceEvent::ValuesEqual(const ControlTraceEvent &rhs) const
{
	if (mValues.size() != rhs.mValues.size())
		return false;

	for (size_t i = 0; i < mValues.size(); i++)
	{
		if (mValues[i] != rhs.mValues[i])
			return false;
	}

	return true;
}

/* ================================================================== */

const unsigned int ControlTraceData::DumpVersion = 1;

ControlTraceData::ControlTraceData()
	: mVersion(DumpVersion)
{
}
ControlTraceData::ControlTraceData(const ControlTraceData &obj)
	: mVersion(DumpVersion), mEvents(obj.mEvents)
{
}

void ControlTraceData::LoadFrom( Storage& storage )
{
	mVersion = 0;
	XMLAdapter<unsigned int> versionAdapter(mVersion, "version");
	storage.Load(versionAdapter);
	if (!mVersion || mVersion > DumpVersion)
	{
		std::stringstream err;
		err << "Unknown CLAM Control Trace file version: " << mVersion;
		throw XmlStorageErr(err.str());
	}

	XMLIterableAdapter<EventList> adapter(mEvents, "event", "events", true);
	storage.Load(adapter);
}
void ControlTraceData::StoreOn( Storage& storage ) const
{
	XMLAdapter<unsigned int> versionAdapter(mVersion, "version");
	storage.Store(versionAdapter);

	XMLIterableAdapter<EventList> adapter(mEvents, "event", "events", true);
	storage.Store(adapter);
}

void ControlTraceData::Append(const ControlTraceEvent &data)
{
	ControlTraceEvent &prev = mEvents.back();
	if (prev.ValuesEqual(data))
		prev.WasRepeated();
	else
		mEvents.push_back(data);
}

/* ================================================================== */

static const char clamControlTraceFileTypeFamily[] = "CLAM Control Trace";
static const Filename::Filter clamControlTraceFileFilters[] = {
		{ "CLAM Control Traces (v1)", "*.clamtrace" },
	};

const char* ControlTraceInFilename::TypeFamily() const
{
	return clamControlTraceFileTypeFamily;
}
const Filename::Filter * ControlTraceInFilename::Filters() const
{
	return clamControlTraceFileFilters;
}

const char* ControlTraceOutFilename::TypeFamily() const
{
	return clamControlTraceFileTypeFamily;
}
const Filename::Filter * ControlTraceOutFilename::Filters() const
{
	return clamControlTraceFileFilters;
}

/* ================================================================= */

void ControlTraceWriterConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetNumberOfInputs(1.);
}

ControlTraceWriter::ControlTraceWriter()
{
	Configure(mConfig);
}

ControlTraceWriter::ControlTraceWriter( const ProcessingConfig& cfg )
{
	Configure( cfg );
}

ControlTraceWriter::~ControlTraceWriter()
{
	RemoveOldControls();
}

bool ControlTraceWriter::ConcreteConfigure( const ProcessingConfig& cfgObj )
{
	RemoveOldControls();
	CopyAsConcreteConfig( mConfig, cfgObj );
	if ( !mConfig.HasTraceFile() )
	{
		AddConfigErrorMessage("No 'trace file' was specified in the configuration!");
		return false;
	}

	ControlTraceOutFilename &file = mConfig.GetTraceFile();
	if ( file == "" )
	{
		AddConfigErrorMessage("No trace file selected");
		return false;
	}

	if (!mConfig.HasNumberOfInputs() || mConfig.GetNumberOfInputs() < 1.)
	{
		AddConfigErrorMessage("The number of inputs has not been configured.");
		return false;
	}

	mInputs.Resize(int(mConfig.GetNumberOfInputs()), "Input", this);
	return true;
}

bool ControlTraceWriter::ConcreteStop()
{
	XMLStorage::Dump(mTrace, "trace", mConfig.GetTraceFile().c_str());
	mTrace.Clear();
	return true;
}

bool ControlTraceWriter::Do()
{
	mTrace.Append(ControlTraceEvent(mInputs));
	return true;
}


void ControlTraceWriter::RemoveOldControls()
{
	mInputs.Clear();
	GetInControls().Clear();
}

/* ================================================================= */

void ControlTraceReaderConfig::DefaultInit()
{
	AddAll();
	UpdateData();
}

ControlTraceReader::ControlTraceReader()
{
	Configure(mConfig);
}

ControlTraceReader::ControlTraceReader( const ProcessingConfig& cfg )
{
	Configure( cfg );
}

ControlTraceReader::~ControlTraceReader()
{
	RemoveOldControls();
}

bool ControlTraceReader::ConcreteConfigure( const ProcessingConfig& cfgObj )
{
	RemoveOldControls();
	CopyAsConcreteConfig( mConfig, cfgObj );
	if ( !mConfig.HasTraceFile() )
	{
		AddConfigErrorMessage("No 'trace file' was specified in the configuration!");
		return false;
	}

	ControlTraceInFilename &file = mConfig.GetTraceFile();
	if ( file == "" )
	{
		AddConfigErrorMessage("No trace file selected");
		return false;
	}

	try {
		XMLStorage::Restore(mTrace, mConfig.GetTraceFile().c_str());
	}
	catch (XmlStorageErr &e)
	{
		AddConfigErrorMessage(e.what());
		return false;
	}

	if (mTrace.GetNumberOfControls() < 1)
	{
		AddConfigErrorMessage("The specified file does not contain any control events.");
		return false;
	}

	mOutputs.Resize(mTrace.GetNumberOfControls(), "Output", this);
	return true;
}

bool ControlTraceReader::ConcreteStart()
{
	mIterator = mTrace.Begin();
	mRepeatCounter = 0;
	return true;
}

bool ControlTraceReader::Do()
{
	if (mIterator == mTrace.End())
		return false;

	const ControlTraceEvent &event = *mIterator;

	// if this is the zeroth (first) step, update the controls
	if (!mRepeatCounter)
		event.UpdateControls(mOutputs);

	// if this step completes the current event, advance and reset counter
	if (mRepeatCounter++ == event.Repeats()) {
		mIterator++;
		mRepeatCounter = 0;
	}

	return true;
}

void ControlTraceReader::RemoveOldControls()
{
	mOutputs.Clear();
	GetOutControls().Clear();
}

/* ================================================================== */

namespace detail
{
typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::ControlTraceReader>
		regtControlTraceReader("ControlTraceReader");
static ProcessingFactory::Registrator<CLAM::ControlTraceWriter>
		regtControlTraceWriter("ControlTraceWriter");
}


} // CLAM namespace

