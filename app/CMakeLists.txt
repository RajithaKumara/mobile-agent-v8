# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

########################################################################################################################

include_directories( SYSTEM ${CMAKE_SOURCE_DIR}/imported-lib/include)

########################################################################################################################

add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp)

#add_library(
#        native-activity
#        SHARED
#        src/main/cpp/native-activity.cpp)

########################################################################################################################

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

########################################################################################################################

#message( SEND_ERROR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
#message( SEND_ERROR ${ANDROID_ABI} )
#message( SEND_ERROR ${CMAKE_SOURCE_DIR} )
#message( SEND_ERROR ${ANDROID_NDK} )

#target_link_libraries(
#        native-activity
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libbase.cr.so
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libplatform.cr.so
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8.cr.so
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicui18n.cr.so
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicuuc.cr.so
#        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libc++.cr.so
#        ${log-lib})

target_link_libraries(
        native-lib
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8_libbase.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libv8.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicui18n.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libicuuc.cr.so
        ${CMAKE_SOURCE_DIR}/imported-lib/${ANDROID_ABI}/libc++.cr.so
        ${log-lib})
