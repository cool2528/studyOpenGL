project(opengl_Shaders)

file(GLOB_RECURSE SOURCE_LIST 
*.cpp
*.c
*.cxx
*.h
*.hpp
)


add_executable(${PROJECT_NAME} ${SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE 
${GLAD_PATH}
)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)