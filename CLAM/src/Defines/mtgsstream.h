/*
Some compiler doesn't have new 
<sstream>
emulating it, with old 
strstream.h
*/
#ifndef stringstream_h
#define stringstream_h
#warning Including mtgsstream.h which is depracated, use <sstream> instead

#ifdef HAVE_STANDARD_SSTREAM
#include <sstream>
/*
/// @todo the following should be in config.h.in
#ifndef HAVE_STANDARD_SSTREAM_STR
inline std::strstream& operator << (std::strstream& out,const std::string& in)
{
	out << in.c_str();
	return out;
}
#endif
*/
#else
#include <strstream.h>
#include <string>
#warning You are using a wrapper stringstream

namespace std {

class stringstream 
  : public strstream 
{
public :
  stringstream() : strstream() { }
	/** @todo Check that this behaves as the standard constructor. */
  stringstream(const char* str) 
  { 
    operator<<(str);
  }
	/** @todo Check that this behaves as the standard constructor. */
  stringstream(const std::string &str) 
  { 
    operator<<(str.c_str());
  }
  ~stringstream() 
  {
    rdbuf()->freeze(false);
  }
  inline void unsureTerminated()
  {
    char* ptr = rdbuf()->str();
    if (ptr[strstream::pcount() - 1] != '\0') {
      rdbuf()->freeze(false);
      rdbuf()->sputc(0);
    } else
      rdbuf()->freeze(false);
  }
  inline const char* c_str()
  {
    unsureTerminated();
    char* ptr = rdbuf()->str();
    rdbuf()->freeze(false);
    return ptr;
  }
  inline const char* data()
  {
    const char *ptr = rdbuf()->str();
    rdbuf()->freeze(false);
    return ptr;
  }
  inline string str()
  {
    return string(data(), strstream::pcount());
  }
	/** @todo Check that this behaves as the standard setter */
  inline void str(const char* str)
  {
	  seekg(0);
	  operator<<(str);
  }
	/** @todo Check that this behaves as the standard setter */
  inline void str(const std::string &str)
  {
	  seekg(0);
	  operator<<(str.c_str());
  }
  stringstream& operator<<(const char* str)
  {
    this->strstream::operator<<(str);
    return *this;
  }
  stringstream& operator<<(const std::string& str)
  {
    this->strstream::operator<<(str.c_str());
    return *this;
  }
  stringstream& operator<<(double d)
  {
    this->ostream::operator<<(d);
    return *this;
  }
};

} //std

#endif//HAVE_STANDARD_SSTREAM

#endif//stringstream_h

