// JADI FUNCTIONS

#ifndef JADI_IMAGE_H
#define JADI_IMAGE_H

#define STBI_HEADER_FILE_ONLY
#include <jadi/Image.c>
#include <string>

class Image {
public:
  Image();
  ~Image();
  bool load(const std::string& filepath);
  int getWidth();
  int  getHeight();
  int  getNumChannels();
  unsigned char* getPixels();
  
protected:
  unsigned char *pixels;
  int  width;
  int  height;
  int  numChannels;
};

inline Image::Image()
: pixels(NULL), width(0), height(0), numChannels(0)
{}

inline Image::~Image() {
  stbi_image_free(pixels);
}

inline bool Image::load(const std::string& filepath) {
  pixels = stbi_load(filepath.c_str(), &width, &height, &numChannels, 0);

  if (pixels==NULL) {
      fprintf(stderr, "Image::load(): image data is NULL");
      return false;
  } else {
      return true;
  }
}

inline int Image::getWidth() {
  return width;
}

inline int Image::getHeight() {
  return height;
}

inline int Image::getNumChannels() {
  return numChannels;
}

inline unsigned char* Image::getPixels() {
  return pixels;
}

#endif