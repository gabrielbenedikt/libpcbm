cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
   project(libpcbmotor VERSION 0.1 DESCRIPTION "C++ PCBMotor rotation mount librarty")

   set(CMAKE_BUILD_TYPE RELEASE)
   
   if(NOT CMAKE_DEBUG_POSTFIX)
      set(CMAKE_DEBUG_POSTFIX d)
   endif()
   if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set(CMAKE_C_FLAGS "-Wall -Wextra -fPIC")
      set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb -fPIC")
      set(CMAKE_C_FLAGS_RELEASE "-Werror -pipe -g -O3 -fPIC -march=native -fomit-frame-pointer -fno-signaling-nans -fno-trapping-math -ftree-vectorize -fopt-info-vec-missed=vec.opt")
      set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")
      set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -fPIC")
      set(CMAKE_CXX_FLAGS_RELEASE "-Werror -pipe -g -O3 -fPIC -march=native -fomit-frame-pointer -fno-signaling-nans -fno-trapping-math -ftree-vectorize -fopt-info-vec-missed=vec.opt")
   elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
      set(CMAKE_C_FLAGS "-Wall -Wextra -fPIC")
      set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb -fPIC")
      set(CMAKE_C_FLAGS_RELEASE "-Werror -pipe -g -O3 -fPIC -march=native -ipo -fomit-frame-pointer -fno-signaling-nans -fno-trapping-math -ftree-vectorize")
      set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")
      set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -fPIC")
      set(CMAKE_CXX_FLAGS_RELEASE "-Werror -pipe -g -O3 -fPIC -march=native -ipo -fomit-frame-pointer -fno-signaling-nans -fno-trapping-math -ftree-vectorize")
   endif()
   
   find_package(Boost 1.70 REQUIRED COMPONENTS system)

   add_library(libpcbmotor SHARED src/pcbm.cpp src/pcbm_comm.cpp)
   
   set_target_properties(libpcbmotor PROPERTIES VERSION ${PROJECT_VERSION})
   set_target_properties(libpcbmotor PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
   set_target_properties(libpcbmotor PROPERTIES PUBLIC_HEADER include/pcbmotor.h)

   target_include_directories(libpcbmotor PRIVATE include)
   target_include_directories(libpcbmotor PRIVATE src)
   
   
   include(GNUInstallDirs)
   
   install(TARGETS libpcbmotor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    configure_file(pcbmotor.pc.in pcbmotor.pc @ONLY)
   
   set_property(TARGET libpcbmotor PROPERTY CXX_STANDARD 20)
   
   install(FILES ${CMAKE_BINARY_DIR}/pcbmotor.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
   
   target_link_libraries(libpcbmotor ${Boost_LIBRARIES})
   
   include(CheckIPOSupported)
   check_ipo_supported(RESULT hasipo OUTPUT error)
   if( hasipo )
      message(STATUS "IPO / LTO enabled")
      set_property(TARGET libpcbmotor PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
   else()
      message(STATUS "IPO / LTO not supported: <${error}>")
   endif()
