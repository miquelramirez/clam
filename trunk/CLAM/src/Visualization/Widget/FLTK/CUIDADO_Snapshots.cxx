#include "CUIDADO_Snapshots.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include "FLDisplayContainer.hxx"
#include "FLDisplayArray.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include "ErrGUI.hxx"

using namespace CLAMGUI;

namespace CLAMGUI {

void CalculateWndSize( int& x, int& y, int& w, int& h )
{
	int screenH = Fl::h();
	int screenW = Fl::w();
	
	x = static_cast<int>( (1.0/4.0) * static_cast<double>( screenW ) );
	y = static_cast<int>( (1.0/4.0) * static_cast<double>( screenH ) );
	
	w = static_cast<int>( (1.0/2.0) * static_cast<double>( screenW ) );
	h = static_cast<int>( (1.0/2.0) * static_cast<double>( screenH ) );
	
}

void DrawArray( DataArray& arr, const char* label )
{
	int x0, y0, w, h;
	
	CalculateWndSize( x0, y0, w, h );
	
	Fl_Double_Window win = Fl_Double_Window( x0, y0, w, h, label  );
	
	TData max = *( std::max_element( arr.GetPtr(), arr.GetPtr()+arr.Size()-1  ) );
	TData min = *( std::min_element( arr.GetPtr(), arr.GetPtr()+arr.Size()-1 ) );

	if (min==max) {
		// Range = 1;
		min-=0.5;
		max+=0.5;
	}

	FLDisplayContainer mc ( 0, 0, w, h );

	FLDisplayArray<TData> display( &arr  );
	
	mc.Add( display );

	mc.SetHorRange( 0, arr.Size() );

	mc.SetVerRange( max, -( max - min)  );

	mc.mpHorRuler->mInteger = true;

	display.color2( FL_GREEN );

	win.resizable( &mc );

	win.end();

	win.show();

	Fl::run();
  }

void ShowSnapshot( DataArray& arr, const char* label )
{		DrawArray( arr, label ); }

void ShowSnapshot( Spectrum& spec, const char* label, bool showmag, bool showphase )
{
	
	CLAM::SpectrumConfig aux;
	spec.GetConfig(aux);

	if ( !aux.GetType().bMagPhase )
	  throw ( GUIException( "Spectrum MUST be in Magnitude Phase structure" ) );
	
	DataArray arr0 = spec.GetMagBuffer();
	DataArray arr1 = spec.GetPhaseBuffer();
	
	if ( showmag )
	  DrawArray( arr0, label );
	if ( showphase )
	  DrawArray( arr1, label );
	
}

void ShowSnapshot( Audio& aud, const char* label )
{
	DataArray arr = aud.GetBuffer();	
	DrawArray(arr, label );
}

void ShowSnapshot( Audio& aud, CLAM::Array<int> mask, const char* label, unsigned char r, unsigned char g, unsigned char b )
{
	DataArray arr = aud.GetBuffer();
	
	int x0, y0, w, h;
	
	CalculateWndSize( x0, y0, w, h );
	
	Fl_Double_Window win = Fl_Double_Window( x0, y0, w, h, label  );
	
	TData max = *( std::max_element( arr.GetPtr(), arr.GetPtr()+arr.Size()-1  ) );
	TData min = *( std::min_element( arr.GetPtr(), arr.GetPtr()+arr.Size()-1 ) );
	
	if (min==max) {
		if(min==0) {
			min-=0.5;
			max+=0.5;
		}
		else {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
	} 
	
	FLDisplayContainer mc ( 0, 0, w, h );
	
	Fl_Color c = fl_color_cube( r*FL_NUM_RED/256, g*FL_NUM_GREEN/256, b*FL_NUM_BLUE/256 ); 
	
	FLDisplayArray<TData> display( &arr, mask, c );
	
	mc.Add( display );
	
	mc.SetHorRange( 0, arr.Size() );
	
	mc.SetVerRange( max, -( max - min)  );
	
	mc.mpHorRuler->mInteger = true;
     
	display.color2( FL_GREEN );
	win.resizable( &mc );
	win.end();
	win.show();
	Fl::run();
}

void ShowSnapshot( DataArray& arr1, DataArray& arr2, const char* label)
{
		int x0, y0, w, h;
	
		CalculateWndSize( x0, y0, w, h );
		
		Fl_Double_Window win = Fl_Double_Window( x0, y0, w, h, label  );
		
		TData max = *( std::max_element( arr1.GetPtr(), arr1.GetPtr()+arr1.Size()-1  ) );
		TData min = *( std::min_element( arr1.GetPtr(), arr1.GetPtr()+arr1.Size()-1 ) );
		
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		
		if ( arr1.Size() != arr2.Size() ) 
			throw ( GUIException( "Compared Arrays Must Have same length" ));
		
		FLDisplayContainer mc ( 0, 0, w, h );
		
		FLDisplayArray<TData> display;
		
		display.AddArray( &arr1, FL_GREEN );
		display.AddArray( &arr2, FL_RED );
		
		mc.Add( display );
		
		mc.SetHorRange( 0, arr1.Size() );
		
		mc.SetVerRange( max, -( max - min)  );
		
		mc.mpHorRuler->mInteger = true;
		
		display.color2( FL_GREEN );
		
		win.resizable( &mc );
		win.end();
		win.show();
		Fl::run();
}

  void ShowSnapshot( Spectrum& sp, SpectralPeakArray& peak_array, const char* label , bool indexed)
  {
		int x0, y0, w, h;
		
		CalculateWndSize( x0, y0, w, h );
		
		Fl_Double_Window win = Fl_Double_Window( x0, y0, w, h, label  );
		
		TData max = *(std::max_element(sp.GetMagBuffer().GetPtr(), sp.GetMagBuffer().GetPtr()+sp.GetMagBuffer().Size()-1));
		TData min = *(std::min_element( sp.GetMagBuffer().GetPtr(), sp.GetMagBuffer().GetPtr()+sp.GetMagBuffer().Size()-1));

		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		 
		FLDisplayContainer mc(0,0,w,h);     
		FLDisplayArray<TData> display(&sp.GetMagBuffer());
		
		DataArray peaks(sp.GetSize());
		peaks.Resize(sp.GetSize());

 		peaks.SetSize(sp.GetSize());
		
		TData max2 = *(std::max_element(peak_array.GetMagBuffer().GetPtr(), peak_array.GetMagBuffer().GetPtr()+
																		peak_array.GetMagBuffer().Size()-1));
		TData min2 = *(std::min_element(peak_array.GetMagBuffer().GetPtr(), peak_array.GetMagBuffer().GetPtr()+
																		peak_array.GetMagBuffer().Size()-1));
		
		if (max2 > max) max=max2;
		if (min2 < min) min=min2;

		// Initializing the peak array
		for (int k = 0; k < sp.GetSize(); k++)
			peaks[k] = min;
		
		// Inserting the peaks magnitudes
		if (!indexed) // all peaks
			{
				for ( int i = 0; i < peak_array.GetnPeaks(); i++ )
					{	peaks[ (int) peak_array.GetBinPos(i) ] = peak_array.GetMag( i );	}
			}
		else { // plot only indexed peaks
			for (int i = 0; i < peak_array.GetIndexArray().Size(); i++)
				{ peaks[(int) peak_array.GetThruIndexBinPos(i) ] = peak_array.GetThruIndexMag(i);}
		}
		
		display.AddArray(&peaks,FL_YELLOW);
		
		mc.Add( display );     
		mc.SetHorRange( 0, sp.GetSize() );
		mc.SetVerRange( max, -( max - min)  );
		
		mc.mpHorRuler->mInteger = true;
		
		display.color2( FL_GREEN );
    
		win.resizable( &mc ); 
		win.end();
		win.show();
		
		Fl::run();
  }

 void ShowSnapshot( Spectrum& sp, SpectralPeakArray& peak_array, DataArray &selec, const char* label , bool indexed)
  {

		int x0, y0, w, h;
		
		CalculateWndSize( x0, y0, w, h );
		
		Fl_Double_Window win = Fl_Double_Window( x0, y0, w, h, label  );
		
		TData max = *(std::max_element(sp.GetMagBuffer().GetPtr(), sp.GetMagBuffer().GetPtr()+sp.GetMagBuffer().Size()-1));
		TData min = *(std::min_element( sp.GetMagBuffer().GetPtr(), sp.GetMagBuffer().GetPtr()+sp.GetMagBuffer().Size()-1));

		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		 
		FLDisplayContainer mc(0,0,w,h);     
		FLDisplayArray<TData> display(&sp.GetMagBuffer());
		
		DataArray peaks(sp.GetSize());
		peaks.SetSize(sp.GetSize());
		
		TData max2 = *(std::max_element(peak_array.GetMagBuffer().GetPtr(), peak_array.GetMagBuffer().GetPtr()+
																		peak_array.GetMagBuffer().Size()-1));
		TData min2 = *(std::min_element(peak_array.GetMagBuffer().GetPtr(), peak_array.GetMagBuffer().GetPtr()+
																		peak_array.GetMagBuffer().Size()-1));
		
		if (max2 > max) max=max2;
		if (min2 < min) min=min2;

		// Initializing the peak array
		for (int k = 0; k < sp.GetSize(); k++)
			peaks[k] = min;
		
		// Inserting the peaks magnitudes
		if (!indexed) // all peaks
			{
				for ( int i = 0; i < peak_array.GetnPeaks(); i++ )
					{peaks[ (int) peak_array.GetBinPos(i)  ] = peak_array.GetMag( i );}
			}
		else { // plot only indexed peaks
			for (int i = 0; i < peak_array.GetIndexArray().Size(); i++)
				{ peaks[ (int) peak_array.GetThruIndexBinPos(i) ] = peak_array.GetThruIndexMag(i);}
		}
		
		display.AddArray(&peaks,FL_YELLOW);
		display.AddArray(&selec, FL_RED);

		mc.Add( display );     
		mc.SetHorRange( 0, sp.GetSize() );
		mc.SetVerRange( max, -( max - min)  );

		mc.mpHorRuler->mInteger = true;
		
		display.color2( FL_GREEN );		
		

		win.resizable( &mc ); 
		win.end();
		win.show();
		
		Fl::run();
  }

  void ShowSnapshot(Spectrum* sp[], const char* label , int numBands, int size)
  {
		// Show likelihood structure
     int x0, y0, w, h;
	
     CalculateWndSize( x0, y0, w, h );
     
     Fl_Double_Window win = Fl_Double_Window(x0,y0,w,h,label);
     
	 DataArray maxval(numBands);
	 DataArray minval(numBands);
	 maxval.SetSize(numBands);
	 minval.SetSize(numBands);
	 int i,j;

	 FLDisplayContainer mc(0,0,w,h);     
     FLDisplayArray<TData> display(&sp[0]->GetMagBuffer());


	 maxval[0] = *(std::max_element(sp[0]->GetMagBuffer().GetPtr(),
															 sp[0]->GetMagBuffer().GetPtr()+size-1));
	 minval[0] = *(std::min_element(sp[0]->GetMagBuffer().GetPtr(),
					 sp[0]->GetMagBuffer().GetPtr()+size-1));
		 if (minval[0]==maxval[0]) {
			if(minval[0]==0) {
				 minval[0]-=0.5;
				 maxval[0]+=0.5;
			 }
			else {
			 // Range = 1;
			 minval[0]-=minval[0]/10;
			 maxval[0]+=maxval[0]/10;
			}
		 }

		 TData cte = maxval[0]-minval[0];

	 
		 CLAM::Array< DataArray > a(numBands);  // VERY SLOW!!!
		 a.SetSize(numBands);

		 for(i=1; i<numBands; i++) {

			 for(j=0;j<size;j++)
				 a[i].AddElem(sp[i]->GetMagBuffer()[j]+cte);

			 display.AddArray(&a[i],FL_GREEN);

			 maxval[i] = *(std::max_element(sp[i]->GetMagBuffer().GetPtr(),
																	 sp[i]->GetMagBuffer().GetPtr()+size-1));
			 minval[i] = *(std::min_element(sp[i]->GetMagBuffer().GetPtr(),
																	 sp[i]->GetMagBuffer().GetPtr()+size-1));
			 
			 if (minval[i]==maxval[i]) {
				 if(minval[i]==0) {
					 minval[i]-=0.5;
					 maxval[i]+=0.5;
				 }
				 else {
				 // Range = 1;
				 minval[i]-=minval[i]/10;
				 maxval[i]+=maxval[i]/10;
				 }
			 }
			 cte += maxval[i]-minval[i];
     }

     mc.Add(display);     
     mc.SetHorRange(0,size);
     mc.SetVerRange(cte+maxval[numBands-1], -(cte  - minval[0]));
     
     mc.mpHorRuler->mInteger = true;
     
     display.color2(FL_GREEN);
    
     win.resizable(&mc); 
     win.end();
     win.show();
     
     Fl::run();
/*	 delete maxval;
	 delete minval; */
  }

  void ShowSnapshot( Spectrum* sequence[], int at_once, const char* label, int size )
  { 
    int screen_h = Fl::h();
    int screen_w = Fl::w();

    int x, y, w, h;
		
    x = 50; y = 50;
    w = screen_w / 2; h = screen_h;
		
    Fl_Double_Window* main_win = new Fl_Double_Window( x, y, w, h-50, label );
		
    Fl_Scroll* container = new Fl_Scroll( 0, 0, w, h-50 );
		
    main_win->end();
		
    // Creating the necessary arrays
		
    int k = 0;
    
    int disp_h = screen_h / at_once;
		
    for ( k = 0; k < size; k++ )
      {
				
				FLDisplayContainer* mc = new FLDisplayContainer ( 0, disp_h * k +y, screen_w/2, disp_h  );
				
				DataArray& array = sequence[size-1-k]->GetMagBuffer();
				
				TData max = *( std::max_element( array.GetPtr(), array.GetPtr()+array.Size()-1  ) );
				TData min = *( std::min_element( array.GetPtr(), array.GetPtr()+array.Size()-1 ) );
				
				if (min==max) {
					// Range = 1;
					min-=0.5;
					max+=0.5;
				}
				
				FLDisplayArray<TData>*  display = new FLDisplayArray<TData>( &array  );
				
				mc->Add( *display );
				
				mc->SetHorRange( 0, array.Size() );
				
				mc->SetVerRange( max, -( max - min)  );
				
				mc->mpHorRuler->mInteger = true;
				
				display->color2( FL_GREEN );
				
				//	main_->resizable( &mc );
				container->add( mc );
      }
    
		main_win->show();
		
    Fl::run();
		
    delete main_win;
		
  }
	
	void ShowSnapshot(DataArray &audio, DataArray &desc1, DataArray &desc2, const char* label)
  { 
		int screen_h = Fl::h();
    int screen_w = Fl::w();
		
    int x, y, w, h;
		
    x = 50; y = 50;
    w = screen_w / 2; h = screen_h;
		
    Fl_Double_Window* main_win = new Fl_Double_Window( x, y, w, h-50, label );
		
    Fl_Scroll* container = new Fl_Scroll( 0, 0, w, h-50 );
		
    main_win->end();
		
    // Creating the necessary arrays
		
		int disp_h = screen_h / 3;
		
		FLDisplayContainer* mc = new FLDisplayContainer ( 0, y, screen_w/2, disp_h );
		TData max = *( std::max_element( audio.GetPtr(), audio.GetPtr()+audio.Size()-1  ) );
		TData min = *( std::min_element( audio.GetPtr(), audio.GetPtr()+audio.Size()-1 ) );
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		FLDisplayArray<TData>*  display = new FLDisplayArray<TData>( &audio  );
		mc->Add( *display );
		mc->SetHorRange( 0, audio.Size() );
		mc->SetVerRange( max, -( max - min)  );
		mc->mpHorRuler->mInteger = true;
		display->color2( FL_YELLOW );
		container->add( mc );

		FLDisplayContainer* mc2 = new FLDisplayContainer ( 0, disp_h + y, screen_w/2, disp_h );			
		max = *( std::max_element( desc1.GetPtr(), desc1.GetPtr()+desc1.Size()-1  ) );
		min = *( std::min_element( desc1.GetPtr(), desc1.GetPtr()+desc1.Size()-1 ) );
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		FLDisplayArray<TData>*  display2 = new FLDisplayArray<TData>( &desc1  );
		mc2->Add( *display2 );
		mc2->SetHorRange( 0, desc1.Size() );
		mc2->SetVerRange( max, -( max - min)  );
		mc2->mpHorRuler->mInteger = true;
		display2->color2( FL_GREEN );
		container->add( mc2 );
		
		FLDisplayContainer* mc3 = new FLDisplayContainer ( 0, disp_h * 2 + y, screen_w/2, disp_h );
		max = *( std::max_element( desc2.GetPtr(), desc2.GetPtr()+desc2.Size()-1  ) );
		min = *( std::min_element( desc2.GetPtr(), desc2.GetPtr()+desc2.Size()-1 ) );
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		FLDisplayArray<TData>*  display3 = new FLDisplayArray<TData>( &desc2  );
		mc3->Add( *display3 );
		mc3->SetHorRange( 0, desc2.Size() );
		mc3->SetVerRange( max, -( max - min)  );
		mc3->mpHorRuler->mInteger = true;
		display3->color2( FL_RED );
		container->add( mc3 );
		
		main_win->show();
		
    Fl::run();
		
    delete main_win;
	}


void ShowSnapshot(DataArray& audio, CLAM::Array<int> mask, DataArray &desc1, DataArray &desc2, const char* label=0)
  { 
		int screen_h = Fl::h();
    int screen_w = Fl::w();
		
    int x, y, w, h;
		
    x = 50; y = 50;
    w = screen_w / 2; h = screen_h;
		
    Fl_Double_Window* main_win = new Fl_Double_Window( x, y, w, h-50, label );
		
    Fl_Scroll* container = new Fl_Scroll( 0, 0, w, h-50 );
		
    main_win->end();
		
    // Creating the necessary arrays
		
		int disp_h = screen_h / 3;
		
		FLDisplayContainer* mc = new FLDisplayContainer ( 0, y, screen_w/2, disp_h );
		TData max = *( std::max_element( audio.GetPtr(), audio.GetPtr()+audio.Size()-1  ) );
		TData min = *( std::min_element( audio.GetPtr(), audio.GetPtr()+audio.Size()-1 ) );
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		
		//not included in signature!!!!
		int r=0,g=0,b=255;
		Fl_Color c = fl_color_cube( r*FL_NUM_RED/256, g*FL_NUM_GREEN/256, b*FL_NUM_BLUE/256 ); 
		FLDisplayArray<TData>* display= new FLDisplayArray<TData>( &audio, mask, c );
		//FLDisplayArray<TData>*  display = new FLDisplayArray<TData>( &audio  );
		
		mc->Add( *display );
		mc->SetHorRange( 0, audio.Size() );
		mc->SetVerRange( max, -( max - min)  );
		mc->mpHorRuler->mInteger = true;
		display->color2( FL_YELLOW );
		container->add( mc );

		FLDisplayContainer* mc2 = new FLDisplayContainer ( 0, disp_h + y, screen_w/2, disp_h );			
		max = *( std::max_element( desc1.GetPtr(), desc1.GetPtr()+desc1.Size()-1  ) );
		min = *( std::min_element( desc1.GetPtr(), desc1.GetPtr()+desc1.Size()-1 ) );
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		FLDisplayArray<TData>*  display2 = new FLDisplayArray<TData>( &desc1  );
		mc2->Add( *display2 );
		mc2->SetHorRange( 0, desc1.Size() );
		mc2->SetVerRange( max, -( max - min)  );
		mc2->mpHorRuler->mInteger = true;
		display2->color2( FL_GREEN );
		container->add( mc2 );
		
		FLDisplayContainer* mc3 = new FLDisplayContainer ( 0, disp_h * 2 + y, screen_w/2, disp_h );
		max = *( std::max_element( desc2.GetPtr(), desc2.GetPtr()+desc2.Size()-1  ) );
		min = *( std::min_element( desc2.GetPtr(), desc2.GetPtr()+desc2.Size()-1 ) );
		if (min==max) {
			// Range = 1;
			min-=0.5;
			max+=0.5;
		}
		FLDisplayArray<TData>*  display3 = new FLDisplayArray<TData>( &desc2  );
		mc3->Add( *display3 );
		mc3->SetHorRange( 0, desc2.Size() );
		mc3->SetVerRange( max, -( max - min)  );
		mc3->mpHorRuler->mInteger = true;
		display3->color2( FL_RED );
		container->add( mc3 );
		
		main_win->show();
		
    Fl::run();
		
    delete main_win;
	}

  void ShowSnapshot( CLAM::Array< Spectrum* >& sequence, int at_once, const char* label )
  { 
    int screen_h = Fl::h();
    int screen_w = Fl::w();

    int x, y, w, h;

    x = 50; y = 50;
    w = screen_w / 2; h = screen_h;

    Fl_Double_Window* main_win = new Fl_Double_Window( x, y, w, h-50, label );

    Fl_Scroll* container = new Fl_Scroll( 0, 0, w, h-50 );

    main_win->end();

    // Creating the necessary arrays

    int k = 0;
    
    int disp_h = screen_h / at_once;

    for ( k = 0; k < sequence.Size(); k++ )
      {

	FLDisplayContainer* mc = new FLDisplayContainer ( 0, disp_h * k +y, screen_w/2, disp_h  );

	DataArray& array = sequence[k]->GetMagBuffer();

	TData max = *( std::max_element( array.GetPtr(), array.GetPtr()+array.Size()-1  ) );
	TData min = *( std::min_element( array.GetPtr(), array.GetPtr()+array.Size()-1 ) );
	
	FLDisplayArray<TData>*  display = new FLDisplayArray<TData>( &array  );
	
	mc->Add( *display );

	mc->SetHorRange( 0, array.Size() );

	mc->SetVerRange( max, -( max - min)  );

	mc->mpHorRuler->mInteger = true;

	display->color2( FL_GREEN );

	//	main_->resizable( &mc );
	container->add( mc );
      }
    
	main_win->show();

    Fl::run();

    delete main_win;

  }

  void ShowSnapshot( CLAM::Array< DataArray* >& sequence, int at_once, const char* label )
  {
    int screen_h = Fl::h();
    int screen_w = Fl::w();

    int x, y, w, h;

    x = 50; y = 50;
    w = screen_w / 2; h = screen_h;

    Fl_Double_Window* main_win = new Fl_Double_Window( x, y, w, h-50, label );

    Fl_Scroll* container = new Fl_Scroll( 0, 0, w, h-50 );

    main_win->end();

    // Creating the necessary arrays

    int k = 0;
    
    int disp_h = screen_h / at_once;

    for ( k = 0; k < sequence.Size(); k++ )
      {

	FLDisplayContainer* mc = new FLDisplayContainer ( 0, disp_h * k + y, screen_w/2, disp_h );

	DataArray& array = *(sequence[k]);

	TData max = *( std::max_element( array.GetPtr(), array.GetPtr()+array.Size()-1  ) );
	TData min = *( std::min_element( array.GetPtr(), array.GetPtr()+array.Size()-1 ) );
	
	FLDisplayArray<TData>*  display = new FLDisplayArray<TData>( &array  );
	
	mc->Add( *display );

	mc->SetHorRange( 0, array.Size() );

	mc->SetVerRange( max, -( max - min)  );

	mc->mpHorRuler->mInteger = true;

	display->color2( FL_GREEN );

	//	main_->resizable( &mc );
	container->add( mc );
      }

	main_win->show();

    Fl::run();

    delete main_win;
  }

}
