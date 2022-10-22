cmake_minimum_required(VERSION 3.23)
project(Image_Processing_APIs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)



enable_testing()


add_executable(prueba.exe Test.cpp
        Processing_APIS.cpp
        Processing_APIS.h)
target_link_libraries(prueba.exe ${OpenCV_LIBS} )


add_executable(server.exe  Server.cpp
        Processing_APIS.cpp
        Processing_APIS.h)
target_link_libraries(server.exe ${OpenCV_LIBS})


add_executable(serverTest.exe  ServerTest.cpp
        Processing_APIS.cpp
        Processing_APIS.h)
target_link_libraries(serverTest.exe ${OpenCV_LIBS}  GTest::gtest_main )

add_executable(cliente.exe Client.cpp)
target_link_libraries(cliente.exe ${OpenCV_LIBS})


