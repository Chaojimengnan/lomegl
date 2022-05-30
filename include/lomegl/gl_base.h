#pragma once
#include <glad/glad.h>

#include <functional>
#include <type_traits>

#include <lotools/raii_control.h>

namespace lomegl {

using gl_val = lot::unique_val<unsigned int, std::function<void(unsigned int&)>>;

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

gl_val gl_val_factory(gl_val_type val_type);

gl_val gl_val_factory(unsigned int val, gl_val_type val_type);

} // namespace lomegl