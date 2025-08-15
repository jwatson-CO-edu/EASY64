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
             primitives.cpp 
             lexer.cpp 
             parser.cpp 
             interpreter.cpp )

target_link_libraries( CPC_LIB )



########## PROGRAMS ################################################################################

##### 00: Lexer Test #####
set( EXECUTABLE_NAME 00_tokenize-file.out )
add_executable( ${EXECUTABLE_NAME} 00_tokenize-file.cpp )
target_link_libraries( ${EXECUTABLE_NAME} CPC_LIB )

##### 01: Math Test #####
set( EXECUTABLE_NAME 01_math-ops.out )
add_executable( ${EXECUTABLE_NAME} 01_math-ops.cpp )
target_link_libraries( ${EXECUTABLE_NAME} CPC_LIB )


