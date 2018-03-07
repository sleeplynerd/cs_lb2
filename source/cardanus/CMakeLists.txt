project(cardanus)
include_directories(${cardanus_SOURCE_DIR})
file(GLOB SOURCES "*.cpp" "*.hpp" "*.h")
add_library(cardanus ${SOURCES})
