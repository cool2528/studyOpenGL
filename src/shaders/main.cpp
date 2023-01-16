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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// 设置一个顶点数据 将颜色数据和顶点数据都存放到VBO中 顶点缓冲数组中
static const float vertices[] = {
    // 位置             //颜色
    -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
    0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
    0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
};

// 检测编译着色器代码是否成功
static bool check_compile_shader_status(GLuint shader_object,std::string& error_msg)
{
    int is_success;
    char sz_error_msg[512];
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &is_success);
    if(!is_success)
    {
        glGetShaderInfoLog(shader_object, 512, nullptr, sz_error_msg);
        error_msg = sz_error_msg;
        return false;
    }
    return true;
}

// 检测链接着色器程序是否失败
static bool check_link_program_status(GLuint program_object,std::string& error_msg)
{
    int is_success;
    char sz_error_msg[512];
    glGetProgramiv(program_object, GL_LINK_STATUS, &is_success);
    if (!is_success)
    {
        glGetProgramInfoLog(program_object, 512, nullptr, sz_error_msg);
        error_msg = sz_error_msg;
        return false;
    }
    return true;
}


// 顶点着色器 GLSL 源码
const char* vertex_shader_source = "#version 330 core \n"
"layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为 0 \n"
"layout (location = 1) in vec3 aColor; // 颜色变量属性的位置值设置为1 \n"
"out vec3 vertexcolor; // 为片段着色器指定一个颜色输出\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"vertexcolor = aColor; // 把输出设置为暗红色\n"
"}\n";


// 片段着色器 GLSL 源码
const char* fragment_shader_source = "#version 330 core \n"
"out vec4 frag_color;\n"
"//in vec4 vertexcolor; // 从顶点着色器传来的输入变量(名称类型相同 opengl 默认会链接到一起)\n"
"//uniform vec4 uniform_color; // 在opengl 程序中设置这个变量值\n"
"in vec3 vertexcolor; // 使用顶点着色器中输出的颜色信息\n"
"void main()\n"
"{\n"
"//frag_color = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"frag_color = vec4(vertexcolor,1.0f);\n"
"//frag_color = uniform_color;\n"
"}\n";

int main(int argc, char** argv)
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
    if (!window)
    {
        cout << "create gl window fail" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 初始化glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        cout << "init glad fail" << endl;
        return -1;
    }

    // 输出硬件支持的最大顶点属性的上限 各个平台显卡支持的都不同
    int max_vertex_attributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&max_vertex_attributes);
    cout << "current opengl max vertex attributes " << max_vertex_attributes << endl;
    // 在渲染之前必须告知gl 渲染的尺寸大小
    glViewport(0, 0, 800, 600);

    // 为窗口尺寸改变设置回调函数
    glfwSetFramebufferSizeCallback(window,
        window_size_changed);


    // 初始化顶点着色器
    //1、 创建一个顶点着色器对象
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    //2、将顶点着色器源码附加到这个着色器对象上 然后编译它
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);

    glCompileShader(vertex_shader);
    std::string error_message;
    if(!check_compile_shader_status(vertex_shader, error_message))
    {
        cout << "compile shader fail " << error_message << endl;
        // 清理gl fw
        glfwTerminate();
        return -1;
    }

    // 初始化片段着色器
    //1、创建一个片段着色器对象
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //2、将片段着色器源码附加到这个片段着色器对象上 然后编译它
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

    glCompileShader(fragment_shader);
    if (!check_compile_shader_status(fragment_shader, error_message))
    {
        cout << "compile shader fail " << error_message << endl;
        // 清理gl fw
        glfwTerminate();
        return -1;
    }


    // 将顶点着色器对象与 片段着色器对象 链接到一个用来渲染的着色器程序 中

    // 创建一个着色器程序
    GLuint shader_program = glCreateProgram();

    // 将顶点着色器附加到这个着色器程序上
    glAttachShader(shader_program, vertex_shader);
    //将片段着色器附加到这个着色器程序上
    glAttachShader(shader_program, fragment_shader);

    //链接着色器对象到这个着色器程序中
    glLinkProgram(shader_program);

    if (!check_link_program_status(shader_program, error_message))
    {
        cout << "link program fail " << error_message << endl;
        // 清理gl fw
        glfwTerminate();
        return -1;
    }
    // 链接完着色器对象到着色器程序后可以删除这些着色器对象了
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // 生成顶点数组对象
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // 绑定顶点数组对象
    glBindVertexArray(VAO);

    // 生成顶点数据缓冲对象ID
    GLuint VBO;
    glGenBuffers(1, &VBO);

    //绑定顶点缓冲对象ID 到顶点换成类型上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 将内存中的顶点缓冲数据复制到 显存中的顶点缓冲区中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置位置顶点属性
    glVertexAttribPointer(0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        nullptr);
    // 启用顶点属性数组中 指定位置的顶点属性
    glEnableVertexAttribArray(0);

    //设置颜色顶点属性
    glVertexAttribPointer(1,
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    (void*)(3 * sizeof(float)));

    // 启用顶点属性数组中 指定颜色的顶点属性
    glEnableVertexAttribArray(1);

    // 开始渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //处理用户输入
        process_input(window);

        // 处理渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //使用uniform 根据时间动态改变这个三角形的片段着色器输出的颜色
  /*      const float current_time_value = glfwGetTime();
        const float color_value = sin(current_time_value) / 2.0f + 0.5f;
        const auto uniform_color_location = glGetUniformLocation(shader_program, "uniform_color");*/
        // 使用这个着色器程序
        glUseProgram(shader_program);
        // 动态更新颜色信息
 /*       if(uniform_color_location != -1){
            glUniform4f(uniform_color_location, 0.0,
            color_value,
            0.0,
            1.0f);
        }*/
        // 绑定顶点数组VAO
        glBindVertexArray(VAO);
        // 渲染三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        // 交换缓冲区
        glfwSwapBuffers(window);

    }

    // 删除顶点数组对象
    glDeleteVertexArrays(1,&VAO);
    // 删除顶点缓冲对象
    glDeleteBuffers(1, &VBO);
    // 删除着色器程序对象
    glDeleteProgram(shader_program);

    // 清理gl fw
    glfwTerminate();
    return 0;
}