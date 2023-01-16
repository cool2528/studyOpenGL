
set(GLAD_PATH ${CMAKE_SOURCE_DIR}/glad)
add_compile_options("/utf-8")
add_subdirectory(triangles)
add_subdirectory(window)
add_subdirectory(rectangle)
add_subdirectory(shaders)