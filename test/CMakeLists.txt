add_definitions(-D_VARIADIC_MAX=10)
add_definitions(-D_DEBUG)
add_definitions(-D_MT)
add_definitions(-D_DLL)

file(GLOB SOURCES "*.cpp")
add_executable(test_cardanus
    ${SOURCES}
)

add_subdirectory(googletest)
target_link_libraries(test_cardanus cardanus gtest gtest_main)

