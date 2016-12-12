#include "shader.h"

Shader::Shader(const std::string _filename) {
    // constructs the program
    this->m_program = glCreateProgram();

    this->filename = "assets/shaders/" + _filename;

    // set vertex shader
    this->m_shaders[0] = create_shader(load_shader(this->filename + ".vs"), GL_VERTEX_SHADER);

    // set fragment shader
    this->m_shaders[1] = create_shader(load_shader(this->filename + ".fs"), GL_FRAGMENT_SHADER);

    // attach all shaders
    for(unsigned int i = 0; i < NUM_SHADERS; i++) {
        glAttachShader(this->m_program, m_shaders[i]);
    }

    glBindAttribLocation(this->m_program, 0, "vPosition");
    // links program and checks for errors
    glLinkProgram(this->m_program);
    check_shader_error(m_program, GL_LINK_STATUS, true, "Error in \"" + this->filename + "\" : Program linking failed: ");

    this->flag_loaded = false;
}

GLuint Shader::create_shader(const std::string& text, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);

    if(shader == 0) {
        std::cerr << "Error: Shader creation failed!" << std::endl;
    }

    const GLchar* shader_source_strings[1];
    GLint shader_source_strings_length[1];

    shader_source_strings[0] = text.c_str();
    shader_source_strings_length[0] = text.length();

    glShaderSource(shader, 1, shader_source_strings, shader_source_strings_length);
    glCompileShader(shader);

    check_shader_error(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

    return shader;
}

std::string Shader::load_shader(const std::string& filename) {
    std::ifstream file;
    file.open((filename).c_str());

    std::string output;
    std::string line;

    if(file.is_open()) {
        while(file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << filename << std::endl;
    }

    return output;
}

void Shader::check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& error_message) {
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(is_program) {
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if(success == GL_FALSE) {
        if(is_program) {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }

        std::cerr << error_message << ": '" << error << "'" << std::endl;
    }
}

Shader::~Shader() {
    // detach and delete the allocated shaders
    for(unsigned int i = 0; i < NUM_SHADERS; i++) {
        glDetachShader(this->m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

    // finally delete the program
    glDeleteProgram(this->m_program);
}
