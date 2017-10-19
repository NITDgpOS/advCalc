set(CMAKE_CXX_STANDARD 14)
set(TARGET calc)

# message(STATUS "some ${CMAKE_SOURCE_DIR}")
# message(STATUS "some ${PROJECT_SOURCE_DIR}")

find_package(Readline REQUIRED)

set(LIBS ${LIBS} ${Readline_LIBRARIES})

add_executable(${TARGET} main.cpp input_bindings.cpp)
target_link_libraries(${TARGET} ${LIBS})
