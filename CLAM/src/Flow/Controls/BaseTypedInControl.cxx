#include <CLAM/BaseTypedInControl.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM
{
	BaseTypedInControl::BaseTypedInControl(const std::string &name, Processing * proc, bool publish)
		: mName(name)
		, mProcessing(proc)
		, mUpperBound(1)
		, mLowerBound(0)
		, mBounded(false)
		, mHasDefaultValue(false)
	{
		if (proc && publish) proc->RegisterInControl(this);
	}
	
	BaseTypedInControl::~BaseTypedInControl()
	{
		while (!mLinks.empty())
			mLinks.front()->RemoveLink(*this);
		if (mProcessing)
			mProcessing->GetInControls().ProcessingInterface_Unregister(this);
	}
	bool BaseTypedInControl::IsConnectedTo(BaseTypedOutControl & out)
	{
		return out.IsConnectedTo(*this);
	}

	bool BaseTypedInControl::IsBounded() const
	{
		return mBounded;
	}
	float BaseTypedInControl::UpperBound() const
	{
		return mUpperBound;
	}
	float BaseTypedInControl::LowerBound() const
	{
		return mLowerBound;
	}
	void BaseTypedInControl::SetBounds( float lower, float upper )
	{
		mLowerBound = lower;
		mUpperBound = upper;
		mBounded = true;
	}
	void BaseTypedInControl::SetDefaultValue( float val )
	{
		mDefaultValue = val;
		mHasDefaultValue = true;
	}
	float BaseTypedInControl::DefaultValue() const
	{
		if (mHasDefaultValue) return mDefaultValue;
		return (mUpperBound+mLowerBound)/2.f;
	}
}
