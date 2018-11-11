cmake_minimum_required(VERSION 3.6)

project(queue_server VERSION 1.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

file(GLOB_RECURSE SOURCES *.cpp *.h)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/server)

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)