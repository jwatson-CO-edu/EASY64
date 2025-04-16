########## INIT ####################################################################################

cmake_minimum_required( VERSION 3.5 FATAL_ERROR )

project( CPluscal )



########## SETTINGS ################################################################################

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.." ) # Place the exec in the root directory
include( CMakePrintHelpers )



########## DEPENDENCIES ############################################################################

##### Packages #####

cmake_print_variables( PROJECT_SOURCE_DIR )

##### Local Libraries #####

add_library( CPC_LIB
             types_builtin.cpp 
             lexing_basic.cpp 
             interpreter.cpp )

target_link_libraries( CPC_LIB )



########## PROGRAMS ################################################################################

##### 01: CMake Test #####
set( EXECUTABLE_NAME 01_interpreter_test.out )
add_executable( ${EXECUTABLE_NAME} 01_interpreter_test-01.cpp )
target_link_libraries( ${EXECUTABLE_NAME} CPC_LIB )
