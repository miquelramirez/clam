#ifndef ANNOTATOR_ENTER_HXX
#define ANNOTATOR_ENTER_HXX

#include "AnnotatorEnterBase.h"

class AnnotatorDataFacade;

class AnnotatorEnter : public AnnotatorEnterBase
{
	public:
		AnnotatorEnter( QWidget *  parent = 0, const char * name = 0, WFlags f = 0);
		void handCursor();
		void arrowCursor();
		void clicked(QIconViewItem * item);
		void show();
		~AnnotatorEnter();
	private:
		AnnotatorDataFacade * mData;
};

#endif

