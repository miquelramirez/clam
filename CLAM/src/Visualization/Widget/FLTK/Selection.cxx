/*****************************************************************************h
Fl_Envelope Widgets

Filename:   Selection.cxx
Programmer: Maarten de Boer
            mdeboer@iua.upf.es / maarten@things.nl
Date:       20 Jun 1999

This is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.
******************************************************************************/
#include "Selection.H"

void Selection::Remove(int from,int to)
{
	SelectionRange *r,*prev=0;
	r=mFirst;

	while (r) {
		if (from<r->mTo) {
			if (from>r->mFrom) {
				if (to<r->mTo) {
					SelectionRange *a=new SelectionRange(from,r->mTo);
					r->mTo=from;
					a->mNext=r->mNext;
					r->mNext=a;
					prev=r;
					r=a;
				}else{
					r->mTo=from;
					prev=r;
					r=r->mNext;				
				}
			}
			if (r) {
				SelectionRange *a=r;
				while (r && to>=r->mTo) {
					r=r->mNext;				
				}
				while (a!=r) {
					SelectionRange* n=a->mNext;
					delete a;
					a=n;							
				}
				if (prev) 
					prev->mNext=r;
				else
					mFirst=r;

				if (r && to>r->mFrom) r->mFrom=to;

			}
			return;
		}
		prev=r;
		r=r->mNext;
	}	
}

void Selection::Add(int from,int to) {
	SelectionRange *r,*prev=0;

	if (mFirst==0){
		 mFirst=new SelectionRange(from,to);
		 return;
	}
	r=mFirst;

	if (from>=r->mFrom) {
		while (r && from>=r->mFrom) {
			prev=r;
			r=r->mNext;
		}
		r=prev;
		SelectionRange* a=r;
		if (from>r->mTo) {
			prev=0;
			r=r->mNext;
			while (r && to>=r->mFrom) {
				prev=r;
				r=r->mNext;
			}				
			r=prev;
			if (r==0) {
				r=new SelectionRange(from,to);
				r->mNext=a->mNext;
				a->mNext=r;
				return;
			}
			if (r->mTo>to) to=r->mTo;
			r->mFrom=from;
			r->mTo=to;
			return;
		}else{
			prev=0;
			r=r->mNext;
			while (r && to>=r->mFrom) {
				prev=r;
				r=r->mNext;
			}				
			r=prev;
			if (r==0) {
				if (to>a->mTo) a->mTo=to;
				return;
			}
			if (r->mTo>to) to=r->mTo;
			SelectionRange* d=a->mNext;
			a->mNext=r->mNext;
			a->mTo=to;
			while (d && d!=r->mNext) {
				SelectionRange* n=d->mNext;
				delete d;
				d=n;
			}
			return;
		}
	}else{
		SelectionRange* a=r;
		while (r && to>=r->mFrom) {
			prev=r;
			r=r->mNext;
		}
		r=prev;
		if (!r) {
			r=new SelectionRange(from,to);
			r->mNext=mFirst;
			mFirst=r;
			return;
		}
		if (r->mTo>to) {
			to=r->mTo;
		}
		mFirst->mFrom=from;
		mFirst->mTo=to;
		SelectionRange* b=mFirst->mNext;
		mFirst->mNext=r->mNext;
		while (b && b!=r->mNext) {
			SelectionRange* n=b->mNext;
			delete b;
			b=n;
		}
	}
}
