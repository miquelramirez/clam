#include "SnapshotGenerator.hxx"
#include "ArrayPresentation.hxx"

namespace CLAMGUI
{

void showSnapshotArray( const CLAM::DataArray& array )
{
	WidgetTKWrapper* toolkit = WidgetTKWrapper::GetWrapperFor( "FLTK" );
	
	ArrayPresentation pres( "Snapshot" );

	pres.UpdateData( array );
	pres.Show();

	do 
		{
			pres.Do();
			toolkit->Tick();
		} while ( !toolkit->IsClosing() );
}

void showSnapshotArray( CLAM::DataArray& array )
{
	WidgetTKWrapper* toolkit = WidgetTKWrapper::GetWrapperFor( "FLTK" );
	
	ArrayPresentation pres( "Snapshot" );

	pres.UpdateData( array );
	pres.Show();

	do 
		{
			pres.Do();
			toolkit->Tick();
		} while ( !toolkit->IsClosing() );

}

}
