#ifndef FocusPropertyFollower_hxx
#define FocusPropertyFollower_hxx

#include <QtGui/QLabel>

class FocusPropertyFollower : public QLabel
{
	Q_OBJECT
public:
	FocusPropertyFollower(QWidget * parent);
	virtual ~FocusPropertyFollower();
public slots:
	void updateMessageOnFocus();
};

#endif//FocusPropertyFollower_hxx

