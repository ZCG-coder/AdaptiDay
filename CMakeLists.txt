CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(AdaptiDay VERSION 1.0 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Freetype)

IF (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    ADD_COMPILE_DEFINITIONS(LINUX)
    SET(LINUX TRUE CACHE INTERNAL "OS is Linux")
ELSEIF (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    ADD_COMPILE_DEFINITIONS(WINDOWS)
    SET(WINDOWS TRUE CACHE INTERNAL "OS is Windows")
ELSEIF (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    ADD_COMPILE_DEFINITIONS(MACOSX)
    SET(MACOSX TRUE CACHE INTERNAL "OS is MacOS")
ENDIF()

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_COMPILE_DEFINITIONS(DEBUG)
    SET(DEBUG TRUE CACHE INTERNAL "Build type is Debug")
    SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
ELSEIF (CMAKE_BUILD_TYPE STREQUAL "Release")
    ADD_COMPILE_DEFINITIONS(RELEASE)
    SET(RELEASE TRUE CACHE INTERNAL "Build type is Release")
ENDIF()

SET(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Base directory")

SET(SOURCES main.cpp)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)

ADD_LIBRARY(imgui STATIC ${BASE_DIR}/imgui/imgui.cpp
                         ${BASE_DIR}/imgui/imgui_draw.cpp
                         ${BASE_DIR}/imgui/imgui_widgets.cpp
                         ${BASE_DIR}/imgui/imgui_tables.cpp
                         ${BASE_DIR}/imgui/imgui_multicontext_compositor.cpp
                         ${BASE_DIR}/imgui/backends/imgui_impl_sdl2.cpp
                         ${BASE_DIR}/imgui/backends/imgui_impl_opengl3.cpp)

ADD_EXECUTABLE(AdaptiDay ${SOURCES})
TARGET_LINK_LIBRARIES(imgui PRIVATE
                      ${SDL2_LIBRARIES}
                      ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(AdaptiDay PRIVATE imgui)
IF (MACOSX)
    TARGET_LINK_LIBRARIES(AdaptiDay PRIVATE "-framework CoreFoundation")
ENDIF()
TARGET_INCLUDE_DIRECTORIES(AdaptiDay PRIVATE include
                           ${SDL2_INCLUDE_DIRS}
                           ${OPENGL_INCLUDE_DIRS}
                           ${BASE_DIR}/imgui)
TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE include
                           ${SDL2_INCLUDE_DIRS}
                           ${OPENGL_INCLUDE_DIRS}
                           ${BASE_DIR}/imgui)

# If we have freetype, we can use it to render text
FIND_PACKAGE(Freetype QUIET)
IF (FREETYPE_FOUND)
    MESSAGE(STATUS "Found Freetype")
    ADD_LIBRARY(imgui_freetype STATIC "${BASE_DIR}/imgui/misc/freetype/imgui_freetype.cpp")
    TARGET_INCLUDE_DIRECTORIES(imgui_freetype PRIVATE "${BASE_DIR}/imgui")
    TARGET_LINK_LIBRARIES(imgui_freetype PRIVATE ${FREETYPE_LIBRARIES})
    TARGET_LINK_LIBRARIES(imgui PRIVATE imgui_freetype)
    TARGET_INCLUDE_DIRECTORIES(imgui_freetype PRIVATE ${FREETYPE_INCLUDE_DIRS})
    TARGET_COMPILE_DEFINITIONS(imgui PRIVATE IMGUI_ENABLE_FREETYPE)
    TARGET_COMPILE_DEFINITIONS(AdaptiDay PRIVATE HAS_FREETYPE)
ENDIF()
