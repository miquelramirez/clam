/*****************************************************************************h
Fl_Envelope Widgets

Filename:   Selection.H
Programmer: Maarten de Boer
            mdeboer@iua.upf.es / maarten@things.nl
Date:       20 Jun 1999

This is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.
******************************************************************************/
#ifndef __Selection__
#define __Selection__

class Selection {
private:
	class SelectionRange
	{
	friend class Selection;
	private:
		int mFrom;
		int mTo;
	public:
		SelectionRange(int from,int to){ mFrom=from; mTo=to; mNext=0;}
/*
untested:
		SelectionRange(const SelectionRange& copy) {
			mFrom = copy.mFrom;
			mTo = copy.mTo;
			mNext=0;
		}
*/
		~SelectionRange(){}
		SelectionRange* mNext;
	};
	SelectionRange* mFirst;
	SelectionRange* mCurrent;
	int mCurrentValue;
public:
	void Clear(void) {
		mCurrent=0;
		while (mFirst) {
			SelectionRange* next=mFirst->mNext;
			delete mFirst;
			mFirst=next;
		}
	}
	Selection(void) {
		mFirst = 0;
	}
/*
untested:
	Selection(const Selection& copy) {
		mFirst = 0;
		if (copy.mFirst==0) return;
		
		SelectionRange* r=mFirst=new SelectionRange(copy.First);
		SelectionRange* t=copy.First->mNext;
		
		while (t) {
			r->mNext=new SelectionRange(t);
			r=r->mNext;
			t=t->mNext;
		}
	}
*/
	void Add(int from,int to);
	void Add(int i){ Add(i,i+1); }

	void Remove(int from,int to);
	void Remove(int i){ Remove(i,i+1); }


	bool StartGet(void) {
		mCurrent=mFirst;
		if (mCurrent) mCurrentValue=mCurrent->mFrom;
		return mCurrent!=0;
	}
	
	bool DoneGet(void) {
		return mCurrent==0;
	}

	bool Selected(int i) {
		if (mFirst==0) return 0;
		if (mCurrent==0 || mCurrent->mFrom>i) mCurrent=mFirst;
		if (i<mCurrent->mFrom) return 0;
		while (mCurrent && i>=mCurrent->mFrom) {
			if (i>=mCurrent->mFrom && i<mCurrent->mTo) {
				return 1;
			}
			
			mCurrent=mCurrent->mNext;
		}
		return 0;
	}

	int GetFrom(void)
	{
		if (mCurrent==0) return -1;
		return mCurrent->mFrom;	
	}

	int GetTo(void)
	{
		if (mCurrent==0) return -1;
		int ret=mCurrent->mTo;	
		mCurrent=mCurrent->mNext;
		return ret;
	}
	
	int Get(void) {
		int ret=mCurrentValue;
		if (mCurrent==0) return -1;
		mCurrentValue++;
		if (mCurrentValue == mCurrent->mTo) {
			mCurrent = mCurrent->mNext;
			if (mCurrent) mCurrentValue=mCurrent->mFrom;
		}
		return ret;
	}

	void Print(void) {
		SelectionRange* r=mFirst;
//		printf("\n");
		while (r) {
//			printf("%d %d\n",r->mFrom,r->mTo);
			r=r->mNext;
		}
//		printf("\n");
	}
};

#endif
