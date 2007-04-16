/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Processing.hxx"

namespace CLAM {

	/** Configuration class for PrintControl objects. It contains the name of the object and a string with the kind of message it prints.
	 *  @see PrintControl, ProcessingConfig
	 */
	class PrintControlConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PrintControlConfig, 2,ProcessingConfig);

		/** The name of PrintControl object related to this configuration*/
		DYN_ATTRIBUTE (0, public, std::string, Name);

		/** The kind of message that the PrintControl object with this configuration prints*/
		DYN_ATTRIBUTE (1, public, std::string, Message);
	protected:

		/** Default initialization of the object*/
		void DefaultInit()
		{
			AddName();
			AddMessage();
			UpdateData();
			SetMessage("print:");
		}
	};


	/** A class useful to debug watching what kind of MIDI messages we are processing. Prints in the screen the type of message, and its value. If the objective is control more than one type of data (i.e. Note On and Note Velocity), more than one PrintControl objects must be created.
	 *  @see Processing, PrintControlConfig
	 */
	class PrintControl:public Processing
	{
	private:
		PrintControlConfig mConfig;

	public:
		InControlTmpl<PrintControl> mInput;

		/** Constructor of the class. Receives by parameter the configuration that will be used to create the object
		 *  @param cfg The configuration of the new object, created by the user
		 */
		PrintControl(const PrintControlConfig& cfg)
		:mInput("input",this,&PrintControl::DoPrint) {
			Configure(cfg);
		}

		const char * GetClassName() const {return "PrintControl";}

		/** Configuration method interface. The Processing base class forces all the concrete classes derived from it to implement this method, which must actually perform the specific configuration tasks.  
		 *  Note that the user can not call this method directly. He will use Configure instead. The argument is expected to be an object of the necesary concrete configuration class.
		 *  @return true if the object has been configured correctly; true otherwise
		 *  @param The related ProcessingConfig object
		 */
		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig(mConfig, c);
			return true;
		}

		/** Getter for the configuration of the class
		 *  @return The ProcessingConfig object attached to this Processing object
		 */
		const ProcessingConfig &GetConfig() const { return mConfig;};

		/** Method created to print in default screen the values of MIDI messages catched by the MIDIDevice. Each PrintControl object controls messages of only one kind and one value (two values are controlled by two objects), so this method will print the type of message and its value.
		 *  @param val The value received with this message
		 *  @return 1 if method has been executed correctly
		 */
		int DoPrint(TControlData val)
		{
			printf("%s %f\n",mConfig.GetMessage().c_str(),val);
			return 1;
		}

		/** Supervised method, not implemented yet*/
		bool Do(void) { return true; }
	};

}

