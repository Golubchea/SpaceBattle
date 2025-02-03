cmake_minimum_required(VERSION 3.21)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archives)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)


set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
#options
option (QES_UNIT_TESTING "Enable unit tests " ON)
set(QES_linkingType "SHARED" CACHE STRING "linkingType SHARED or STATIC")
set_property(CACHE QES_linkingType PROPERTY STRINGS SHARED STATIC)
add_compile_definitions(_USE_MATH_DEFINES)
#library

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE QES_SRC "src/*.cpp" "src/*.h" "include/*.h")
add_executable(${PROJECT_NAME} ${QES_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads Boost::system)


#testing
# if(QES_UNIT_TESTING)
#     enable_testing()

#     file(GLOB_RECURSE QES_TESTS "tests/*.cpp")
#     foreach(TEST_SOURCE ${QES_TESTS})
#         get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
#         add_executable(${TEST_NAME} ${TEST_SOURCE})
#         target_link_libraries(${TEST_NAME} PUBLIC ${PROJECT_NAME} GTest::gtest_main )

#         add_test(
#           NAME ${TEST_NAME}
#           COMMAND $<TARGET_FILE:${TEST_NAME}>
#         )

#     endforeach()
# endif()






#installs
export(TARGETS ${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
)

include(GNUInstallDirs) # provides CMAKE_INSTALL_* variables
install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

install(
    TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(EXPORT ${PROJECT_NAME}Config
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake"
    NAMESPACE ${PROJECT_NAME}::
)
