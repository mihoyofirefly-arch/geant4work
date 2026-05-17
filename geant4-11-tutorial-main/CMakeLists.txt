cmake_minimum_required(VERSION 3.2 FATAL_ERROR) # CMakeLists.txt for Geant4 example

project(Tutorial)# Name of the project

find_package(Geant4 REQUIRED ui_all vis_all)# Find Geant4 package with all UI and visualization components

include(${Geant4_USE_FILE})# Include the Geant4 use file to set up necessary variables and configurations

include_directories(${PROJECT_SOURCE_DIR}/include)# Include the project's include directory

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)# Get all source files in the src directory

file(GLOB MACRO_FILES "macros/*.mac")# Get all macro files in the macros directory
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)# Copy the macro files to the build directory

add_executable(sim sim.cc ${sources})#Create an executable named sim from the sim.cc file and all source files
target_link_libraries(sim ${Geant4_LIBRARIES})# Link the sim executable with the Geant4 libraries

add_custom_target(Tutorial DEPENDS sim)# Create a custom target named Tutorial that depends on the sim executable
