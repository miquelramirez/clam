#ifndef __PresentationWindow__
#define __PresentationWindow__

#include <FL/Fl_Double_Window.H>

namespace CLAM
{
	class Audio;
	class Spectrum;
}
using CLAM::Audio;
using CLAM::Spectrum;

namespace CLAMGUI
{

	// TODO: there should be a class in between Presentation and the 
	//  FLTK Presentations, that has a PresentationWindow as member
	// See also: AudioPresentation.hxx: GetWindow()
	class ProcDataPresentation<Audio>;
	class ProcDataPresentation<Spectrum>;
	
	class PresentationWindow:public Fl_Double_Window
	{
	protected:
		friend class ProcDataPresentation<Audio>; 
		friend class ProcDataPresentation<Spectrum>; 
	
		Presentation* mPresentation;

		PresentationWindow(Presentation* p,int x,int y,int w,int h,const char* l)
			:Fl_Double_Window(x,y,w,h,l)
		{
			mPresentation = p;
		}
	public:
		~PresentationWindow()
		{
		}
	public:
		Presentation* GetPresentation(void)
		{
			return mPresentation;
		}
	};
}

#endif
