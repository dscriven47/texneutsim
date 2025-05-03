cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(TexNeutSim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

set(PROJECT_SOURCE_DIR_GEANT ${PROJECT_SOURCE_DIR}/src/geant/)

file(GLOB sources ${PROJECT_SOURCE_DIR_GEANT}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR_GEANT}/*.hh)

file(GLOB MACRO_FILES "*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB MACRO_FILES "*.dat")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(texneutsim ${PROJECT_SOURCE_DIR_GEANT}/texneutsim.cc ${sources} ${headers})
target_link_libraries(texneutsim ${Geant4_LIBRARIES})

add_custom_target(TexNeutSimu DEPENDS sim)
