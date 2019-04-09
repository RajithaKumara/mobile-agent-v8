# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

########################################################################################################################

include_directories( SYSTEM ${CMAKE_SOURCE_DIR}/imported-lib/include/node)

########################################################################################################################

add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp)

find_library(
        log-lib
        log)

########################################################################################################################

#message( SEND_ERROR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
#message( SEND_ERROR ${ANDROID_ABI} )
#message( SEND_ERROR ${CMAKE_SOURCE_DIR} )
#message( SEND_ERROR ${ANDROID_NDK} )

target_link_libraries(
        native-lib
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libnode.so
        ${log-lib})
