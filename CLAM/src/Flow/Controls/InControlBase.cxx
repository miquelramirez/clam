#include <CLAM/InControlBase.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM
{
	InControlBase::InControlBase(const std::string &name, Processing * proc)
		: mName(name)
		, mProcessing(proc)
		, mUpperBound(1)
		, mLowerBound(0)
		, mBounded(false)
		, mHasDefaultValue(false)
	{
		if (proc) proc->RegisterInControl(this);
	}
	
	InControlBase::~InControlBase()
	{
		while (!mLinks.empty())
			mLinks.front()->RemoveLink(*this);
		if (mProcessing)
			mProcessing->GetInControls().ProcessingInterface_Unregister(this);
	}
	bool InControlBase::IsConnectedTo(OutControlBase & out)
	{
		return out.IsConnectedTo(*this);
	}

	bool InControlBase::IsBounded() const
	{
		return mBounded;
	}
	float InControlBase::UpperBound() const
	{
		return mUpperBound;
	}
	float InControlBase::LowerBound() const
	{
		return mLowerBound;
	}
	void InControlBase::SetBounds( float lower, float upper )
	{
		mLowerBound = lower;
		mUpperBound = upper;
		mBounded = true;
	}
	void InControlBase::SetDefaultValue( float val )
	{
		mDefaultValue = val;
		mHasDefaultValue = true;
	}
	float InControlBase::DefaultValue() const
	{
		if (mHasDefaultValue) return mDefaultValue;
		return (mUpperBound+mLowerBound)/2.f;
	}
}
