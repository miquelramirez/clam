#ifndef CheckClamNetwork_hxx
#define CheckClamNetwork_hxx

#include <CLAM/XMLStorage.hxx>
#include <CLAM/Network.hxx>
#include <iostream>
#include <fstream>
#include <string>

#ifdef USE_JACK
    #include <CLAM/JACKNetworkPlayer.hxx>
#endif
#ifdef USE_PORTAUDIO
    #include <CLAM/PANetworkPlayer.hxx>
#endif

class CheckClamNetwork
{
    CheckClamNetwork()  {}
    ~CheckClamNetwork() {}


    static bool FileExists( const std::string filename )
    {
        //Check for existence of XML Network file
        std::ifstream file( filename.c_str() );
        if( !file ) return false;
        return true;
    }

public:

    static bool isCorrectNetwork(const std::string _networkFile)
    {
        try
        {
            CLAM::Network _network;
            CLAM::XMLStorage::Restore(_network, _networkFile);
            return true;
        }
        catch (CLAM::XmlStorageErr & e)
        {
            if(FileExists(_networkFile))
            {
                std::cout << "An occurred while loading the network file: ";
                std::cout << e.what() << std::endl;
            }
            else
            {
                std::cout << "Network file " << _networkFile << std::endl;
            }

        }
        return false;
    }

};

#endif //CheckClamNetwork_hxx