#ifndef __FL_SMS_BPF_EDITOR__
#define __FL_SMS_BPF_EDITOR__

#include <FL/Fl_Group.H>
#include "BPF.hxx"
#include "Signalv0.hxx"
#include "Slotv2.hxx"
#include <string>

class Fl_Envelope_Scroll;
class Fl_Check_Button;
class Fl_Box;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor : public Fl_Group
	{
	public:
		Fl_SMS_BPF_Editor( int X, int Y, int W, int H );
		virtual ~Fl_SMS_BPF_Editor();

		void InitPoints( double yvalue );
		void InitPoints( const CLAM::BPF& );
		void InsertPointsIntoBPF( CLAM::BPF& editedBPF );
		void Clear();

		void SetGridWidth( double xwidth, double ywidth );
		void SetHorizontalRange( double xmin, double xmax );
		void SetVerticalRange( double ymin, double ymax );
		void SetXAxisLabel( const char* label );
		void SetYAxisLabel( const char* label );

		virtual void resize( int X, int Y, int W, int H );
		
		SigSlot::Signalv0   PointsChanged;	

	protected:

		Fl_Envelope_Scroll&   FunctionEditor() { 
			return *mpFunctionEditor;
		}

		Fl_Check_Button& SnapToGridButton() {
			return *mpSnapToGridBtn;
		}

		virtual void CreateContents();
		virtual void ApplyLayoutOnContents( int X, int Y, int W, int H);

		SigSlot::Slotv2<double,double>  NewPoint;
		SigSlot::Slotv0                 PointRemoved;
		SigSlot::Slotv0                 PointMoved;

		void OnNewPoint( double x, double y );
		void OnPointRemoved();
		void OnPointMoved();

		static void cbSnapToGrid( Fl_Check_Button* btn, void* data );

	protected:
		Fl_Envelope_Scroll* mpFunctionEditor;
		Fl_Check_Button*    mpSnapToGridBtn;
		Fl_Box*             mpXAxisLabelBox;
		Fl_Box*             mpYAxisLabelBox;
		std::string         mXAxisLabel;
		std::string         mYAxisLabel;
	};

}

#endif // Fl_SMS_BPF_Editor.hxx
