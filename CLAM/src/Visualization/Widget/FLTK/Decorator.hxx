#ifndef __DECORATOR__
#define __DECORATOR__

#include "GeometryKit.hxx"
#include <algorithm>

namespace CLAMGUI
{

template < typename FeatureContainer>
class Decorator
{
private:
  typedef typename FeatureContainer::iterator Iterator;

public:

  Decorator()
  {
  }

  Decorator( Iterator FeatureStart, Iterator FeatureEnd )
  {
    SetUp( FeatureStart, FeatureEnd );
  
  }

  virtual ~Decorator()
  {
  }
  
  template < typename Iterator >
  void SetUp( Iterator FeatureStart, Iterator FeatureEnd )
  {
    mSpecPoints.assign( FeatureStart, FeatureEnd );
    std::sort( mSpecPoints.begin(), mSpecPoints.end() );
  }

  void SetViewport( const Recti& r )
  {
    mViewport = r;
  }

  // Returns true if something has been done for that concrete x

  template < typename DecoringPolicy >
  bool operator()( int value, int x_value, DecoringPolicy depol )
  {
    if ( std::find( mSpecPoints.begin(), mSpecPoints.end(), value ) != mSpecPoints.end() )
      {
	depol( x_value, mViewport );

	return true;
      }
    else
      return false;
  }
  

private:

  FeatureContainer     mSpecPoints;
  Recti                mViewport;


};

}
#endif // Decorator.hxx
