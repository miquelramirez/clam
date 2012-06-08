#ifndef __MIDISETTINGS__
#define __MIDISETTINGS__

#include <vector>
#include <string>

namespace CLAM
{
    namespace VM
    {
		class MIDISettings
		{
		public:
			MIDISettings();
			~MIDISettings();

			std::vector<std::string> GetMIDIDevices();
			std::vector<std::string> GetMIDIDeviceNameLst();

			std::vector<int> GetAvailableMIDIPrograms();
			std::vector<std::string> GetAvailableMIDIProgramNameLst();

		private:
			std::vector<std::string> mMIDIDevices;
			std::vector<std::string> mMIDIDeviceNameLst;
			std::vector<int>         mMIDIPrograms;
			std::vector<std::string> mMIDIProgramNameLst;

			void LoadMIDISettings();
			void LoadMIDIDevices();
			void SetMIDIPrograms();
		};
    }
}

#endif

