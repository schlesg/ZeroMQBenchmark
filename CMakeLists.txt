cmake_minimum_required(VERSION 3.5)
project(ZeroMQBenchmark )

set(CMAKE_CXX_STANDARD 11)

SET(GCC_COVERAGE_LINK_FLAGS    "-lboost_ program_options")
# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

## load in pkg-config support
find_package(PkgConfig)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

add_executable(ZMQPub hwclient.cpp)
add_executable(ZMQSub hwserver.cpp)

add_executable(ZMQEchoer echoer.cpp)
add_executable(ZMQInitiator initiator.cpp)

# set(hwserver_SOURCE_FILES hwserver.cpp)
# add_executable(hwserver ${hwserver_SOURCE_FILES})

## at the 0mq library to our link directive
target_link_libraries(ZMQPub PUBLIC ${ZeroMQ_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_DATE_TIME_LIBRARY} nlohmann_json ${Boost_LIBRARIES})
target_link_libraries(ZMQSub PUBLIC ${ZeroMQ_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_DATE_TIME_LIBRARY} nlohmann_json ${Boost_LIBRARIES})
target_link_libraries(ZMQEchoer PUBLIC ${ZeroMQ_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_DATE_TIME_LIBRARY} nlohmann_json ${Boost_LIBRARIES})
target_link_libraries(ZMQInitiator PUBLIC ${ZeroMQ_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_DATE_TIME_LIBRARY} nlohmann_json ${Boost_LIBRARIES})

## add the include directory to our compile directives
# target_include_directories(hwclient PUBLIC "./")

target_include_directories(ZMQPub PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(ZMQSub PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(ZMQEchoer PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(ZMQInitiator PUBLIC ${ZeroMQ_INCLUDE_DIR})

