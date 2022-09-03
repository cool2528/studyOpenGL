project(opengl_Triangles)

file(GLOB_RECURSE SOURCE_LIST 
*.cpp
*.cxx
*.h
*.hpp
)

message(DEBUG ${SOURCE_LIST})
add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)