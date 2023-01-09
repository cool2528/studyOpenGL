#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
using namespace std;
// 窗口大小改变回调函数
static void window_size_changed(GLFWwindow* window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}
// 处理用户输入
static void process_input(GLFWwindow* window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, true);
	}
}
int main(int argc, char **argv)
{
    // 初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //设置gl 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置gl 次版本号

    // 使用glfw 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800,
        600,
        "study_window",
        nullptr,
        nullptr);
    if(!window)
    {
        cout << "create gl window fail" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 初始化glad
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        cout << "init glad fail" << endl;
        return -1;
    }
    // 在渲染之前必须告知gl 渲染的尺寸大小
    glViewport(0, 0, 800, 600);

    // 为窗口尺寸改变设置回调函数
    glfwSetFramebufferSizeCallback(window,
        window_size_changed);


    // 开始渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //处理用户输入
        process_input(window);

        // 处理渲染指令

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        // 交换缓冲区
        glfwSwapBuffers(window);

    }

    // 清理gl fw
    glfwTerminate();
    return 0;
}