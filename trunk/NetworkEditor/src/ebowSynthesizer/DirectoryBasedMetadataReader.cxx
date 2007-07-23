#include "DirectoryBasedMetadataReader.hxx"
#include <CLAM/XMLStorage.hxx>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

namespace CLAM {

DirectoryBasedMetadataReader::DirectoryBasedMetadataReader()
{
}

DirectoryBasedMetadataReader::~DirectoryBasedMetadataReader()
{
}

void DirectoryBasedMetadataReader::ReadMetadata(std::string& argDirectoryPath, std::vector<CLAM::SampleMetadata>& argMetadataVector)
{
	DIR *directoryPtr;
	struct dirent *directoryEntryPtr;

	directoryPtr = opendir ( argDirectoryPath.c_str() );
	if (directoryPtr == NULL)
	{
		throw new CLAM::BadFilenameException();
	}
	else
	{
		while (directoryEntryPtr = readdir (directoryPtr))
		{
//				const char* tmp = ".";
			if (strncmp(directoryEntryPtr->d_name, ".", 1) != 0)
			{
				if (strstr(directoryEntryPtr->d_name, ".xml") != NULL)
				{
					std::string absolutePath = argDirectoryPath + std::string("/") + std::string(directoryEntryPtr->d_name);

					//std::cout << "DirectoryBasedMetadataReader:ReadMetadata; found XML file: " << absolutePath << std::endl;

					CLAM::SampleMetadata sampleConfig;
					CLAM::XMLStorage::Restore(sampleConfig, absolutePath);
					sampleConfig.ConfigureAfterRestore();

					argMetadataVector.push_back( sampleConfig );
				}
			}
		}
		(void) closedir (directoryPtr);
	}
}

} // end namespace CLAM
