#ifndef __PLOT__
#define __PLOT__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "BPF.hxx"
#include "Color.hxx"
#include <string>
#include "GenericPlot.hxx"

namespace CLAMVM
{
	class SequenceRenderingManager;

	class Plot : public GenericPlot
	{
	public:
		Plot( );
		Plot( std::string name );
		~Plot();

		virtual void SetYRange( double ymin, double ymax );
		void SetColor( Color c );
		void SetData( const CLAM::Array<CLAM::TData>& array, double xmin, double xmax );
		void SetData( const CLAM::BPF&, int samples = 100 );

	protected:

		virtual void      CreateWidget();

	protected:		
		SequenceRenderingManager* mRenderer;

	};


}

#endif // Plot.hxx
