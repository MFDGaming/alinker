cmake_minimum_required(VERSION 3.1)
project(alinker)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB SOURCES src/*.c)

if(${IS_ARM_BUILD})
    set(ANDROID_LINKER ANDROID_ARM_LINKER)
else()
    set(ANDROID_LINKER ANDROID_X86_LINKER)
endif()

add_library(alinker ${SOURCES})
target_include_directories(alinker PUBLIC include)
target_compile_definitions(alinker PRIVATE ${ANDROID_LINKER} LINKER_TEXT_BASE=0xB0001000 LINKER_AREA_SIZE=0x01000000 LINKER_DEBUG=0)
target_link_libraries(alinker rt bsd Threads::Threads)

if(${IS_ARM_BUILD})
    set_target_properties(alinker PROPERTIES COMPILE_FLAGS "-mthumb" LINK_FLAGS "-mthumb")
else()
    set_target_properties(alinker PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()