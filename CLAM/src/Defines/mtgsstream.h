// -*- c++ -*-
//
// Copyright (C) 1999-2000 by Roger Rene Kommer, artefaktur
// Projekt: ACDK
// 
// $Header$
//
// $Log$
// Revision 1.1  2002/09/23 16:55:00  xamat
// Initial revision
//
// Revision 1.2  2002/01/07 17:05:00  erobledo
// Some fixes to allow compilation with g++-2.95.x:
// * Removed const qualyfication from some methods.
// * Added str setters.
// * Added char* insertion operators, to increase speed.
// * Added qualyfication of parent operators invocation, to avoid
//   infinite recursion in some circunstances.
//
// Revision 1.1  2001/12/13 22:21:22  dgarcia
// Added as a patch to solve some gcc 2.95 misscomplianties on sstream
//
// Revision 1.1.1.1  2000/12/11 18:05:18  kommer
// ACDK Free edition
//
// Revision 1.1.1.1  2000/11/23 09:53:26  roger
// initial release
//
// Revision 1.1.1.1  2000/11/22 13:35:19  roger
// initial acdk sources
//
// Revision 1.1  2000/04/14 09:09:38  roger
// *** empty log message ***
//
// Revision 1.3  1999/10/21 18:04:32  roger
// Copyright notice updated
//
// Revision 1.2  1999/10/04 08:10:26  roger
// renamed M\MLib => ACDK
//
// Revision 1.1  1999/08/12 19:07:39  roger
// for gnuc-compatibility
//
// Revision 1.2  1999/08/12 13:52:21  kai
// Dos2Unix Konvertierung.
//
// Revision 1.1.1.1  1999/08/05 11:15:01  roger
// Inital unstable snapshot
//

/*
Some compiler doesn't have new 
<sstream>
emulating it, with old 
strstream.h
*/
#ifndef stringstream_h
#define stringstream_h

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

