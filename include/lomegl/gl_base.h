#pragma once
#include <glad/glad.h>

#include <functional>
#include <type_traits>

#include <lotools/raii_control.h>

namespace lomegl {

// using gl_val = lot::unique_val<unsigned int, std::function<void(unsigned int&)>>;

enum class gl_val_type
{
    vao,             // 0
    vbo,             // 1
    ebo,             // 1
    vertex_shader,   // 2
    fragment_shader, // 2
    geometry_shader, // 2
    program,         // 3
    texture          // 4
};

using unique_vao = lot::fn_unique_val<unsigned int, [](unsigned int& index) { glDeleteVertexArrays(1, &index); }>;
using unique_vbo = lot::fn_unique_val<unsigned int, [](unsigned int& index) { glDeleteBuffers(1, &index); }>;
using unique_ebo = unique_vbo;
using unique_vertex_shader = lot::fn_unique_val<unsigned int, [](unsigned int& index) { glDeleteShader(index); }>;
using unique_fragment_shader = unique_vertex_shader;
using unique_geometry_shader = unique_vertex_shader;
using unique_program = lot::fn_unique_val<unsigned int, [](unsigned int& index) { glDeleteProgram(index); }>;
using unique_texture = lot::fn_unique_val<unsigned int, [](unsigned int& index) { glDeleteTextures(1, &index); }>;

template <gl_val_type val_type>
auto gl_val_factory(unsigned int val)
{
    if constexpr (val_type == gl_val_type::vao)
    {
        return unique_vao { val };
    } else if constexpr (val_type == gl_val_type::vbo)
    { // NOLINT(bugprone-branch-clone)
        return unique_vbo { val };
    } else if constexpr (val_type == gl_val_type::ebo)
    {
        return unique_ebo { val };
    } else if constexpr (val_type == gl_val_type::vertex_shader)
    { // NOLINT(bugprone-branch-clone)
        return unique_vertex_shader { val };
    } else if constexpr (val_type == gl_val_type::fragment_shader)
    {
        return unique_fragment_shader { val };
    } else if constexpr (val_type == gl_val_type::geometry_shader)
    {
        return unique_geometry_shader { val };
    } else if constexpr (val_type == gl_val_type::program)
    {
        return unique_program { val };
    } else if constexpr (val_type == gl_val_type::texture)
    {
        return unique_texture { val };
    }
}

template <gl_val_type val_type>
auto gl_val_factory()
{
    unsigned int val = 0;
    if constexpr (val_type == gl_val_type::vao)
    {
        glGenVertexArrays(1, &val);
    } else if constexpr (val_type == gl_val_type::vbo || val_type == gl_val_type::ebo)
    {
        glGenBuffers(1, &val);
    } else if constexpr (val_type == gl_val_type::vertex_shader)
    {
        val = glCreateShader(GL_VERTEX_SHADER);
    } else if constexpr (val_type == gl_val_type::fragment_shader)
    {
        val = glCreateShader(GL_FRAGMENT_SHADER);
    } else if constexpr (val_type == gl_val_type::geometry_shader)
    {
        val = glCreateShader(GL_GEOMETRY_SHADER);
    } else if constexpr (val_type == gl_val_type::program)
    {
        val = glCreateProgram();
    } else if constexpr (val_type == gl_val_type::texture)
    {
        glGenTextures(1, &val);
    }
    return gl_val_factory<val_type>(val);
}

} // namespace lomegl