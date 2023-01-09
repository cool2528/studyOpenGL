#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
using namespace std;
// ���ڴ�С�ı�ص�����
static void window_size_changed(GLFWwindow* window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}
// �����û�����
static void process_input(GLFWwindow* window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, true);
	}
}
int main(int argc, char **argv)
{
    // ��ʼ��GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //����gl ���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ����gl �ΰ汾��

    // ʹ��glfw �������ڶ���
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
    // ��ʼ��glad
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        cout << "init glad fail" << endl;
        return -1;
    }
    // ����Ⱦ֮ǰ�����֪gl ��Ⱦ�ĳߴ��С
    glViewport(0, 0, 800, 600);

    // Ϊ���ڳߴ�ı����ûص�����
    glfwSetFramebufferSizeCallback(window,
        window_size_changed);


    // ��ʼ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        //�����û�����
        process_input(window);

        // ������Ⱦָ��

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        // ����������
        glfwSwapBuffers(window);

    }

    // ����gl fw
    glfwTerminate();
    return 0;
}