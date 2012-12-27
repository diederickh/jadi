#include <string>
#include <setjmp.h>
#include <string.h>

#include "images/jpeglib.h"
#include "images/jerror.h"

class Image {
public:
  Image();
  bool loadJPEG(const std::string& filepath);
  int getWidth();
  int getHeight();
  int getNumChannels();
  unsigned char* getPixels();
  
protected:
  unsigned char *pixels;
  int width;
  int height;
  int numChannels;
};
