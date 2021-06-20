find_package(asio REQUIRED)

# Add source files
set(SOURCE_FILES main.cpp)

# message("CXX id: ${CMAKE_CXX_COMPILER_ID}")
# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
#    set(SOURCE_FILES ${SOURCE_FILES} sample.rc)
endif(WIN32)

# the above paths and defines will be used in this build
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCE_FILES})
# set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/$<CONFIG>)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "/usr/local/include")
if(asio_FOUND)
    message("asio found: ${asio_INCLUDE_DIR}")
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE asio asio::asio)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${asio_INCLUDE_DIR})
else()
    message("asio not found")
endif()

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/lib"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/lib"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/bin"
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)