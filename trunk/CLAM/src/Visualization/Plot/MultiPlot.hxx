#ifndef __MULTIPLOT__
#define __MULTIPLOT__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "BPF.hxx"
#include "Color.hxx"
#include <string>
#include <map>
#include "GenericPlot.hxx"

namespace CLAMVM
{
	class SequenceRenderingManager;

	class MultiPlot : public GenericPlot
	{
	public:
		MultiPlot( );
		MultiPlot( std::string name );
		~MultiPlot();
		
		void SetColor( std::string key, Color c );
		void AddData( std::string key, const CLAM::Array<CLAM::TData>& array, double xmin, double xmax );
		void AddData( std::string key, const CLAM::BPF&, int samples = 100 );
		void RemoveData( std::string key );
		void RemoveAllData();

	protected:

		virtual void  CreateWidget();		
		bool          ExistsRenderer( const std::string& key );
		void          CreateNewRenderer( const std::string& key );
		void          UpdateWidgetRenderers();
		double        MinimumXOfAll();
		double        MaximumXOfAll();

	protected:		
		typedef std::map< std::string, SequenceRenderingManager* > RendererTable;

		RendererTable                  mRenderers;
		
	};


}



#endif // Multiplot.hxx
