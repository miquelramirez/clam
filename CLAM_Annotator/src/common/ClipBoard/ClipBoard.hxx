#ifndef CLIPBOARD_HXX
#define CLIPBOARD_HXX

#include "Song.hxx"
#include <vector>

class ClipBoard : public std::vector<Song*>
{
public:
  static ClipBoard * getInstance();
protected:
  ClipBoard()
    {
    }
};

#endif

