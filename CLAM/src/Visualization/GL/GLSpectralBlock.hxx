#ifndef __GLSPECTRALBLOCK__
#define __GLSPECTRALBLOCK__

#include "ArrayRenderer.hxx"
#include "CLAMGL.hxx"

namespace CLAMGUI
{

class GLSpectralBlock
	: public ArrayRenderer
{

public:

	GLSpectralBlock( int divs );

	virtual ~GLSpectralBlock()
	{
	}

	void CacheData( const DataArray& );

	void DefineViewport( const DataArray&, Viewport& );

	void Draw();

protected:

	void DataTransform( const DataArray& );

	void XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
	{
	}

	void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
	{
	}

	void BuildDisplayList();

private:	
	
	int          mDivisions;
	DataArray    mBlockHeights;
	GLuint       mDLid;
	bool         mDLready;
};

}

#endif // GLSpectralBlock.hxx
