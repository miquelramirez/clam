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
		const char* id = descriptor->Label;
		CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
		if (factory.KeyExists(id))
		{
			std::cout << "The LADSPA plugin '" << id 
			<< "' is contained by two libraries. Uninstall one: \n1) " 
			<< pluginFullFilename << "\n2) " << factory.GetValueFromAttribute(id, "library")
			<< std::endl;

		}
		factory.AddCreatorWarningRepetitions(id,
				new CLAM::LadspaWrapperCreator(pluginFullFilename,
					i,
					id));
		factory.AddAttribute(id, "category", "LADSPA");
		factory.AddAttribute(id, "description", descriptor->Name);
		factory.AddAttribute(id, "library", pluginFullFilename);

		std::ostringstream oss;
		oss << descriptor->Label << "_buffer" << i;
		std::string id2=oss.str();
		factory.AddCreatorWarningRepetitions(id2,
				new CLAM::LadspaWrapperBufferCreator(pluginFullFilename,
					i,
					id2));
		factory.AddAttribute(id2, "category", "LADSPA_BUFFER");
		factory.AddAttribute(id2, "description", descriptor->Name);
		factory.AddAttribute(id2, "library", pluginFullFilename);

		//std::cout << "[LADSPA] added \"" << plugin.factoryID << "\" to the Factory" << std::endl;
	}
	if (ReleaseLibraryHandler(handle, pluginFullFilename))
	{
		std::cout<<"[LADSPA Plugin] error unloading library handle of: " << pluginFullFilename<<std::endl;
		std::cout<<LibraryLoadError()<<std::endl;
	}
}
