# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

########################################################################################################################

include_directories( src/main/cpp/include )

#include_directories( SYSTEM /usr/include/node )

#include_directories( SYSTEM /media/rajitha/190GB/Ubuntu/chromium/v8/v8/include )
include_directories( SYSTEM ${CMAKE_SOURCE_DIR}/imported-lib/include)
#link_directories( SYSTEM /media/rajitha/190GB/Ubuntu/chromium/v8/v8/include )

#include_directories( SYSTEM /home/rajitha/Documents/FYP/cpp/example/src/tq84 )

########################################################################################################################

add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp)

add_library(
        native-math
        SHARED
        src/main/cpp/agent_math.cpp)

add_library(
        native-activity
        SHARED
        src/main/cpp/native-activity.cpp)

########################################################################################################################

#add_library( imported-lib
#        SHARED
#        IMPORTED )
#set_target_properties(
#        imported-lib
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/imported-lib/libtq84.so )

#add_library( libv8
#        SHARED
#        IMPORTED )
#set_target_properties(
#        libv8
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8.cr.so
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicui18n.cr.so)

find_library(
        log-lib
        log)

#add_library( nodejs-lib SHARED IMPORTED)
#set_target_properties( nodejs-lib
#        PROPERTIES IMPORTED_LOCATION
#        /usr/share/code/libnode.so )

########################################################################################################################

#message( SEND_ERROR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
#message( SEND_ERROR ${ANDROID_ABI} )
#message( SEND_ERROR ${CMAKE_SOURCE_DIR} )
#message( SEND_ERROR ${ANDROID_NDK} )

target_link_libraries(
#        native-lib
        native-activity
#        native-math
#        imported-lib
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libbase.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libplatform.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicui18n.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicuuc.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libc++.cr.so
        ${log-lib})

target_link_libraries(
        native-lib
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libbase.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libplatform.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicui18n.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicuuc.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libc++.cr.so
        ${log-lib})
