project(d3d8_wrapper)

include(FetchContent)

# Fetch min-dx8-sdk for headers (used for interface definitions)
FetchContent_Declare(
    dx8
    GIT_REPOSITORY https://github.com/TheSuperHackers/min-dx8-sdk.git
    GIT_TAG        7bddff8c01f5fb931c3cb73d4aa8e66d303d97bc
)
FetchContent_GetProperties(dx8)
if(NOT dx8_POPULATED)
    FetchContent_Populate(dx8)
endif()

# Add source files
file(GLOB_RECURSE DX8WRAPPER_SRC 
    "Source/*.cpp"
    "Include/*.h"
)

add_library(d3d8_wrapper STATIC ${DX8WRAPPER_SRC})

# Includes
target_include_directories(d3d8_wrapper PUBLIC
    ${dx8_SOURCE_DIR}
    Include
)

# Link against Android OpenGL ES libraries
target_link_libraries(d3d8_wrapper PRIVATE
    EGL
    GLESv3
    android
    log
)

# Define d3d8 aliases for compatibility with existing game code
add_library(d3d8 ALIAS d3d8_wrapper)
add_library(d3d8lib ALIAS d3d8_wrapper) # Used by Core/GameEngineDevice
add_library(d3dx8 ALIAS d3d8_wrapper)   # Used by Core/GameEngineDevice
