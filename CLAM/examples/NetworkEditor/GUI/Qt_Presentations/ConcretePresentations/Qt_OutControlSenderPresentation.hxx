
#ifndef __Qt_OutControlSenderPresentation_hxx__
#define __Qt_OutControlSenderPresentation_hxx__

#include "Qt_ProcessingPresentation.hxx"
#include "DataTypes.hxx"
#include "OutControlSender.hxx"

class QWidget;


namespace NetworkGUI
{

class Qt_OutControlSenderPresentation : public Qt_ProcessingPresentation
{
	Q_OBJECT

	QWidget * mControlRepresentation;
public:
	Qt_OutControlSenderPresentation();
	void UpdateSize( bool hasToResize = true );
protected:
	void ConfigurationUpdated( bool ok );
	void CreateControlRepresentationWidget( const CLAM::OutControlSenderConfig::EControlRepresentation & );
	void AdjustControlRepresentationValues();
	void ExecuteResize( const QPoint & difference );
	void paintEvent( QPaintEvent * );
	void mouseDoubleClickEvent ( QMouseEvent * ){} // to avoid editing name		

	CLAM::TControlData mMin;
	CLAM::TControlData mMax;
	CLAM::TControlData mStep;
	CLAM::TControlData mDefault;
public slots:
	void SlotValueChanged( int );
};
	
} // namespace NetworkGUI

#endif //__Qt_OutControlSenderPresentation_hxx__  
