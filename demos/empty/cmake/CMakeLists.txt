cmake_minimum_required(VERSION 2.8)

set(demo_name "demo")

set(jadi_base_dir ${CMAKE_CURRENT_LIST_DIR}/../../../jadi/)
set(jadi_inc_dir ${jadi_base_dir}/include/)
set(jadi_src_dir ${jadi_base_dir}/src/)
set(demo_base_dir ${CMAKE_CURRENT_LIST_DIR}/../)
set(demo_src_dir ${demo_base_dir}src/)
set(demo_inc_dir ${demo_base_dir}src/)
set(demo_install_dir ${demo_base_dir}bin/)

file(GLOB_RECURSE jadi_cpp_files ${jadi_src_dir}*.cpp)
file(GLOB_RECURSE demo_cpp_files ${demo_src_dir}*.cpp)

set(jadi_include_dirs
  ${jadi_inc_dir}
  ${demo_inc_dir}
)

set(jadi_source_files
  ${jadi_cpp_files}
  ${demo_src_dir}/main.cpp
  ${demo_src_dir}/Demo.cpp
)

if(APPLE) 
  set(jadi_defines "-DJADI_PLATFORM=JADI_OSX")
  set(CMAKE_EXE_LINKER_FLAGS " -framework CoreFoundation -framework OpenGL -framework Cocoa -framework IOKit ")
endif(APPLE)

if(UNIX AND NOT APPLE)
  set(jadi_defines "-DJADI_PLATFORM=JADI_LINUX")
  #set(CMAKE_EXE_LINKER_FLAGS 
endif(UNIX AND NOT APPLE)

include_directories(
  ${jadi_include_dirs}
)

add_definitions(
  ${jadi_defines}
  "-Wno-invalid-offsetof"
  "-Wreturn-type"
  "-Werror=return-type"
)

add_executable(${demo_name} ${jadi_source_files})
install(TARGETS ${demo_name} DESTINATION ${demo_install_dir})