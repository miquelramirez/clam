// Window.cxx: implementation of the TMWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "Window.hxx"


using namespace MTG;

///////////////////////////////////////////////////////
//
// WindowConfig
//
///////////////////////////////////////////////////////

  /** Init Configuration
  */ 
  void WindowConfig::Init()
  {
    // Attribute instantiation
		UpdateData();

    //default Array sizes


    // Attribute initialization (Default values)

  }

  /** GetConfig and SetConfig
  */
  const WindowConfig& Window::GetConfig() const
  {
    return GetprConfig();
  }

  void SetConfig(MTG::ProcessingDataConfig&);
  


//////////////////////////////////////////////////////////////////////
//
// Window
//
//////////////////////////////////////////////////////////////////////

/**Methods for initialization
 */

  void Window::Init()
  {
    Configure(WindowConfig());
  }

/** Methods for Configuration
 */

  void Window::Configure(const WindowConfig &newConfig)
  {
    AddprConfig();
		AddprSize();
		AddWindowBuffer();
    AddWindowType();
    
		UpdateData();
	
		//set default values
		SetprSize(0);
		SetWindowType(MTG::EWindowType::BLACKMAN_HARRIS_62);

		//set default winBufferSize
		GetWindowBuffer().Resize(0);
		GetWindowBuffer().SetSize(0);

  }

/**Shorthand Set and Get 
 */
		TSize Window::GetSize()
		{
			return GetWindowBuffer().Size();
		};

		void Window::SetSize(TSize	size)
		{
			if(size >=0 )
			{
				GetWindowBuffer().SetSize(size);
				GetWindowBuffer().Resize(size);
			}
			else
				throw Err("Window::SetSize: Size of window can not be below zero");
		};
