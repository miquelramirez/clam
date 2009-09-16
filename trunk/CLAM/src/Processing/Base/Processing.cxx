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
		OutControlBase & out = sender.GetOutControl(outControlName);
		InControlBase & in = receiver.GetInControl(inControlName);
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
		OutControlBase & out = sender.GetOutControl(outControlNumber);
		InControlBase & in = receiver.GetInControl(inControlNumber);
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
		InControlBase & in = receiver.GetInControl(inControlName);
		FloatOutControl controlSender("tmpOutControl");
		CLAM_ASSERT(controlSender.IsLinkable(in), "GetFloatFromInControl: the control was not a Float control");
		controlSender.AddLink(in);
		controlSender.SendControl(value);
	}

	void SendFloatToInControl(Processing & receiver, int inControlIndex, float value){
		InControlBase & in = receiver.GetInControl(inControlIndex);
		FloatOutControl controlSender("tmpOutControl");
		CLAM_ASSERT(controlSender.IsLinkable(in), "GetFloatFromInControl: the control was not a Float control");
		controlSender.AddLink(in);
		controlSender.SendControl(value);
	}
	
	void SendFloatToOutControl(Processing & sender, const std::string & inControlName, float value){
		FloatOutControl * out = dynamic_cast<FloatOutControl*>(&(sender.GetOutControl(inControlName)));
		CLAM_ASSERT(out, "SendFloatToOutControl: the control was not a Float control");
		out->SendControl(value);
	}
	
	void SendFloatToOutControl(Processing & sender, int outControlIndex, float value){
		FloatOutControl * out = dynamic_cast<FloatOutControl*>(&(sender.GetOutControl(outControlIndex)));
		CLAM_ASSERT(out, "SendFloatToOutControl: the control was not a Float control");
		out->SendControl(value);
	}
	
	float GetFloatFromInControl(Processing & proc, const std::string & inControlName){
		FloatInControl * in = dynamic_cast<FloatInControl*>(&(proc.GetInControl(inControlName)));
		CLAM_ASSERT(in, "GetFloatFromInControl: the control was not a Float control");
		return in->GetLastValue();
	}
	
	float GetFloatFromInControl(Processing & proc, int inControlIndex){
		FloatInControl * in = dynamic_cast<FloatInControl*>(&(proc.GetInControl(inControlIndex)));
		CLAM_ASSERT(in, "GetFloatFromInControl: the control was not a Float control");
		return in->GetLastValue();
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
		if (_network)
			return _network->BackendBufferSize();
		//TODO: 1- inherit the buffer size on embeded processings without linked network
		//TODO: 2- resolve the multiple configuration instances, that makes the first ConcreteConfigure (on XML loading) print this message
		//std::cout<<"Warning: no linked network, using hardcoded backend buffer size (1024) on processing "<<GetClassName()<<std::endl;
		return 1024;
	}
	unsigned Processing::BackendSampleRate()
	{
		return _network? _network->BackendSampleRate() : 44100;
	}

	void Processing::RegisterOutPort(OutPortBase* out) 
	{
		mOutPortRegistry.ProcessingInterface_Register(out);
	}
	void Processing::RegisterInPort(InPortBase* in)
	{
		mInPortRegistry.ProcessingInterface_Register(in);
	}

	void Processing::RegisterOutControl(OutControlBase* out) 
	{
		mOutControlRegistry.ProcessingInterface_Register(out);
	}
	void Processing::RegisterInControl(InControlBase* in)
	{
		mInControlRegistry.ProcessingInterface_Register(in);
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
	bool Processing::AddConfigErrorMessage( const std::string& msg )
	{
		_configErrorMessage += msg;
		// For convenience, so you can report and exit in one line from ConcreteConfigure
		// return AddConfigErrorMessage("My error");
		return false;
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
	void Processing::ConsumeAndProduce()
	{
		// TODO: Not yet implemented
		/*
		for (unsigned i=0; i<GetNOutPorts(); i++)
			GetOutPort(i).Produce();
		for (unsigned i=0; i<GetNInPorts(); i++)
			GetInPort(i).Consume();
		*/
		
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

