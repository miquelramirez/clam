#ifndef _BaseProcessing_
#define _BaseProcessing_

#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>

namespace CLAM{

	/**	\brief Short description
	 *
	 *	Description
	 */
	class BaseProcessing: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "BaseProcessing"; }
		
		/** Ports **/
		AudioInPort mInputAudio;
		AudioOutPort mOutputAudio;

		/** Controls **/
		InControl mInputControl;

	public:
		BaseProcessing(const Config & config=Config())
			:
			mInputAudio("Input Audio",this ),
			mOutputAudio("Audio Output",this),

			mInputControl("Control", this)
		{
			Configure( config );
		}

 		~BaseProcessing() {}

		bool Do()
		{
			bool result = Do( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );

			mInputAudio.Consume(); 
			mOutputAudio.Produce();

			return result;
		}
	
		bool Do(const Audio& in, Audio& out)
		{
			int size = in.GetSize();

			TData* inb = in.GetBuffer().GetPtr();
			TData* outb = out.GetBuffer().GetPtr();

			for (int i=0;i<size;i++) 
			{
				*outb++ = *inb++;
			}

			return true;
		}

	private:

		/** Child processings **/

	};	
	
};//namespace CLAM

#endif // _BaseProcessing_
