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

#include <cstring>
#include <string>


namespace CLAM {

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
		OutControl & out = sender.GetOutControls().Get(outControlName);
		InControl & in = receiver.GetInControls().Get(inControlName);
		out.AddLink(in);
	}
	void ConnectPorts(
			Processing & sender, unsigned outPortNumber, 
			Processing & receiver, unsigned inPortNumber )
	{
		OutPortBase & out = sender.GetOutPorts().GetByNumber(outPortNumber);
		InPortBase & in = receiver.GetInPorts().GetByNumber(inPortNumber);
		out.ConnectToIn(in);
	}
	
	void ConnectControls(
			Processing & sender, unsigned outControlNumber, 
			Processing & receiver, unsigned inControlNumber )
	{
		OutControl & out = sender.GetOutControls().GetByNumber(outControlNumber);
		InControl & in = receiver.GetInControls().GetByNumber(inControlNumber);
		out.AddLink(in);
	}
	
	void ConnectPorts(
			Processing & sender, unsigned outPortNumber, 
			InPortBase & in )
	{
		OutPortBase & out = sender.GetOutPorts().GetByNumber(outPortNumber);
		out.ConnectToIn(in);
	}
	
	void ConnectPorts(
			OutPortBase & out,
			Processing & receiver, unsigned inPortNumber )
	{
		InPortBase & in = receiver.GetInPorts().GetByNumber(inPortNumber);
		out.ConnectToIn(in);
	}
	
	void ConnectPorts(
			Processing & sender, std::string outPortName, 
			InPortBase & in )
	{
		OutPortBase & out = sender.GetOutPorts().Get(outPortName);
		out.ConnectToIn(in);
	}
	
	void ConnectPorts(
			OutPortBase & out,
			Processing & receiver, std::string inPortName )
	{
		InPortBase & in = receiver.GetInPorts().Get(inPortName);
		out.ConnectToIn(in);
	}
	
	Processing::Processing() :
		mpParent(0),
		mExecState(Unconfigured)
	{
	}

	bool Processing::Configure(const ProcessingConfig &c)
	{
		CLAM_ASSERT(mExecState != Running, "Configuring an already running Processing.");
		mConfigErrorMessage = "";
		if (!mpParent) 
			TopLevelProcessing::GetInstance().Insert(*this);
		mExecState = Unconfigured;
		try
		{
			if (!ConcreteConfigure(c)) 
			{
				if (mConfigErrorMessage=="")
					mConfigErrorMessage = "Configuration failed.";
				return false;
			}
		}
		catch( ErrProcessingObj& error ) ///TODO we should use here an ErrConfiguring class. PA
		{
			mConfigErrorMessage += "Exception thrown during ConcreteConfigure:\n";
			mConfigErrorMessage += error.what();
			mConfigErrorMessage += "\n";
			mConfigErrorMessage += "Configuration failed.";
			return false;
		}
		mExecState = Ready;
		mConfigErrorMessage="Ready to be started";
		return true;
	}

	Processing::~Processing()
	{
		if ( mpParent )
			mpParent->Remove(*this);
	}

	void Processing::Start(void) 
	{
		AddConfigErrorMessage( GetClassName() );
		AddConfigErrorMessage( "::Start() Object not ready" );
		CLAM_ASSERT( mExecState==Ready, GetConfigErrorMessage().c_str() );
		try {
			if (ConcreteStart())
				mExecState = Running;
		}
		catch (ErrProcessingObj &e) {
			mConfigErrorMessage += "Start(): Object failed to start properly.\n";
			mConfigErrorMessage += e.what();
		}
	}
	
	void Processing::Stop(void)
	{
		CLAM_ASSERT( mExecState==Running, "Stop(): Object not running." );
		try {
			if(ConcreteStop())
				mExecState = Ready; 
		}
		catch (ErrProcessingObj &e) {
			mConfigErrorMessage += "Stop(): Object failed to stop properly.\n";
			mConfigErrorMessage += e.what();
		}
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
	
	void Processing::SetParent(Processing *o)
	{
		ProcessingComposite *p;

		if (o==0)
			p = &(TopLevelProcessing::GetInstance());
		else
			p = dynamic_cast<ProcessingComposite*>(o);

		CLAM_ASSERT(p, "Setting a non ProcessingComposite as Parent");

		if (mpParent==p)
			return;

		if (mpParent)
			mpParent->Remove(*this);

		mpParent=p;

		mpParent->Insert(*this);
	}

	void Processing::AddConfigErrorMessage( const std::string& msg )
	{
		mConfigErrorMessage += msg;
	}
	
	bool Processing::CanConsumeAndProduce()
	{	
		if(GetExecState()!=Running)
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
		switch (mExecState)
		{
			case Unconfigured:
				return "Unconfigured";
			case Ready:
				return "Ready";
			case Running:
				return "Running";
			default:
				return "Unknown state";
		}
	}
		


};//namespace CLAM

