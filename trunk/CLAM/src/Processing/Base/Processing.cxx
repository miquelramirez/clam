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


#include "Processing.hxx"
#include "ProcessingComposite.hxx"
#include "TopLevelProcessing.hxx"
#include "ErrDynamicType.hxx"
#include "InPort.hxx"

#include "mtgsstream.h" // An alias for <sstream>
#include <cstring>
#include <string>


namespace CLAM {

	const Processing::iterator 
	Processing::null_iterator = Processing::iterator(0);

	Processing::Processing() 
		: mpParent(0),
		mPreconfigureExecuted( false )
	{
		mState = Unconfigured;
	}

	void Processing::PreConcreteConfigure( const ProcessingConfig& c )
	{
		CLAM_ASSERT(mState != Running, "Configuring an already running Processing.");
		CLAM_ASSERT(mState != Disabled, "Configuring a disabled Processing.");
		mStatus = "";

		if (!mpParent) 
			TopLevelProcessing::GetInstance().Insert(*this);
		mPreconfigureExecuted = true;

	}

	void Processing::PostConcreteConfigure()
	{
		CLAM_ASSERT(mState != Running, "Configuring an already running Processing.");
		CLAM_ASSERT(mState != Disabled, "Configuring a disabled Processing.");
		CLAM_ASSERT(mPreconfigureExecuted, "PreConcreteConfigure was not being called" );

		mState=Ready;
		mStatus="Ready to be started";

	}

	bool Processing::Configure(const ProcessingConfig &c)
	{
		PreConcreteConfigure( c );
		
		try
		{

			if (!ConcreteConfigure(c)) 
			{
				mState=Unconfigured;
				mPreconfigureExecuted = false;
				mStatus+=" Configuration failed.";
				mState = Unconfigured;
				return false;
			}
		}
		catch( CLAM::Err& error )
		{
			mState = Unconfigured;
			mPreconfigureExecuted = false;
			mStatus += "Exception thrown during ConcreteConfigure:\n";
			mStatus += error.what();
			mStatus += "\n";
			mStatus += "Configuration failed.";

			return false;
		}
		
		PostConcreteConfigure();
		
		return true;
	}

	void Processing::ConfigureOrphan(const ProcessingConfig &c)
	{

		CLAM_ASSERT(mState != Running, "Configuring an already running Processing.");
		CLAM_ASSERT(mState != Disabled, "Configuring a disabled Processing.");

		if (ConcreteConfigure(c))
			mState=Ready;
		else
			mState=Unconfigured;
	}


	Processing::~Processing()
	{
		if ( mpParent )
			mpParent->Remove(*this);
		
	}

	void Processing::Start(void) 
	{
		CLAM_ASSERT(mState==Ready,
			    ComposeAssertMessage(AddStatus( "Start(): Object not ready" )).c_str() );
		try {
			if (ConcreteStart())
				mState = Running;
		}
		catch (Err &e) {
			mStatus += "Start(): Object failed to start properly.\n";
			mStatus += e.what();
		}
	}
	
	void Processing::Stop(void)
	{
		CLAM_ASSERT( mState==Running ||	mState==Disabled, "Stop(): Object not running." );

		try {
			if(ConcreteStop())
				mState = Ready;
		}
		catch (Err &e) {
			mStatus += "Stop(): Object failed to stop properly.\n";
			mStatus += e.what();
		}
	}

	void Processing::PublishOutPort(OutPort* out) 
	{
		mPublishedOutPorts.Publish(out);
	}
	void Processing::PublishInPort(InPort* in)
	{
		mPublishedInPorts.Publish(in);
	}

	void Processing::PublishOutControl(OutControl* out) 
	{
		mPublishedOutControls.Publish(out);
	}
	void Processing::PublishInControl(InControl* in)
	{
		mPublishedInControls.Publish(in);
	}
	
//	void Processing::LinkOutWithInControl(unsigned outId, Processing* inProc, unsigned inId) const
//	{ //.at(unsigned) can throw an "out_of_range" exception.
//#ifdef HAVE_STANDARD_VECTOR_AT
//		mPublishedOutControls.at(outId)->AddLink(inProc->GetInControl(inId));
//#else
//		mPublishedOutControls[outId]->AddLink(inProc->GetInControl(inId));
//#endif
//	}
//	int Processing::DoControl(unsigned id, TControlData val) const
//	{//.at(unsigned) can throw an "out_of_range" exception.	
//#ifdef HAVE_STANDARD_VECTOR_AT
//		return mPublishedInControls.at(id)->DoControl(val);
//#else
//		return mPublishedInControls[id]->DoControl(val);
//#endif
//	}
//	int Processing::SendControl(unsigned id, TControlData val) const
//	{//.at(unsigned) can throw an "out_of_range" exception.
//#ifdef HAVE_STANDARD_VECTOR_AT
//		return mPublishedOutControls.at(id)->SendControl(val);
//#else
//		return mPublishedOutControls[id]->SendControl(val);
//#endif
//	}

//	InControl* Processing::GetInControl(unsigned inId) const
//	{//.at(unsigned) can throw an "out_of_range" exception.
//#ifdef HAVE_STANDARD_VECTOR_AT
//		return mPublishedInControls.at(inId);
//#else
//		return mPublishedInControls[inId];
//#endif
//	}

//	OutControl* Processing::GetOutControl(unsigned inId) const
//	{//.at(unsigned) can throw an "out_of_range" exception.
//#ifdef HAVE_STANDARD_VECTOR_AT
//		return mPublishedOutControls.at(inId);
//#else
//		return mPublishedOutControls[inId];
//#endif
//	}

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

	void Processing::SetOrphan()
	{
		if (mpParent==0)
			return;

		if (mpParent)
			mpParent->Remove(*this);

		mpParent=0;
	}

	std::string Processing::ComposeAssertMessage( std::string msg )
	{
		std::string assertMessage = GetClassName();
		assertMessage += "::";
		assertMessage += msg;

		return assertMessage;
	}

	const char* Processing::AddStatus(const std::string& a)
	{
		return AddStatus(a.c_str());
	}

	const char* Processing::AddStatus(const char* a)
	{
		static char ret[256];
		int len_a = strlen(a);
		int len_b = mStatus.length();
		char* truncated_str = "[truncated]...";
		int space_left = 255-strlen(truncated_str);
		bool truncated = false;
		if (len_a > space_left) {
			len_a = space_left;
			truncated = true;
		}
		space_left -= len_a; 
		strncpy(ret,a,len_a);
		if (len_b > space_left) {
			len_b = space_left;
			truncated = true;
		}
		strncpy(ret + len_a,mStatus.c_str(),len_b);
		if (truncated)
		{
			strcpy(ret + len_a + len_b,truncated_str);
		}
		return ret;
	}
};//namespace CLAM

