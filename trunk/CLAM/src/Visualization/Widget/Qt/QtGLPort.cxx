#include "QtGLPort.hxx"
#include "Range.hxx"
#include "GLState.hxx"
#include <qtimer.h>

using namespace CLAMGUI;

QtGLPort::QtGLPort( QWidget* parent, const char* name )
	: QGLWidget( parent, name ), mIsConf( false ), mState( NULL )
{
	setMinimumSize(300,100);

	QTimer* time = new QTimer(this);

	connect( time, SIGNAL(timeout()), this, SLOT(updateGL()) );

	time->start( 166 );

}

QtGLPort::~QtGLPort()
{
	if ( mIsConf )
		delete mState;
}

void QtGLPort::Configure( GLState* state, const CBL::Functor0& drawcb )
{
	if ( mIsConf )
	{
		delete mState;
		mIsConf = false;
	}
	mDrawCb=drawcb;
	mState=state;
	mIsConf = true;
}


void QtGLPort::SetView( const Viewport& v )
{
	mView = v;

	emit ViewportReceived();
}

const Viewport& QtGLPort::GetView() const
{
	return mView;
}

QSize QtGLPort::minimumSize()
{
	return QSize( 100, 100 );
}

QSize QtGLPort::sizeHint()
{
	return QSize( 640, 480 );
}


void QtGLPort::ViewportChanged( const Viewport& v)
{
	mView = v;
	makeCurrent();
	resizeGL( width(), height() );
	glDraw();
}


void QtGLPort::initializeGL( )
{
	if ( mIsConf )
		mState->Apply();
}

void QtGLPort::resizeGL( int w, int h )
{
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
/*	glOrtho( mHorRange.mPosition, mHorRange.mPosition + mHorRange.mSize, 
			 mVerRange.mPosition , mVerRange.mPosition + mVerRange.mSize, 
			 -1.0f, 1.0f );*/
	glOrtho( mView.left, mView.right, mView.bottom, mView.top, -1.0f, 1.0f );
	glMatrixMode( GL_MODELVIEW );
}

void QtGLPort::paintGL( )
{
	if (mIsConf )
		mDrawCb();
}
