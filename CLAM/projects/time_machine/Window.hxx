#ifndef _Window_
#define _Window_

#include "Array.hxx"
#include "DataTypes.hxx"
#include "Err.hxx"
#include "EWindowType.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"
#include "Window.hxx"


namespace MTG {

  class Window;

  /** Class to store and manipulate the TMWindow settings.
  */
  class WindowConfig: public MTG::ProcessingDataConfig
  {
  public:
    REG_NUM_ATTR_USING_INTERFACE (WindowConfig, 0, MTG::ProcessingDataConfig);

    
  public:

    void Init();

  };

  /** Tentative Window class 
  */
  class Window: public MTG::ProcessingData 
  {
  public:
    REG_NUM_ATTR_USING_INTERFACE (Window, 4, MTG::ProcessingData);

  private:
    REGISTER (0, prConfig, WindowConfig);
		REGISTER (1, prSize, TSize);
            
	void Init();                        // To be used from the dynamic-type-generated constructor.
  
  public:
		REGISTER (2, WindowBuffer, MTG::Array<TData>);
		REGISTER (3, WindowType, MTG::EWindowType);
    
    /** CONSTRUCTION AND CONFIGURATION
    */
    Window(const WindowConfig &newConfig) : MTG::ProcessingData(4)
    {
      Window_DefaultConstructor(); //what is the default constructor doing ?
      Configure(newConfig);
    }

    const MTG::ProcessingDataConfig &GetProcessingDataConfig() const
    {
      return GetConfig();
    }

    void Configure(const WindowConfig &newConfig);

    const WindowConfig &GetConfig() const;

    void SetConfig(MTG::ProcessingDataConfig&);

  
    /** METHODS FOR DATA ACCESS
    */
		TSize GetSize();

		void SetSize(TSize	size);

  };
}; //end of namespace MTG

#endif // _Window_
