#ifndef __ARRAYPRESENTATION__
#define __ARRAYPRESENTATION__

#include "Array.hxx"
#include "FLDisplayContainer.hxx"
#include <FL/Fl_Double_Window.H>
#include "GLPortNew.hxx"
#include "GLArrayRenderer.hxx"
#include "GLState.hxx"
#include "CLAMGL.hxx"
#include "Viewport.hxx"

using CLAM::DataArray;
using CLAM::TData;

namespace CLAMGUI
{

class ArrayPresGLState
	: public GLState
{
public:
	void Apply()
	{
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glShadeModel( GL_FLAT );
	}
};


class ArrayPresentation
{
public:

	ArrayPresentation( const char* label = 0 )
		: mMainWidget( NULL )
	{

		Init ( Geometry( 100, 100, 800, 600 ), label );
	}

	ArrayPresentation( const Geometry& g, const char* label = 0 )
		: mMainWidget( NULL )
	{
		Init( g, label );
	}
	
	void UpdateData( const  DataArray& array );

	void Show();

	void Do()
	{
		mPort->redraw();
	}

	~ArrayPresentation()
	{
		if ( mMainWidget )
			delete mMainWidget;
		delete mRenderer;
	}

protected:
	
	void Init( const Geometry& g, const char* label );

private:

	GLPort*                   mPort;
	GLArrayRenderer*          mRenderer;
	FLDisplayContainer*       mContainer;
	Fl_Double_Window*         mMainWidget;

	Viewport                  mMetrix;
	

};

}

#endif // ArrayPresentation.hxx
