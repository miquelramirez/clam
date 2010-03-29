// CoreoSequencer
//
//

#include <CLAM/OfflineNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/BaseNetwork.hxx>

#include <iostream>
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////////

/**
 *
 */
class CoreoSequencer
{
  std::string _coreoFile;
  double _frameRate;

  typedef std::vector<double> values_t;

  struct CoreoEntry
  {
    std::string mtc;
    std::string name;
    values_t values;
  };

  typedef std::vector<CoreoEntry> coreo_entries_t;
  coreo_entries_t _coreoEntries;
  coreo_entries_t::iterator _coreoEvent; //const

  // allows for multiple values, separated by ',', no space!
  // i.e. '00:00:00:00 /gaincontrol 1.0,2.0,3.0'
  values_t GetCoreoValues(std::string const& val)
  {
    values_t values;
    std::stringstream ss(val);
    std::string item;
    while(std::getline(ss, item, ',')) 
      values.push_back(atof(item.c_str()));
    return values;
  }

  double SmpteToMs(std::string const& mtc)
  {
    std::vector<std::string> elems;
    std::stringstream ss(mtc);
    std::string item;
    while(std::getline(ss, item, ':')) 
        elems.push_back(item);
    int hours = atoi(elems[0].c_str());
    int minutes = atoi(elems[1].c_str());
    int seconds = atoi(elems[2].c_str());
    int frames = atoi(elems[3].c_str());
    double frame_length = 1000. / _frameRate;

    double mtcInMillisec = 
      (double)hours * 3600000. + (double)minutes * 60000. 
        + (double)seconds * 1000. + (double)frames * frame_length;

    return mtcInMillisec;
  }
  
  std::string MsToSmpte(int milliseconds)
  {
    std::stringstream hours;
    hours.width(2);
    hours.fill('0');
		hours << int(milliseconds / 3600000);
    std::stringstream minutes;
    minutes.width(2);
    minutes.fill('0');
		minutes << int((milliseconds % 3600000)/60000);
    std::stringstream seconds;
    seconds.width(2);
    seconds.fill('0');
		seconds << int((milliseconds % 3600000)%60000/1000);
    std::stringstream frames;
    frames.width(2);
    frames.fill('0');
    double frame_length = 1000. / _frameRate;
		frames << int(int(((milliseconds % 3600000)%60000)%1000)/frame_length);

    std::string smpte = 
      hours.str() + ":" + minutes.str() + ":" + seconds.str() + ":" +frames.str();

    return smpte;
  }

  void PreprocessCoreoFile()
  {
    std::ifstream file(_coreoFile.c_str(), std::ifstream::in);

    std::string mtc;
    std::string name;
    std::string values;
    
    std::string str;
    while(std::getline(file, str))
    {
      std::stringstream ss(str);
      ss >> mtc >> name >> values;  
      //std::cout << "mtc=" << mtc << " name=" << name 
      //  << " values=" << values << std::endl; 

      if (name.find("FPS") != std::string::npos)
        _frameRate = atof(values.c_str());

      if (mtc == "setup")
        continue; // dont need to store setup values
        
      CoreoEntry ce = { mtc, name, GetCoreoValues(values) };
      _coreoEntries.push_back(ce); 
    }
    //std::cout << "framerate=" << _frameRate << std::endl;
    _coreoEvent = _coreoEntries.begin();
  }


public:
  explicit CoreoSequencer(std::string const& coreo_file)
  : _coreoFile(coreo_file)
  , _frameRate(25.000)
  {
    PreprocessCoreoFile();
  }

  ~CoreoSequencer()
  {
  }
 
  // TODO: interpolation between (large) gaps in coreo events
  void UpdateControls(int framesize, int samplerate, int buffernr, CLAM::Network& net)
  {
    double currentTimeInMillisec = 
      ((buffernr * (double)framesize) / (double)samplerate) * 1000.;

    for (; _coreoEvent != _coreoEntries.end() &&
           SmpteToMs(_coreoEvent->mtc) <= currentTimeInMillisec;
           ++_coreoEvent) 
    {
      std::string const& controlName = _coreoEvent->name;
      //std::cout << "buffer=" << buffernr
      //          << " smpte=" << MsToSmpte(currentTimeInMillisec)
      //          << " controlname=" << control_name
      //          << " sample=" << buffernr * framesize;
      
      // check for wrong input
      if (!net.HasProcessing(controlName))
      {
        std::cout << "Error! Control named \'" << controlName 
                  << "\' not found in Network!" << std::endl;
        continue;
      }

      size_t size = _coreoEvent->values.size();
      //std::cout << " coreo_values_size=" << size;

      // check for wrong input
      if (size == 0)
      {
        std::cout << "Error!  No value found for timecode \'" 
                  << MsToSmpte(currentTimeInMillisec) << "\'"
                  << " with MTC path \'" << _coreoEvent->name << "\'"
                  << std::endl;
        continue;
      }

      CLAM::Processing& control = net.GetProcessing(controlName);
      CLAM::ControlSource* controlSource = dynamic_cast<CLAM::ControlSource*>(&control);
      const double& value = _coreoEvent->values[0];
      //std::cout << " value=" << value;

      // TODO: multicontrols
      controlSource->Do(value);

      //std::cout << std::endl;
    }

    //std::cout << "current_buffer=" << buffernr
    //          << " currentTimeInMillisec=" << currentTimeInMillisec
    //          << " sample=" << buffernr * framesize
    //          << std::endl;
  }
};

// End Of File

