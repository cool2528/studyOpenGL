/**
 * @brief 通用着色器加载器
*/
#pragma once
#include <glad/glad.h>
#include <string_view>
#include <type_traits>
#include <fstream>
#include <sstream>
namespace utils
{
    class Shaders
    {
        public:
            explicit Shaders(const std::string_view& vertex_shader_path,
            const std::string_view& fragment_shader_path){
                
                // 从着色器路径读取着色器代码
                std::string vertex_code;
                std::string fragment_code;
                std::ifstream vertex_shader_file(vertex_shader_path.data(),
                    std::ios_base::in | std::ios_base::binary);
                std::ifstream fragment_shader_file(fragment_shader_path.data(),
                    std::ios_base::in | std::ios_base::binary);
                std::stringstream vertex_shader_stream, fragment_shader_stream;
                vertex_shader_stream << vertex_shader_file.rdbuf();
                fragment_shader_stream << fragment_shader_stream.rdbuf();
                // 关闭文件流
                vertex_shader_file.close();
                fragment_shader_file.close();
                vertex_code = vertex_shader_stream.str();
                fragment_code = fragment_shader_stream.str();
                const char* v_shader_code = vertex_code.c_str();
                const char* f_shader_code = fragment_code.c_str();
                std::uint32_t vertex_shader_id = -1, fragment_shader_id = -1;
                int is_succeed = -1;
                char sz_error_message[512]{ 0 };
                // 顶点着色器源码编译
                vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertex_shader_id,
                    1,
                    &v_shader_code,
                    nullptr
                );

                glCompileShader(vertex_shader_id);
                // 检查编译是否有错误
                glGetShaderiv(vertex_shader_id,
                    GL_COMPILE_STATUS, &is_succeed);
                if(!is_succeed)
                {
                    glGetShaderInfoLog(vertex_shader_id,
                        512, nullptr,
                        sz_error_message
                    );
                    throw std::runtime_error(sz_error_message);
                }

                // 片段着色器代码编译
                fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragment_shader_id,
                    1,
                    &f_shader_code,
                    nullptr);
                glCompileShader(fragment_shader_id);
                // 检查编译是否有错误
                glGetShaderiv(fragment_shader_id,
                    GL_COMPILE_STATUS,
                    &is_succeed);
                if(!is_succeed)
                {
                    glGetShaderInfoLog(fragment_shader_id,
                        512, nullptr,
                        sz_error_message
                    );
                    throw std::runtime_error(sz_error_message);
                }

                // 创建着色器程序
                program_id = glCreateProgram();
                glAttachShader(program_id, vertex_shader_id);
                glAttachShader(program_id, fragment_shader_id);
                //链接着色器程序
                glLinkProgram(program_id);
                // 检查链接着色器程序是否有错误
                glGetProgramiv(program_id,
                    GL_LINK_STATUS,
                    &is_succeed);
                if (!is_succeed)
                {
                    glGetShaderInfoLog(program_id,
                        512, nullptr,
                        sz_error_message
                    );
                    throw std::runtime_error(sz_error_message);
                }
                // 删除着色器 他们已经被附加到 我们的着色器程序中了
                glDeleteShader(vertex_shader_id);
                glDeleteShader(fragment_shader_id);
            }
            /**
             * @brief 激活使用着色器程序
            */
           void UseShaderProgram(){
               // 使用着色器程序
                if(program_id != 0)
                {
                    glUseProgram(program_id);
                }
           }
           /**
            * @brief 通用设置uniform 属性变量
           */
          template<typename T>
          void SetUniformAttribute(const std::string_view& uniform_name, T value){
            if (std::is_same_v<T,int>){
                glUniform1i(glGetUniformLocation(program_id,uniform_name.data(),
                value));
            }else if (std::is_same_v<T,float>){
                glUniform1f(glGetUniformLocation(program_id,uniform_name.data(),
                value));

            }else if (std::is_same_v<T,bool>){
                glUniform1i(glGetUniformLocation(program_id,uniform_name.data(),
                static_cast<int>(value)));

            }else if (std::is_same_v<T,double>){
                glUniform1d(glGetUniformLocation(program_id,uniform_name.data(),
                value));
            }
          }
        private:
        uint32_t program_id{0};

    };
}