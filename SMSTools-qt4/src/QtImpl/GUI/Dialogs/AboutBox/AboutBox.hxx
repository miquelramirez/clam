#ifndef __ABOUTBOX__
#define __ABOUTBOX__

#include "AboutBoxBase.h"

namespace QtSMS
{
	class AboutBox : public AboutBoxBase
	{
	public:
		AboutBox(QWidget* parent=0);
		~AboutBox();
	};
}

#endif

