#include <glad/glad.h>

#include "lomegl/gl_exception.h"
#include "lomegl/gl_shader.h"
#include <cassert>
#include <string>

namespace lomegl {

gl_shader::gl_shader() : shader_program_(gl_val_factory<gl_val_type::program>()),
                         vertex_shader_(gl_val_factory<gl_val_type::vertex_shader>(0)),
                         fragment_shader_(gl_val_factory<gl_val_type::fragment_shader>(0)),
                         geometry_shader_(gl_val_factory<gl_val_type::geometry_shader>(0))
{
    // 0 is a invaild value
    vertex_shader_.release();
    geometry_shader_.release();
    fragment_shader_.release();
}

gl_shader::~gl_shader() = default;

[[nodiscard]] bool gl_shader::is_vaild() const noexcept
{
    return is_linked_;
}

[[nodiscard]] unsigned int gl_shader::shader() const noexcept
{
    return shader_program_.get();
}

[[nodiscard]] int gl_shader::get_uniform_loc(std::string_view uniform_name) const noexcept
{
    assert(is_vaild());
    return glGetUniformLocation(shader_program_.get(), uniform_name.data());
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
// void gl_shader::add_source_(gl_val& shader_index, std::string_view shader_name, std::string_view source)
// {
//     const char* temp_str = source.data();
//     lomeglcall(glShaderSource, shader_index.get(), 1, &temp_str, nullptr);
//     lomeglcall(glCompileShader, shader_index.get());

//     int success = 0;

//     lomeglcall(glGetShaderiv, shader_index.get(), GL_COMPILE_STATUS, &success);

//     if (success == 0)
//     {
//         char info_log[512];
//         lomeglcall(glGetShaderInfoLog, shader_index.get(), 512, nullptr, info_log);
//         lomeglcall(glDeleteShader, shader_index.get());
//         shader_index.get() = 0;
//         shader_index.release();
//         throw shader_error(std::string(shader_name) + " complie fails: " + info_log);
//     }

//     lomeglcall(glAttachShader, shader_program_.get(), shader_index.get());
// }

gl_shader& gl_shader::add_vertex(std::string_view vertex_source)
{
    assert(!is_linked_ && vertex_shader_.get() == 0);
    vertex_shader_ = gl_val_factory<gl_val_type::vertex_shader>();
    add_source_(vertex_shader_, "vertex shader", vertex_source.data());
    return *this;
}

gl_shader& gl_shader::add_geometry(std::string_view geometry_source)
{
    assert(!is_linked_ && geometry_shader_.get() == 0);
    geometry_shader_ = gl_val_factory<gl_val_type::geometry_shader>();
    add_source_(geometry_shader_, "geometry shader", geometry_source.data());
    return *this;
}

gl_shader& gl_shader::add_fragment(std::string_view fragment_source)
{
    assert(!is_linked_ && fragment_shader_.get() == 0);
    fragment_shader_ = gl_val_factory<gl_val_type::fragment_shader>();
    add_source_(fragment_shader_, "fragment shader", fragment_source.data());
    return *this;
}

gl_shader& gl_shader::link_shader()
{
    assert(!is_linked_ && vertex_shader_.get() != 0 && fragment_shader_.get() != 0);
    lomeglcall(glLinkProgram, shader_program_.get());

    int success = 0;

    lomeglcall(glGetProgramiv, shader_program_.get(), GL_LINK_STATUS, &success);
    if (success == 0)
    {
        char info_log[512];
        lomeglcall(glGetProgramInfoLog, shader_program_.get(), 512, nullptr, info_log);
        throw shader_error(std::string("program link fails: ") + info_log);
    }

    if (geometry_shader_.get() != 0)
    {
        lomeglcall(glDeleteShader, geometry_shader_.get());
        geometry_shader_.release();
        geometry_shader_.get() = 0;
    }

    lomeglcall(glDeleteShader, vertex_shader_.get());
    lomeglcall(glDeleteShader, fragment_shader_.get());
    vertex_shader_.release();
    fragment_shader_.release();
    vertex_shader_.get() = fragment_shader_.get() = 0;
    is_linked_ = true;
    return *this;
}

gl_shader& gl_shader::use()
{
    assert(is_vaild());
    lomeglcall(glUseProgram, shader_program_.get());
    return *this;
}
} // namespace lomegl
