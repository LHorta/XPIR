cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")

set(BUILD_CPR_TESTS OFF CACHE BOOL "Don't build cpr tests")
find_package(CURL)
if (CURL_FOUND)
    set(USE_SYSTEM_CURL ON CACHE BOOL "Use the system curl for faster builds")
endif ()
add_subdirectory(cpr)

include_directories(..)
include_directories(../..)
include_directories(${CPR_INCLUDE_DIRS})

add_executable(pir_client DESC.cpp PIRClient.cpp PIRClientLog.cpp PIRController.cpp PIRView.cpp PIRViewCLI.cpp main.cpp fcm.cpp fcm.hpp)

target_link_libraries(pir_client pir_client_optim pir pthread ${MPFR_LIBRARIES} ${Boost_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES} ${CPR_LIBRARIES})
