#ifndef __TOKENDELAYVIEW__
#define __TOKENDELAYVIEW__

#include "GView.hxx"
#include "ViewConfiguration.hxx"
#include "Component.hxx"
#include "Enum.hxx"
#include "InControlView.hxx"
#include "CBL.hxx"
#include "TokenDelay.hxx"
#include "Spectrum.hxx"

using CLAM::Enum;
using CLAM::Component;

typedef CLAM::TokenDelay<CLAM::Spectrum> SpectralDelay;

namespace CLAMGUI
{

class TokenDelayView : public View
{
	typedef CBL::Functor1< unsigned >    PublishUCb;
public:

	class Services
		: public Enum
	{
	public:
		static tEnumValue sEnumValues[];
		static tValue     sDefault;

			
		Services()
			: Enum( sEnumValues, sDefault )
		{
		}
		
		Services( tValue v )
			: Enum( sEnumValues, v )
		{
		}
		
		Services( string s )
			: Enum( sEnumValues, s )
		{
		}
			
		~Services()
		{
		}


		Component* Species() const
		{
			return new Services;
		}

		typedef enum
		{
			GetDelay = 0,
			GetMaxDelay,
			SetDelay,
			SetMaxDelay
		} tEnum;
	};
public:
	
	TokenDelayView()
		: View(), mDelayGetter( NULL ), mMaxDelayGetter(NULL)
	{
	}

	virtual ~TokenDelayView()
	{
	}

	virtual const char* GetClassName()
	{
		return "TokenDelayView";
	}

	
	virtual void UpdateModel();

	virtual void NotifyUpdate()
	{
		if ( HasServicesLocked() && HasModelLocked() )
			{
				mDelayCtlView.NotifyUpdate();
			}
	}

	virtual void Refresh();

	virtual void Release();

	virtual void Configure( Aspect& cfg );

	virtual void BindTo( SpectralDelay* obj );

protected:	

private:

	SpectralDelay*                            mObservedObj;
	PublishUCb*                               mDelayGetter;
	PublishUCb*                               mMaxDelayGetter;
	InControlView                             mDelayCtlView;
	


};

}

#endif // TokenDelayView.hxx
