
#ifndef __ControlSurface_hxx__
#define __ControlSurface_hxx__

#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Enum.hxx>
#include <QtGui/QWidget>

namespace CLAM
{

class ControlSurfaceConfig : public ProcessingConfig
{	
public:
	DYNAMIC_TYPE_USING_INTERFACE (ControlSurfaceConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TControlData, MinX );
	DYN_ATTRIBUTE (1, public, TControlData, DefaultX );
	DYN_ATTRIBUTE (2, public, TControlData, MaxX );
	DYN_ATTRIBUTE (3, public, TControlData, MinY );
	DYN_ATTRIBUTE (4, public, TControlData, DefaultY );
	DYN_ATTRIBUTE (5, public, TControlData, MaxY );


protected:
	void DefaultInit(void);
};

class ControlSurface : public Processing
{
	ControlSurfaceConfig mConfig;
	FloatOutControl mOutputX;
	FloatOutControl mOutputY;
	TControlData mLastValueX;
	TControlData mLastValueY;
	bool mFirstDoAfterStart;
public:
	ControlSurface();
	ControlSurface( const ControlSurfaceConfig & );

	bool Do();
	const char * GetClassName() const {return "ControlSurface";}

	const ProcessingConfig &GetConfig() const { return mConfig;}
	void SendControl(TControlData valueX, TControlData valueY);
protected:
	bool ConcreteConfigure(const ProcessingConfig& c);
	bool ConcreteStart();
};

} //namespace CLAM

#endif //__ControlSurface_hxx__


