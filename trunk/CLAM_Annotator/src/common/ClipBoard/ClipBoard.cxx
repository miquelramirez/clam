#include "ClipBoard.hxx"

ClipBoard* ClipBoard::getInstance()
{
  static ClipBoard clipboard;
  return &clipboard;
}
