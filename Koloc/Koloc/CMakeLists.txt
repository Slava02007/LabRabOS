# CMakeList.txt: проект CMake для Koloc; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Koloc "Koloc.cpp" "Koloc.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Koloc PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
