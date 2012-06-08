#include <CLAM/MIDIManager.hxx>
#include <CLAM/MIDISettings.hxx>

#ifdef WIN32
#include <portmidi.h>
#endif

namespace CLAM
{
    namespace VM
    {

		MIDISettings::MIDISettings()
		{
			LoadMIDISettings();
		}

		MIDISettings::~MIDISettings()
		{
		}

		std::vector<std::string> MIDISettings::GetMIDIDevices()
		{
			return mMIDIDevices;
		}

		std::vector<std::string> MIDISettings::GetMIDIDeviceNameLst()
		{
			return mMIDIDeviceNameLst;
		}

		std::vector<int> MIDISettings::GetAvailableMIDIPrograms()
		{
			return mMIDIPrograms;
		}

		std::vector<std::string> MIDISettings::GetAvailableMIDIProgramNameLst()
		{
			return mMIDIProgramNameLst;
		}

		void MIDISettings::LoadMIDISettings()
		{
			LoadMIDIDevices();
			SetMIDIPrograms();
		}

		void MIDISettings::LoadMIDIDevices()
		{
			MIDIManager manager;	
			MIDIDeviceList* deviceLst = manager.FindList();
			if(!deviceLst) return;
			std::vector<std::string> availableDevices = deviceLst->AvailableDevices();
			std::vector<std::string>::iterator it = availableDevices.begin();
			for(int i=0; it != availableDevices.end(); it++, i++) 
			{
#ifdef WIN32			
				const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
				if(info->output)
				{
					mMIDIDevices.push_back((*it));
					mMIDIDeviceNameLst.push_back(info->name);
				}		
#else
				mMIDIDevices.push_back((*it));
				mMIDIDeviceNameLst.push_back((*it));
#endif
			}

		}

		void MIDISettings::SetMIDIPrograms()
		{
			// names
			mMIDIProgramNameLst.push_back("Piano");
			mMIDIProgramNameLst.push_back("Hammond Organ");
			mMIDIProgramNameLst.push_back("Violin");
			mMIDIProgramNameLst.push_back("Trumpet");
			mMIDIProgramNameLst.push_back("Sax");
			mMIDIProgramNameLst.push_back("Oboe");
			mMIDIProgramNameLst.push_back("Flute");

			// identifiers
			mMIDIPrograms.push_back(0);  // Piano
			mMIDIPrograms.push_back(16); // Hammond Organ
			mMIDIPrograms.push_back(40); // Violin
			mMIDIPrograms.push_back(56); // Trumpet
			mMIDIPrograms.push_back(66); // Sax
			mMIDIPrograms.push_back(68); // Oboe
			mMIDIPrograms.push_back(75); // Flute
		}
    }
}

// END

