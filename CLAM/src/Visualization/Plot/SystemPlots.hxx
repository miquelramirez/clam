#ifndef __SYSTEMPLOTS__
#define __SYSTEMPLOTS__

#include <string>
#include <map>
#include <list>

class Fl_Widget;
class Fl_Window;

namespace CLAMVM
{
	class Presentation;
	class PlotBase;

	class SystemPlots
	{
		struct Position 
		{
			int x;
			int y;
		};

		typedef std::map< std::string, Position >      PositionTable;
		typedef std::map< std::string, Fl_Widget* >    PlotTable;
		typedef std::map< std::string, Fl_Window* >    WindowTable;
		typedef std::list< std::string >               DisplayList;
		
		

	public:
		static bool        IsNameInUse( const std::string& str );
		static std::string GenerateNewUniqueName();
		static void        RegisterWidget( const std::string& key, Fl_Widget* p );
		static void        UnregisterWidget( const std::string& key  );
	public:
		static void        Display( const std::string& str );
		static void        DisplayAll();
		static void        BeginDisplayList();
		static void        EndDisplayList();

	private:

		static void        SetFLTKLookAndFeel();
		static void        CleanupActiveWindows();
		static void        AddToDisplayList( const std::string& key );
		static void        CreateWindowsInCascade();

	protected:
		
		static PlotTable   sActivePlots;
		static WindowTable sActiveWindows;
		static bool        sDisplayListStarted;
		static DisplayList sCurrentDisplayList;
		static unsigned    sCurrentIndex;
		static PositionTable sOldWidgetPositions;
	};
}


#endif // SystemPlots.hxx
