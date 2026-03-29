cmake_minimum_required(VERSION 3.20)
project(core)

set(CMAKE_CXX_STANDARD 20)


# Find JNI
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# Build shared library
add_library(core SHARED
        main.cpp
        core/coconut.cpp
        core/coconut.h
        core/checks/static/static_check.h
        core/checks/static/impl/known/weedhack.h
        javahooks/javahooks.cpp
        javahooks/javahooks.h
        javahooks/impl/processbuilder.h
)



target_link_libraries(core PRIVATE
        ${JNI_LIBRARIES}
        -static-libgcc
        -static-libstdc++
        -Wl,-Bstatic,--whole-archive
        -lpthread
        -Wl,-Bdynamic,--no-whole-archive
)

set_target_properties(core PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin"
)