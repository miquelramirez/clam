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


#include "Processing.hxx"
#include "ProcessingComposite.hxx"
#include "TopLevelProcessing.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Network.hxx"
#include "TypedInControl.hxx"
#include "TypedOutControl.hxx"

#include <cstring>
#include <string>


namespace CLAM
{
	void ConnectPorts(
			Processing & sender, const std::string & outPortName, 
			Processing & receiver, const std::string & inPortName )
	{
		OutPortBase & out = sender.GetOutPort(outPortName);
		InPortBase & in = receiver.GetInPort(inPortName);
		out.ConnectToIn(in);
	}

	void ConnectControls(
			Processing & sender, const std::string & outControlName, 
			Processing & receiver, const std::string & inControlName )
	{
		OutControl & out = sender.GetOutControl(outControlName);
		InControl & in = receiver.GetInControl(inControlName);
		out.AddLink(in);
	}
	
	void ConnectTypedControls(
			Processing & sender, const std::string & typedOutControlName, 
			Processing & receiver, const std::string & typedInControlName )
	{
		BaseTypedOutControl & out = sender.GetTypedOutControl(typedOutControlName);
		BaseTypedInControl & in = receiver.GetTypedInControl(typedInControlName);
		out.AddLink(in);
	}
	
	void ConnectPorts(
			Processing & sender, unsigned outPortNumber, 
			Processing & receiver, unsigned inPortNumber )
	{
		OutPortBase & out = sender.GetOutPort(outPortNumber);
		InPortBase & in = receiver.GetInPort(inPortNumber);
		out.ConnectToIn(in);
	}
	
	void ConnectControls(
			Processing & sender, unsigned outControlNumber, 
			Processing & receiver, unsigned inControlNumber )
	{
		OutControl & out = sender.GetOutControl(outControlNumber);
		InControl & in = receiver.GetInControl(inControlNumber);
		out.AddLink(in);
	}

	void ConnectTypedControls(
			Processing & sender, unsigned typedOutControlNumber, 
			Processing & receiver, unsigned typedInControlNumber )
	{
		BaseTypedOutControl & out = sender.GetTypedOutControl(typedOutControlNumber);
		BaseTypedInControl & in = receiver.GetTypedInControl(typedInControlNumber);
		out.AddLink(in);
	}
	
	void ConnectPorts(
			Processing & sender, unsigned outPortNumber, 
			InPortBase & in )
	{
		OutPortBase & out = sender.GetOutPort(outPortNumber);
		out.ConnectToIn(in);
	}
	
	void ConnectPorts(
			OutPortBase & out,
			Processing & receiver, unsigned inPortNumber )
	{
		InPortBase & in = receiver.GetInPort(inPortNumber);
		out.ConnectToIn(in);
	}
	
	void ConnectPorts(
			Processing & sender, std::string outPortName, 
			InPortBase & in )
	{
		OutPortBase & out = sender.GetOutPort(outPortName);
		out.ConnectToIn(in);
	}
	
	void ConnectPorts(
			OutPortBase & out,
			Processing & receiver, std::string inPortName )
	{
		InPortBase & in = receiver.GetInPort(inPortName);
		out.ConnectToIn(in);
	}
	
	void SendFloatToInControl(Processing & receiver, const std::string & inControlName, float value){
		OutControl controlSender("tmpOutControl");
		controlSender.AddLink(receiver.GetInControl(inControlName));
		controlSender.SendControl(value);
	}

	void SendFloatToInControl(Processing & receiver, int inControlIndex, float value){
		OutControl controlSender("tmpOutControl");
		controlSender.AddLink(receiver.GetInControl(inControlIndex));
		controlSender.SendControl(value);
	}
	
	void SendFloatToOutControl(Processing & sender, const std::string & inControlName, float value){
		OutControl& out = *(dynamic_cast<OutControl*>(&(sender.GetOutControl(inControlName))));
		out.SendControl(value);
	}
	
	void SendFloatToOutControl(Processing & sender, int inControlIndex, float value){
		OutControl& out = *(dynamic_cast<OutControl*>(&(sender.GetOutControl(inControlIndex))));
		out.SendControl(value);
	}
	
	float GetFloatFromInControl(Processing & proc, const std::string & inControlName){
		InControl& in = *(dynamic_cast<InControl*>(&(proc.GetInControl(inControlName))));
		return in.GetLastValue();
	}
	
	float GetFloatFromInControl(Processing & proc, int inControlIndex){
		InControl& in = *(dynamic_cast<InControl*>(&(proc.GetInControl(inControlIndex))));
		return in.GetLastValue();
	}
	
	Processing::Processing() 
		: mpParent(0)
		, _network(0)
		, _execState(Unconfigured)
	{
	}

	bool Processing::Configure(const ProcessingConfig &c)
	{
		CLAM_ASSERT(!IsRunning(), "Configuring an already running Processing.");
		_configErrorMessage = "";
//		if (!mpParent) //TODO remove
//			TopLevelProcessing::GetInstance().Insert(*this);
		_execState = Unconfigured;
		try
		{
			if (!ConcreteConfigure(c)) 
			{
				if (_configErrorMessage=="")
					_configErrorMessage = "Configuration failed.";
				return false;
			}
		}
		catch( ErrProcessingObj& error ) ///TODO we should use here an ErrConfiguring class. PA
		{
			_configErrorMessage += "Exception thrown during ConcreteConfigure:\n";
			_configErrorMessage += error.what();
			_configErrorMessage += "\n";
			_configErrorMessage += "Configuration failed.";
			return false;
		}
		_execState = Ready;
		_configErrorMessage="Ready to be started";
		return true;
	}

	Processing::~Processing()
	{
		if ( mpParent )
			mpParent->Remove(*this);
	}

	void Processing::Start(void) 
	{
		CLAM_ASSERT(!IsRunning(), "Starting an already started processing");
		CLAM_ASSERT(IsConfigured(), "Starting an unconfigured processing");
		try {
			if (ConcreteStart())
				_execState = Running;
		}
		catch (ErrProcessingObj &e) {
			_configErrorMessage += "Exception thrown while starting.\n";
			_configErrorMessage += e.what();
		}
	}
	
	void Processing::Stop(void)
	{
		CLAM_ASSERT(IsRunning(), "Stop(): Object not running." );
		try {
			if(ConcreteStop())
				_execState = Ready; 
		}
		catch (ErrProcessingObj &e) {
			_configErrorMessage += "Exception thrown while stoping.\n";
			_configErrorMessage += e.what();
		}
	}
	unsigned Processing::BackendBufferSize()
	{
		return _network? _network->BackendBufferSize() : 1024;
	}
	unsigned Processing::BackendSampleRate()
	{
		return _network? _network->BackendBufferSize() : 44100;
	}

	void Processing::RegisterOutPort(OutPortBase* out) 
	{
		mOutPortRegistry.ProcessingInterface_Register(out);
	}
	void Processing::RegisterInPort(InPortBase* in)
	{
		mInPortRegistry.ProcessingInterface_Register(in);
	}

	void Processing::RegisterOutControl(OutControl* out) 
	{
		mOutControlRegistry.ProcessingInterface_Register(out);
	}
	void Processing::RegisterInControl(InControl* in)
	{
		mInControlRegistry.ProcessingInterface_Register(in);
	}
	void Processing::RegisterTypedOutControl(BaseTypedOutControl* out) 
	{
		mTypedOutControlRegistry.ProcessingInterface_Register(out);
	}
	void Processing::RegisterTypedInControl(BaseTypedInControl* in)
	{
		mTypedInControlRegistry.ProcessingInterface_Register(in);
	}
	
	void Processing::SetParent(Processing * parent)
	{
		ProcessingComposite * composite;
		if (!parent)
			composite = &(TopLevelProcessing::GetInstance());
		else
			composite = dynamic_cast<ProcessingComposite*>(parent);
		CLAM_ASSERT(composite, "Setting a non ProcessingComposite as Parent");

		if (mpParent==composite)
			return;

		if (mpParent)
			mpParent->Remove(*this);
		mpParent=composite;
		mpParent->Insert(*this);
	}

	void Processing::SetNetworkBackLink(Network * network)
	{
		_network=network;
	}
	void Processing::AddConfigErrorMessage( const std::string& msg )
	{
		_configErrorMessage += msg;
	}
	
	bool Processing::CanConsumeAndProduce()
	{
		if(!IsRunning())
		{
			std::cerr << "Cannot execute '" << GetClassName() << "' because not Running!" << std::endl;
			return false;
		}
//		std::cerr<< "inports ready? " << GetInPorts().AreReadyForReading() << std::endl;
//		std::cerr << "outports ready? " << GetOutPorts().AreReadyForWriting() << std::endl;
		return GetInPorts().AreReadyForReading() && GetOutPorts().AreReadyForWriting();
	}
	const ProcessingConfig& Processing::GetConfig() const
	{
		static NullProcessingConfig nullConfig;
		return nullConfig;
	}
	std::string Processing::GetExecStateString() const 
	{
		switch (_execState)
		{
			case Unconfigured:
				return "Unconfigured";
			case Ready:
				return "Ready";
			case Running:
				return "Running";
		}
		CLAM_ASSERT(false, "Unknown processing exec state found");
		return "INTERNAL ERROR";
	}
		


};//namespace CLAM

