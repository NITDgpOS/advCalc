set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
include_directories(..)

find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)
add_executable(calc_qt calc.cpp main.cpp calc.h)
target_link_libraries(calc_qt Qt5::Widgets ${LIBS})
