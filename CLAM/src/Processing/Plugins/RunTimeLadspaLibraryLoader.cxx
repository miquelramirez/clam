#include "RunTimeLadspaLibraryLoader.hxx"

void RunTimeLadspaLibraryLoader::SetupLibrary(void* handle, const std::string & pluginFullFilename) const
{


	void * clam_marker= GetSymbol(handle, "clam_library_marker");
	if (clam_marker and _disableLoadingClamLadspas)
	{
		//std::cout << "[LADSPA Plugin] Skipping clam-ladspa plugin: " << pluginFullFilename << std::endl;
		return;
	}

	LADSPA_Descriptor_Function descriptorTable = 0;
	descriptorTable = (LADSPA_Descriptor_Function)GetSymbol(handle, "ladspa_descriptor");
	if (!descriptorTable)
	{
		std::cout << "[LADSPA Plugin] Warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
		return;
	}
	//std::cout << "[LADSPA] \topened plugin: " << pluginFullFilename << std::endl;
	for (unsigned long i=0; descriptorTable(i); i++)
	{
		LADSPA_Descriptor* descriptor = (LADSPA_Descriptor*)descriptorTable(i);
		std::ostringstream oss;
		oss << descriptor->UniqueID;
		const std::string uniqueId = oss.str();
		oss.str("");
		oss << "ladspa_" << descriptor->Label << "_" << uniqueId;
		std::string pluginName = oss.str();
		CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
		if (factory.KeyExists(pluginName))
		{
			std::cout << "The LADSPA plugin '" << pluginName 
			<< "' is contained by two libraries. Uninstall one: \n1) " 
			<< pluginFullFilename << "\n2) " << factory.GetValueFromAttribute(pluginName, "library")
			<< std::endl;
			continue;
		}
		
		if (factory.GetKeys("ladspa_id",uniqueId).size() != 0)
		{
			std::cout << "The LADSPA id " << uniqueId
			<< " is contained by two libraries. Uninstall one: \n1) " 
			<< pluginFullFilename << "\n2) " << factory.GetValueFromAttribute(factory.GetKeys("ladspa_id",uniqueId).front(),"library")
			<< std::endl;
		}


		factory.AddCreatorWarningRepetitions(pluginName,
				new CLAM::LadspaWrapperCreator(pluginFullFilename,
					i,
					pluginName));
		factory.AddAttribute(pluginName, "category", "LADSPA");
		factory.AddAttribute(pluginName, "description", descriptor->Name);
		factory.AddAttribute(pluginName, "library", pluginFullFilename);
		factory.AddAttribute(pluginName, "ladspa_id", uniqueId); 

		oss << "_buffer";
		pluginName = oss.str();
		factory.AddCreatorWarningRepetitions(pluginName,
				new CLAM::LadspaWrapperBufferCreator(pluginFullFilename,
					i,
					pluginName));
		factory.AddAttribute(pluginName, "category", "LADSPA_BUFFER");
		factory.AddAttribute(pluginName, "description", descriptor->Name);
		factory.AddAttribute(pluginName, "library", pluginFullFilename);
		factory.AddAttribute(pluginName, "ladspa_id",uniqueId); 

		//std::cout << "[LADSPA] added \"" << plugin.factoryID << "\" to the Factory" << std::endl;
	}
	if (ReleaseLibraryHandler(handle, pluginFullFilename))
	{
		std::cout<<"[LADSPA Plugin] error unloading library handle of: " << pluginFullFilename<<std::endl;
		std::cout<<LibraryLoadError()<<std::endl;
	}
}
