#ifndef JADI_UTILS_H
#define JADI_UTILS_H

#include <assert.h>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <float.h>
#include <jadi/Jadi.h>
#include <jadi/OBJ.h>
#include <jadi/Image.h>

#if JADI_PLATFORM == JADI_WIN
#include <Windows.h>
/*
// From libshout 2.3.1
#ifdef _MSC_VER
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef __int32 int32_t;
typedef int  ssize_t;
#endif
 */
#endif

#if JADI_PLATFORM == JADI_OSX
#include <libgen.h> /* dirname */
#include <CoreFoundation/CFRunLoop.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach-o/dyld.h> /* _NSGetExecutablePath */
#include <sys/resource.h>
#include <sys/sysctl.h>
#include <unistd.h>  /* sysconf */
#endif

#if JADI_PLATFORM == JADI_LINUX 
#include <unistd.h> /* readlink */
#include <sys/time.h> /* timeofday */
#include <libgen.h> /* dirname */
#define MAX_PATH 4096
#endif


#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI 6.28318530717958647693
#endif

#ifndef M_TWO_PI
#define M_TWO_PI 6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#define HALF_PI 1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#define CLAMP(val,min,max) (MAX(MIN(val,max),min))
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef DX
#define DX(i,j,w)((j)*(w))+(i)
#endif

// as described in: "From Quaternion to Matrix and Back", J.M.P. van Waveren, 27th feb. 2005, id software
static float fast_sqrt(float x) {
  long i; 
  float y, r; 
  y = x * 0.5f; 
  i = *(long *)( &x ); 
  i = 0x5f3759df - ( i >> 1 ); 
  r = *(float *)( &i ); 
  r = r * ( 1.5f - r * r * y ); 
  return r; 
}
/*
static std::string get_data_path() {

}
*/

#if JADI_PLATFORM == JADI_WIN
// -------------------------------------- WIN ---------------------------------------
static std::string get_exe_path() {
  char buffer[MAX_PATH];

  // Try to get the executable path with a buffer of MAX_PATH characters.
  DWORD result = ::GetModuleFileNameA(nullptr, buffer, static_cast<DWORD>(MAX_PATH));
  if (result) {
    return "";
  }

  std::string::size_type pos = std::string(buffer).find_last_of( "\\/" );
  return std::string(buffer).substr(0, pos);
}

static uint64_t millis(void) {
  static LARGE_INTEGER s_frequency;
  static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
  if (s_use_qpc) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (1000LL * now.QuadPart) / s_frequency.QuadPart;
  } else {
    return GetTickCount();
  }
}
// -------------------------------------- WIN ---------------------------------------
#endif


#if JADI_PLATFORM == JADI_LINUX 
// -------------------------------------- LINUX -------------------------------------
static std::string get_exe_path() {
  char buffer[MAX_PATH];
  size_t size = MAX_PATH;
  ssize_t n = readlink("/proc/self/exe", buffer, size - 1);
  if (n <= 0) {
    return "";
  }
  buffer[n] = '\0';


  const char* dn = dirname(buffer);
  size = strlen(dn);
  std::string ret(dn, size) ;
  ret.push_back('/');
  return ret;
}

static uint64_t millis() {
  timeval time;
  gettimeofday(&time, NULL);
  uint64_t n = time.tv_usec;
  n /= 1000; // convert seconds to millis
  n += (time.tv_sec * 1000); // convert micros to millis
  return n;
}

// -------------------------------------- LINUX -------------------------------------
#endif

#if JADI_PLATFORM == JADI_OSX 
// -------------------------------------- OSX ---------------------------------------
static std::string get_exe_path() {
  char buffer[1024];
  uint32_t usize = sizeof(buffer);;

  int result;
  char* path;
  char* fullpath;

  result = _NSGetExecutablePath(buffer, &usize);
  if (result) {
    return "";
  }

  path = (char*)malloc(2 * PATH_MAX);
  fullpath = realpath(buffer, path);

  if (fullpath == NULL) {
    free(path);
    return "";
  }

  strncpy(buffer, fullpath, usize);
  free(fullpath);
  const char* dn = dirname(buffer);
  usize = strlen(dn);
  std::string ret(dn, usize) ;
  ret.push_back('/');
  return ret;
}

static uint64_t millis(void) {
  mach_timebase_info_data_t info;
  if (mach_timebase_info(&info) != KERN_SUCCESS) {
    abort();
  }
  return (mach_absolute_time() * info.numer / info.denom) / 1000000;
}
// -------------------------------------- OSX ---------------------------------------
#endif


static std::string to_data_path(const std::string filename) {
  std::string exepath = get_exe_path();
  exepath += "data/" +filename;
  return exepath;
}

// Simple wrapper which loads an object file from the data path, returns number of vertices
static size_t load_obj_file(const std::string& filename, float** result, bool useNormals, bool useTexCoords=false) {
  OBJ obj;
  if(!obj.load(to_data_path(filename))) {
    *result = NULL;
    return 0;
  }
  return obj.getVertices(result, useNormals, useTexCoords);
}

static float random(float max) {
  return max * rand() / (RAND_MAX + 1.0f);
}

static float random(float x, float y) {
  float high = 0;
  float low = 0;
  float result = 0;
	
  high = std::max<float>(x,y);
  low = std::min<float>(x,y);
  result = low + ((high-low) * rand()/(RAND_MAX + 1.0));
  return result;
}

#endif 
