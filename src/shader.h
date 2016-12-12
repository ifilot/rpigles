#ifndef _SHADER_H
#define _SHADER_H

#include <string>
#include <iostream>
#include <fstream>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

class Shader {
private:
    static const unsigned int NUM_SHADERS = 2;
    std::string filename;
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    bool flag_loaded;

public:
    Shader(const std::string _filename);

    GLuint get_shader_id() const {
        return this->m_program;
    }

    inline void link_shader() {
        glUseProgram(this->m_program);
    }

    inline void unlink_shader() {
        glUseProgram(0);
    }

    ~Shader();

private:
    std::string load_shader(const std::string& filename);
    GLuint create_shader(const std::string& text, GLenum shader_type);
    void check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& error_message);
};

#endif //_SHADER_H
