#include "SystemPlots.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include "Assert.hxx"
#include <sstream>

namespace CLAMVM
{
	SystemPlots::PlotTable SystemPlots::sActivePlots;
	SystemPlots::WindowTable SystemPlots::sActiveWindows;
	SystemPlots::PositionTable SystemPlots::sOldWidgetPositions;
	bool SystemPlots::sDisplayListStarted = false;
	SystemPlots::DisplayList SystemPlots::sCurrentDisplayList;
	unsigned SystemPlots::sCurrentIndex = 0;

	bool SystemPlots::IsNameInUse( const std::string& str )
	{
		if ( sActivePlots.find( str ) == sActivePlots.end() )
			return false;
		
		return true;
	}

	std::string SystemPlots::GenerateNewUniqueName( )
	{
		std::stringstream sstr;
		sstr << sCurrentIndex;
		sCurrentIndex++;

		std::string newName = "unnamed_plot_"; 

		return newName + sstr.str();
	}

	void SystemPlots::RegisterWidget( const std::string& key, Fl_Widget* value  )
	{
		sActivePlots[ key ] = value;
	}

	void SystemPlots::UnregisterWidget( const std::string& key )
	{
		CLAM_WARNING( sActivePlots.erase( key ),
			      "SystemPlots: Unexisting key was unregistered!" );
	}

	void SystemPlots::SetFLTKLookAndFeel()
	{
		Fl::get_system_colors();
		Fl::set_boxtype(FL_UP_BOX,FL_THIN_UP_BOX);
		Fl::set_boxtype(FL_DOWN_BOX,FL_THIN_DOWN_BOX);
		Fl::set_boxtype(FL_UP_FRAME,FL_THIN_UP_FRAME);
		Fl::set_boxtype(FL_DOWN_FRAME,FL_THIN_DOWN_FRAME);
		
	}

	void SystemPlots::Display( const std::string& key )
	{
		CLAM_ASSERT( sActivePlots.find( key ) != sActivePlots.end(),
			     "Attempted to display an unexisting plot!" );

		if ( sDisplayListStarted )
		{
			AddToDisplayList( key );
			return;
		}

		Fl_Widget* widget = sActivePlots[ key ];

		sActiveWindows[ key ] = new Fl_Window( widget->x(), widget->y(), widget->w(), widget->h() );
		Position p = { widget->x(), widget->y() };
		sOldWidgetPositions[key]=p;
		widget->position( 0, 0 );
		sActiveWindows[ key ]->label( widget->label() );
		sActiveWindows[ key ]->add( widget );
		sActiveWindows[ key ]->resizable( widget );
		sActiveWindows[ key ]->end();
		sActiveWindows[ key ]->show();

		SetFLTKLookAndFeel();

		Fl::run();
		Fl::flush();
		
		widget->position( sOldWidgetPositions[key].x, sOldWidgetPositions[key].y );

		CleanupActiveWindows();
		sOldWidgetPositions.clear();
	}

	void SystemPlots::CleanupActiveWindows()
	{
		WindowTable::iterator i = sActiveWindows.begin();

		for( ; i != sActiveWindows.end(); i++ )
		{
			if ( i->first == "display_list_container" )
			{
				DisplayList::iterator j;

				for ( j = sCurrentDisplayList.begin();
				      j != sCurrentDisplayList.end();
				      j++ )
				{
					i->second->remove( sActivePlots[ *j ] );					
				}

			}
			else
			{
				i->second->remove( sActivePlots[ i->first ] );
			}

			delete i->second;
			i->second = NULL;
			sActivePlots[i->first]->position( 
					sOldWidgetPositions[i->first].x, sOldWidgetPositions[i->first].y );

		}

		sActiveWindows.clear();
	}

	void SystemPlots::AddToDisplayList( const std::string& key )
	{
		sCurrentDisplayList.push_back( key );
	}

	void SystemPlots::BeginDisplayList( )
	{
		CLAM_ASSERT( sDisplayListStarted == false,
			     "Attempted to build a nested display list!" );
		sDisplayListStarted = true;
		sCurrentDisplayList.clear();
	}

	void SystemPlots::EndDisplayList()
	{
		sDisplayListStarted = false;

		CreateWindowsInCascade();

		SetFLTKLookAndFeel();

		Fl::run();
		
		CleanupActiveWindows();
	}

	void SystemPlots::DisplayAll()
	{
		PlotTable::iterator i ;
		
		for ( i = sActivePlots.begin();
		      i!= sActivePlots.end(); i++ )
			sCurrentDisplayList.push_back( i->first );

		CreateWindowsInCascade();

		SetFLTKLookAndFeel();

		Fl::run();
		Fl::flush();

		CleanupActiveWindows();
		sCurrentDisplayList.clear();
	}

	void SystemPlots::CreateWindowsInCascade()
	{
		int x = 10;
		int y = 10;
		int dpx = 30;

		DisplayList::iterator i;

		for ( i = sCurrentDisplayList.begin();
		      i!=sCurrentDisplayList.end();
		      i++ )
		{
			Fl_Widget* widget = sActivePlots[ *i ];
			
			sActiveWindows[ *i ] = new Fl_Window( x, y, widget->w(), widget->h() );
			Position p = { widget->x(), widget->y() };
			sOldWidgetPositions[*i]= p;
			widget->position( 0, 0 );

			sActiveWindows[ *i ]->label( widget->label() );
			sActiveWindows[ *i ]->add( widget );
			sActiveWindows[ *i ]->resizable( widget );
			sActiveWindows[ *i ]->end();
	
			sActiveWindows[ *i ]->show();
			x+=dpx;
			y+=dpx;
		}


	}
}
