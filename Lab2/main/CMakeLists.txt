add_executable (CMakeTarget "Main.cpp" "Main.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries(CMakeTarget PRIVATE arrayfunctions)

target_include_directories(CMakeTarget PRIVATE ${CMAKE_SOURCE_DIR}/lib)
