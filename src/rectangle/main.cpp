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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// ����һ����������
static const float vertices[] = {
    0.5f,0.5f,0.0f,//���Ͻ�
    0.5f,-0.5f,0.0f, //���½�
    -0.5f,-0.5f,0.0f, //���½�
    -0.5f,0.5f,0.0f //���Ͻ�
};

//����һ��������������
static const unsigned int index_vector[] = {
    // ע��������0��ʼ! 
    // ����������(0,1,2,3)���Ƕ�������vertices���±꣬
    // �����������±��������ϳɾ���
    0,1,3,//��һ��������
    1,2,3 // �ڶ���������
};
// ��������ɫ�������Ƿ�ɹ�
static bool check_compile_shader_status(GLuint shader_object, std::string& error_msg)
{
    int is_success;
    char sz_error_msg[512];
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &is_success);
    if (!is_success)
    {
        glGetShaderInfoLog(shader_object, 512, nullptr, sz_error_msg);
        error_msg = sz_error_msg;
        return false;
    }
    return true;
}

// ���������ɫ�������Ƿ�ʧ��
static bool check_link_program_status(GLuint program_object, std::string& error_msg)
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


// ������ɫ�� GLSL Դ��
const char* vertex_shader_source = "#version 330 core \n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"}\n";


// Ƭ����ɫ�� GLSL Դ��
const char* fragment_shader_source = "#version 330 core \n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"frag_color = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\n";

int main(int argc, char** argv)
{
    // ��ʼ��GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //����gl ���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ����gl �ΰ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // ʹ��glfw �������ڶ���
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
    // ��ʼ��glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        cout << "init glad fail" << endl;
        return -1;
    }
    // ����Ⱦ֮ǰ�����֪gl ��Ⱦ�ĳߴ��С
    glViewport(0, 0, 800, 600);

    // Ϊ���ڳߴ�ı����ûص�����
    glfwSetFramebufferSizeCallback(window,
        window_size_changed);


    // ��ʼ��������ɫ��
    //1�� ����һ��������ɫ������
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    //2����������ɫ��Դ�븽�ӵ������ɫ�������� Ȼ�������
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);

    glCompileShader(vertex_shader);
    std::string error_message;
    if (!check_compile_shader_status(vertex_shader, error_message))
    {
        cout << "compile shader fail " << error_message << endl;
        // ����gl fw
        glfwTerminate();
        return -1;
    }

    // ��ʼ��Ƭ����ɫ��
    //1������һ��Ƭ����ɫ������
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //2����Ƭ����ɫ��Դ�븽�ӵ����Ƭ����ɫ�������� Ȼ�������
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

    glCompileShader(fragment_shader);
    if (!check_compile_shader_status(fragment_shader, error_message))
    {
        cout << "compile shader fail " << error_message << endl;
        // ����gl fw
        glfwTerminate();
        return -1;
    }


    // ��������ɫ�������� Ƭ����ɫ������ ���ӵ�һ��������Ⱦ����ɫ������ ��

    // ����һ����ɫ������
    GLuint shader_program = glCreateProgram();

    // ��������ɫ�����ӵ������ɫ��������
    glAttachShader(shader_program, vertex_shader);
    //��Ƭ����ɫ�����ӵ������ɫ��������
    glAttachShader(shader_program, fragment_shader);

    //������ɫ�����������ɫ��������
    glLinkProgram(shader_program);

    if (!check_link_program_status(shader_program, error_message))
    {
        cout << "link program fail " << error_message << endl;
        // ����gl fw
        glfwTerminate();
        return -1;
    }
    // ��������ɫ��������ɫ����������ɾ����Щ��ɫ��������
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // ���ɶ����������
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // �󶨶����������
    glBindVertexArray(VAO);

    // ���ɶ������ݻ������ID
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // ����Ԫ�ػ������
    GLuint EBO;
    glGenBuffers(1, &EBO);

    //�󶨶��㻺�����ID �����㻻��������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // ���ڴ��еĶ��㻺�����ݸ��Ƶ� �Դ��еĶ��㻺������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ��Ԫ�ػ������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // �����ڴ��е�Ԫ�ػ������ݵ� ���Ԫ�ػ��������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_vector),
        index_vector, GL_STATIC_DRAW);


    // ���Ӷ�������
    glVertexAttribPointer(0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr);

    // ���ö������������� ָ��λ�õĶ�������
    glEnableVertexAttribArray(0);
    // �����ǰ���㻺�����
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // �����ǰ�����������
    glBindVertexArray(0);
    // ��ʼ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        //�����û�����
        process_input(window);

        // ������Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ��Ⱦ������
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        // ��������ʹ����Ԫ�ػ������ ��ָ�����㻺������ж���������������Ϣ ��������ʹ�� glDrawElements ����
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwPollEvents();
        // ����������
        glfwSwapBuffers(window);

    }

    // ɾ�������������
    glDeleteVertexArrays(1, &VAO);
    // ɾ�����㻺�����
    glDeleteBuffers(1, &VBO);
    // ɾ��Ԫ����������
    glDeleteBuffers(1, &EBO);
    // ɾ����ɫ���������
    glDeleteProgram(shader_program);

    // ����gl fw
    glfwTerminate();
    return 0;
}