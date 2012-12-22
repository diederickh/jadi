cmake_minimum_required(VERSION 2.8)

set(demo_name "demo")

set(jadi_base_dir ${CMAKE_CURRENT_LIST_DIR}/../../../jadi/)
set(jadi_inc_dir ${jadi_base_dir}/include/)
set(jadi_src_dir ${jadi_base_dir}/src/)
set(jadi_lib_dir ${jadi_base_dir}/lib/)

set(demo_base_dir ${CMAKE_CURRENT_LIST_DIR}/../)
set(demo_src_dir ${demo_base_dir}src/)
set(demo_inc_dir ${demo_base_dir}src/)
set(demo_install_dir ${demo_base_dir}bin/)

file(GLOB_RECURSE jadi_cpp_files ${jadi_src_dir}*.cpp)
file(GLOB_RECURSE demo_cpp_files ${demo_src_dir}*.cpp)

if(APPLE) 
  set(JADI_PLATFORM_MAC TRUE)
  set(jadi_defines "-DJADI_PLATFORM=JADI_OSX")
  set(CMAKE_EXE_LINKER_FLAGS " -framework CoreFoundation -framework OpenGL -framework Cocoa -framework IOKit ")
endif(APPLE)

if(UNIX AND NOT APPLE)
  set(JADI_PLATFORM_LINUX TRUE)
  set(jadi_defines "-DJADI_PLATFORM=JADI_LINUX")
endif(UNIX AND NOT APPLE)

if(WIN32)
  set(JADI_PLATFORM_WIN TRUE)
  set(jadi_defines "-DJADI_PLATFORM=3")
endif(WIN32)

set(jadi_include_dirs
  ${jadi_inc_dir}
  ${demo_inc_dir}
)

set(jadi_source_files
  ${jadi_cpp_files}
  ${demo_cpp_files}
)

if(JADI_PLATFORM_MAC)
  set(jadi_libs 
    ${jadi_lib_dir}mac/libGLEW.a
    ${jadi_lib_dir}mac/libglfw3.a
    )
endif(JADI_PLATFORM_MAC)

if(JADI_PLATFORM_LINUX)
  set(jadi_libs 
    ${jadi_lib_dir}linux/libGLEW.a
    ${jadi_lib_dir}linux/libglfw3.a
    GL
    X11
    Xxf86vm
    rt
    pthread
)
endif(JADI_PLATFORM_LINUX)

if(JADI_PLATFORM_WIN)
endif(JADI_PLATFORM_WIN)

include_directories(
  ${jadi_include_dirs}
)

if(JADI_PLATFORM_MAC OR JADI_PLATFORM_LINUX)
  add_definitions(
    ${jadi_defines}
    "-Wno-invalid-offsetof"
    "-Wreturn-type"
    "-Werror=return-type"
    "-ggdb"
  )
endif(JADI_PLATFORM_MAC OR JADI_PLATFORM_LINUX)

add_executable(${demo_name} ${jadi_source_files})
install(TARGETS ${demo_name} DESTINATION ${demo_install_dir})
target_link_libraries(${demo_name} ${jadi_libs})