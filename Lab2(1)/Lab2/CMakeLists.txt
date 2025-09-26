# CMakeList.txt: проект CMake для Lab2(1); включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CMakeTarget "Lab2(1).cpp" "Lab2(1).h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries(CMakeTarget PRIVATE arrayfunctions)

target_include_directories(CMakeTarget PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
