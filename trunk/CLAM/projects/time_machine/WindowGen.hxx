#ifndef _WindowGen_
#define _WindowGen_


#include "Audio.hxx"
#include "DataTypes.hxx"
#include "Defines.hxx"
#include "DynamicType.hxx"
#include "EWindowtype.hxx"
#include "GlobalFunctions.hxx"
#include "OSDefines.hxx"
#include "ProcessingObject.hxx"
#include "Storage.hxx"
#include "Window.hxx"

#include "config.h"

#define	nWindowType = 14


namespace MTG {

  class WindowGen;

  class WindowGenConfig: public MTG::ProcessingObjectConfig
	{
	public:
    REG_NUM_ATTR_USING_INTERFACE (WindowGenConfig, 6, MTG::ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, WindowType, MTG::EWindowType);
		REGISTER (2, ForceZeroPhaseWindow, bool); //if activated: window length is always odd
		REGISTER (3, InvertWindow, bool);		//inverts the window function: used for synthesis
		REGISTER (4, OverlapWindow, bool);	//activates fixed overlap of 50%
		REGISTER (5, FrameSize, TInt32);	//framesize needed to calculate the overlapsize
    
		void Init();

	};

  /**
   *   This is just a template to which can be used to start a new Processing Object.
   *   Just copy it to another fiel and replace the  WindowGen with your  own 
   *   class name.
   *
   */
  
  class WindowGen: public ProcessingObject 
  {
  protected:	
	  
    WindowGenConfig mConfig;

		double BesselFunction(double X);
    
    void KaiserBessel(TSize size,TData *pWindow,double alpha);
    
    void BlackmanHarris62(TSize size,TData *pWindow);
    
    void BlackmanHarris70(TSize size,TData *pWindow);
    
    void BlackmanHarris74(TSize size,TData *pWindow);
    
    void BlackmanHarris92(TSize size,TData *pWindow);
    
    void Hamming(TSize size,TData *pWindow);
    
    void Triangular(TSize size,TData *pWindow);

		void Constant(TSize size,TData *pWindow);

  private:
		const char *GetClassName() const {return "WindowGen";}

    bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:

		WindowGen(void);

		WindowGen(const WindowGenConfig &c);

	  ~WindowGen();

		const ProcessingObjectConfig &GetConfig() const { return mConfig;}

		bool Do(void);

    bool Do(TSize size, Array<TData> &out);

		bool Do(TSize size, Window	&out);

    bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

    void StoreOn(Storage &s) {};

	private:
	};
} //end of namespace MTG

#endif // _WindowGen_
