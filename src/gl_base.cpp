#include <glad/glad.h>

#include "lomegl/gl_base.h"
#include <stdexcept>

namespace lomegl {
gl_val gl_val_factory(gl_val_type val_type)
{
    unsigned int val = 0;
    switch (val_type)
    {
    case gl_val_type::vao:
        glGenVertexArrays(1, &val);
        break;
    case gl_val_type::vbo:
    case gl_val_type::ebo:
        glGenBuffers(1, &val);
        break;
    case gl_val_type::vertex_shader:
        val = glCreateShader(GL_VERTEX_SHADER);
        break;
    case gl_val_type::fragment_shader:
        val = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case gl_val_type::geometry_shader:
        val = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    case gl_val_type::program:
        val = glCreateProgram();
        break;
    case gl_val_type::texture:
        glGenTextures(1, &val);
        break;
    default:
        throw std::runtime_error("Unknow type");
    }

    return gl_val_factory(val, val_type);
}

gl_val gl_val_factory(unsigned int val, gl_val_type val_type)
{
    switch (val_type)
    {
    case gl_val_type::vao:
        return { val, [](unsigned int index) {
                    glDeleteVertexArrays(1, &index);
                } };
    case gl_val_type::vbo:
    case gl_val_type::ebo:
        return { val, [](unsigned int index) {
                    glDeleteBuffers(1, &index);
                } };
    case gl_val_type::vertex_shader:
    case gl_val_type::fragment_shader:
    case gl_val_type::geometry_shader:
        return { val, glDeleteShader };
    case gl_val_type::program:
        return { val, glDeleteProgram };
    case gl_val_type::texture:
        return { val, [](unsigned int index) {
                    glDeleteTextures(1, &index);
                } };
    default:
        throw std::runtime_error("Unknow type");
    }
}

} // namespace lomegl
