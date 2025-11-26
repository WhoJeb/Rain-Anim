cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

# Set to true when shipping
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

# Statically link std lib functions so it doesn't need system .dll cuz it breaks on windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

if(PRODUCTION_BUILD)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # link time optimization
else()
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE) 
endif()

project(rainAnim)

# Define MY_SOURCES to be a list of all the source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE MY_INCLUDES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1)

if(PRODUCTION_BUILD)
  # Setup the ASSETS_PATH macro to be in the root folder of your exe
  target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

  # Remove the option to debug asserts.
  target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)
else()
  # Adds macro RESOURCES_PATH, use whenever reading or writing to a file in the resorces folder
  target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
  target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} ${MY_INCLUDES} )

find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED) # Or SDL2
# find_package(GLEW REQUIRED) # Or GLAD
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(GLM REQUIRED glm)


target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    OpenGL::GL
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLM_LIBRARIES}
)

# target_link_libraries(test OpenGL::GL glfw GLEW) # Or SDL2, GLAD

message(STATUS "Sources: ${MY_SOURCES}")
message(STATUS "Includes: ${MY_INCLUDES}")
