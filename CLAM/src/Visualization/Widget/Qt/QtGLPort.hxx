#ifndef __QTGLPORT__
#define __QTGLPORT__

#include <qgl.h>
#include "CBL.hxx"
#include "Range.hxx"
#include "Viewport.hxx"

namespace CLAMGUI
{
class GLState;

class Range;

class QtGLPort : public QGLWidget
{
	Q_OBJECT
public:

	QtGLPort( QWidget* parent, const char* name );

	virtual ~QtGLPort();

	void Configure( GLState* state, const CBL::Functor0& drawcb );

	void SetView( const Viewport& v );

	const Viewport& GetView() const;

	QSize minimumSize();

	QSize sizeHint();

public slots:

	void ViewportChanged( const Viewport& v );

signals:

	void ViewportReceived();
protected:

	void initializeGL();

	void resizeGL( int w, int h );

	void paintGL();

private:
	
	CBL::Functor0      mDrawCb;
    bool               mIsConf;
	GLState*           mState;
	Viewport           mView;
};

}

#endif // QtGLPort.hxx
