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
		mInControls(this),
		mOutControls(this),
		mOutPorts(this),
		mInPorts(this)
	{
		mState = Unconfigured;
	}

	bool Processing::Configure(const ProcessingConfig &c) throw(ErrProcessingObj)
	{
		std::string config_name;
		std::string old_name = mName;
		mStatus = "";

		if (mState == Running ||
			mState == Disabled)
			throw(ErrProcessingObj("Processing::Configure(): Object is running.",this));

		// As we have no acces to the actual dynamic configuration object
		// but via its abstract interface, we have no way to do apriori an
		// ExistAttr check, so we have to catch the possible exceptions.
		if (c.HasName())
			config_name = c.GetName();

		bool name_change_requested = config_name != ""     && 
		                             config_name != mName;
		if (name_change_requested)
			mName = config_name;


		if (!mpParent) {
			mpParent =  &(TopLevelProcessing::GetInstance());

			if (mName == "")
				mName = mpParent->InsertAndGiveName(*this);
			else
				mpParent->Insert(*this);
		}
		else if (name_change_requested)
			try {
				mpParent->NameChanged(*this,old_name);
			}
			catch(ErrProcessingObj& e) {
				// Invalid name.
				mName = old_name;
				mStatus ="Configuration failed: \n";
				mStatus+=e.what();
				throw(e);
			}


		try {
			if (ConcreteConfigure(c)) {
				mState=Ready;
				mStatus="";
				return true;
			}
			mState=Unconfigured;
			mStatus+=" Configuration failed.";
			return false;
		}
		catch(std::bad_cast) {
			mState = Unconfigured;
			mName = old_name;
			mStatus="Configuration failed: wrong argument type.";
			throw(ErrProcessingObj("ProcesingObject::Configure(): wrong argument type.",this));
		}
	}

	bool Processing::ConfigureOrphan(const ProcessingConfig &c) throw(ErrProcessingObj)
	{
		std::string config_name;
		std::string old_name = mName;

		if (mState == Running ||
			mState == Disabled)
			throw(ErrProcessingObj("Processing::Configure(): Object is running.",this));

		// As we have no acces to the actual dynamic configuration object
		// but via its abstract interface, we have no way to do apriori an
		// ExistAttr check, so we have to catch the possible exceptions.
		try {
			config_name = c.GetName();
		}
		catch (ErrDynamicType)
		{
			throw(ErrProcessingObj("Processing::Configure(): No name in config object.",this));
		}

		bool name_change_requested = config_name != ""     && 
		                             config_name != mName;
		if (name_change_requested)
			mName = config_name;

		try {
			if (ConcreteConfigure(c))
				mState=Ready;
			else
				mState=Unconfigured;
			
			return true;
		}
		catch(std::bad_cast) {
			mName = old_name;
			throw(ErrProcessingObj("ProcesingObject::Configure(): wrong argument type.",this));
		}
	}


	Processing::~Processing()
	{
		if ( mpParent )
			mpParent->Remove(*this);
		
	}

	void Processing::Start(void)
	{
		CLAM_ASSERT(mState==Ready,"Start(): Object not ready");
		mState=Running;
		
		try {
			if (!ConcreteStart())
				mState=Unconfigured;
		}
		catch (Err &e) {
			ErrProcessingObj new_e("Start(): Object failed to start properly.",this);
			mState=Unconfigured;
			new_e.Embed(e);
			CLAM_ASSERT( false, new_e.what() );
		}
	}
	
	void Processing::Stop(void)
	{
		CLAM_ASSERT( mState==Running ||	mState==Disabled, "Stop(): Object not running." );

		mState=Ready;
		ConcreteStop();
	}

	void Processing::PublishOutPort(OutPort* out) 
	{
		mPublishedOutPorts.push_back(out);
	}
	void Processing::PublishInPort(InPort* in)
	{
		mPublishedInPorts.push_back(in);
	}

	void Processing::PublishOutControl(OutControl* out) 
	{
		mPublishedOutControls.push_back(out);
	}
	void Processing::PublishInControl(InControl* in)
	{
		mPublishedInControls.push_back(in);
	}
	void Processing::LinkOutWithInControl(unsigned outId, Processing* inProc, unsigned inId) const
	{ //.at(unsigned) can throw an "out_of_range" exception.
#ifdef HAVE_STANDARD_VECTOR_AT
		mPublishedOutControls.at(outId)->AddLink(inProc->GetInControl(inId));
#else
		mPublishedOutControls[outId]->AddLink(inProc->GetInControl(inId));
#endif
	}
	int Processing::DoControl(unsigned id, TControlData val) const
	{//.at(unsigned) can throw an "out_of_range" exception.	
#ifdef HAVE_STANDARD_VECTOR_AT
		return mPublishedInControls.at(id)->DoControl(val);
#else
		return mPublishedInControls[id]->DoControl(val);
#endif
	}
	int Processing::SendControl(unsigned id, TControlData val) const
	{//.at(unsigned) can throw an "out_of_range" exception.
#ifdef HAVE_STANDARD_VECTOR_AT
		return mPublishedOutControls.at(id)->SendControl(val);
#else
		return mPublishedOutControls[id]->SendControl(val);
#endif
	}

	InControl* Processing::GetInControl(unsigned inId) const
	{//.at(unsigned) can throw an "out_of_range" exception.
#ifdef HAVE_STANDARD_VECTOR_AT
		return mPublishedInControls.at(inId);
#else
		return mPublishedInControls[inId];
#endif
	}

	OutControl* Processing::GetOutControl(unsigned inId) const
	{//.at(unsigned) can throw an "out_of_range" exception.
#ifdef HAVE_STANDARD_VECTOR_AT
		return mPublishedOutControls.at(inId);
#else
		return mPublishedOutControls[inId];
#endif
	}


	std::string Processing::GetFullName() const 
	{
		if (mpParent && mpParent != this)
			return mpParent->GetFullName()+"."+mName;
		else
			return mName;
	}

	void Processing::SetParent(Processing *o)
	{
		ProcessingComposite *p;

		if (o==0)
			p = &(TopLevelProcessing::GetInstance());
		else
			p = dynamic_cast<ProcessingComposite*>(o);

		if (!p)
			throw(ErrProcessingObj("Processing::SetParent():"
			                       "Inconsistent non-composite parent.",this));

		if (mpParent==p)
			return;

		if (mpParent)
			mpParent->Remove(*this);

		mpParent=p;

		if (GetConfig().GetName() == "")
			mName = mpParent->InsertAndGiveName(*this);
		else
			mpParent->Insert(*this);
	}

	void Processing::SetOrphan()
	{
		if (mpParent==0)
			return;

		if (mpParent)
			mpParent->Remove(*this);

		mpParent=0;

		if (GetConfig().GetName() == "")
			mName = "";
	}

};//namespace CLAM
