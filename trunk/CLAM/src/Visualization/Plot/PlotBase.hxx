#ifndef __PLOTBASE__
#define __PLOTBASE__

#include <string>

namespace CLAMVM
{
	class PlotBase
	{

	public:
		virtual ~PlotBase();

		void SetName( std::string name );
		const std::string& GetName() const;

		virtual void SetPosition( int x, int y ) = 0;
		virtual void SetSize( int w, int h ) = 0;
		virtual void SetLabel( const char* label ) = 0;
		virtual void SetYRange( double ymin, double ymax ) = 0;
		virtual void SetTooltipFormat( const char* tooltipFmt ) = 0;				
	
	private:
		std::string mName;

	};

	inline const std::string& PlotBase::GetName() const {
		return mName;
	}

}

#endif // PlotBase.hxx
