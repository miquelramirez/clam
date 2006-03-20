#ifndef EquidistantPointsFunction_hxx
#define EquidistantPointsFunction_hxx

#include <string>
#include <CLAM/DataTypes.hxx>

namespace CLAM
{
	class EquidistantPointsFunction
	{
	public:
		typedef std::string Units;
	public:
		EquidistantPointsFunction()
			: mBoundMin(false)
			, mBoundMax(false)
			, mMinValue(0)
			, mMaxValue(0)
		{
		}
		void setValueUnits(const Units & units)
		{
			mUnits = units;
		}
		void setDivision(CLAM::TData firstPosition, CLAM::TData intervalueGap, const Units & units)
		{
			mFirstPointPosition=firstPosition;
			mInterPointInterval=intervalueGap;
			mXUnits = units;
		}
		void setValues(const CLAM::TData * data, unsigned size, const Units & units)
		{
			mValues = data;
			mSize = size;
			mBpf.Resize(size);
			mBpf.SetSize(size);
			for(int i=0; i<size ;i++)
			{
				mBpf.SetXValue(i, mFirstPointPosition+i*mInterPointInterval);
				mBpf.SetValue(i,mValues[i]);
			}
		}
		void limitMaxValue(CLAM::TData max)
		{
			mBoundMax=true;
			mMaxValue=max;
		}
		void limitMinValue(CLAM::TData min)
		{
			mBoundMin=true;
			mMinValue=min;
		}
		CLAM::TData getMax()
		{
			if (mBoundMax) return mMaxValue;
			CLAM::TData max = 0;
			for (unsigned int i=0; i<mSize; i++)
			{
				if (mValues[i]<max) continue;
				max=mValues[i];
			}
			return max;
		}
		CLAM::TData getMin()
		{
			if (mBoundMin) return mMinValue;
			CLAM::TData min = 0;
			for (unsigned int i=0; i<mSize; i++)
			{
				if (mValues[i]>min) continue;
				min=mValues[i];
			}
			return min;
		}
		CLAM::BPF & GetBPF()
		{
			return mBpf;
		}

	private:
		const CLAM::TData * mValues; //< Array containing the values
		unsigned mSize; //< The number of values
		CLAM::BPF mBpf; //< Transcribed BPF

		// Cadidates to become a FrameDivision object
		CLAM::TData mFirstPointPosition;
		CLAM::TData mInterPointInterval;
		Units mXUnits;
		
		// Candidates to become a MagnitudeType object
		bool mBoundMin;
		bool mBoundMax;
		CLAM::TData mMinValue;
		CLAM::TData mMaxValue;
		CLAM::TData mDefaultValue;
		Units mUnits;
	};
}




#endif // EquidistantPointsFunction_hxx
