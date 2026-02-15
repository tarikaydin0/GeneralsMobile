project(d3d8_wrapper)

# Add source files
file(GLOB_RECURSE DX8WRAPPER_SRC 
    "Source/*.cpp"
    "Include/*.h"
)

add_library(d3d8_wrapper STATIC ${DX8WRAPPER_SRC})

# Includes - use project's DirectX8 headers
target_include_directories(d3d8_wrapper PUBLIC
    ${CMAKE_SOURCE_DIR}/Dependencies/Generic/DirectX8/Include
    Include
)

# Link against Android OpenGL ES libraries
target_link_libraries(d3d8_wrapper PRIVATE
    EGL
    GLESv3
    android
    log
)

# Define d3d8 alias for compatibility
add_library(d3d8 ALIAS d3d8_wrapper)
add_library(d3d8lib ALIAS d3d8_wrapper) # For Core/GameEngineDevice usage
add_library(d3dx8 ALIAS d3d8_wrapper) # Also implement D3DX8 here or separate?
# Core/GameEngineDevice links d3dx8.
# D3DX8 is helper library. I might need a separate stub/impl for it.
# For now alias to wrapper and I'll put stubs there.
