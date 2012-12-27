#import <Jadi/Image.h>

Image::Image() :
pixels(NULL)
{}

bool Image::loadJPEG(const std::string& filepath) {
  struct jpeg_decompress_struct cinfo; // compression info
  struct jpeg_error_mgr jerr; // error manager

  cinfo.err = jpeg_std_error(&jerr); // set error manager
  jpeg_create_decompress(&cinfo); // create decompression
  
  FILE* infile;

	if ((infile = fopen(filepath.c_str(), "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filepath.c_str());
    return false;
	}
  
  // Need to add error checking
//  /* Establish the setjmp return context for my_error_exit to use. */
//  if (setjmp(jerr.setjmp_buffer)) {
//    jpeg_destroy_decompress(&cinfo);
//    fclose(infile);
//    return false;
//  }
  
	jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  
  width = cinfo.output_width;
  height = cinfo.output_height;
  numChannels = cinfo.output_components;
  
  printf("%d, %d, %d\n", width, height, numChannels);

  int row_stride = cinfo.output_width * cinfo.output_components;
  JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  
  pixels = new unsigned char[width*height*numChannels];
  
  unsigned long dx = 0;
  
  while (cinfo.output_scanline < cinfo.output_height) {
//    printf( "%i, %i\n", cinfo.output_scanline, cinfo.output_height);
    jpeg_read_scanlines(&cinfo, buffer, 1);
    memcpy(pixels+dx, buffer[0], row_stride);
    dx += row_stride;
  }
  
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  
  return true;
}

int Image::getWidth() {
  return width;
}

int Image::getHeight() {
  return height;
}

int Image::getNumChannels() {
  return numChannels;
}

unsigned char* Image::getPixels() {
  return pixels;
}
