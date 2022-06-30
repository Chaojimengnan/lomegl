#include "lomegl/gl_world.h"
#include "lomegl/gl_object.h"
#include "lomegl/gl_shader.h"
#include "lomegl/gl_texture.h"
#include "lomegl/gl_vertex.h"

#include <glm/gtc/type_ptr.hpp>

namespace lomegl {

gl_world::gl_world() = default;

gl_world::~gl_world()
{
    if (get_current_world() == this)
        get_current_world_() = nullptr;
}

gl_world& gl_world::set_current_camera(const char* obj_name)
{
    assert(exists<gl_object>(obj_name));
    current_camera_ = obj_name;
    return *this;
}

[[nodiscard]] gl_shader& gl_world::get_current_shader()
{
    assert(!current_shader_.empty());
    return get<gl_shader>(current_shader_.c_str());
}

[[nodiscard]] gl_object& gl_world::get_current_camera()
{
    assert(!current_camera_.empty());
    return get<gl_object>(current_camera_.c_str());
}

[[nodiscard]] std::string gl_world::get_current_shader_name() noexcept
{
    return current_shader_;
}

[[nodiscard]] std::string gl_world::get_current_camera_name() noexcept
{
    return current_camera_;
}

[[nodiscard]] std::pair<int, int> gl_world::get_screen_size() noexcept
{
    return screen_size_;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
gl_world& gl_world::set_screen_size(int width, int height) noexcept
{
    screen_size_.first = width;
    screen_size_.second = height;
    return *this;
}

int gl_world::get_screen_width() const noexcept
{
    return screen_size_.first;
}

int gl_world::get_screen_height() const noexcept
{
    return screen_size_.second;
}

gl_world& gl_world::set_current_shader_view_mat()
{
    assert(!current_shader_.empty() && !current_camera_.empty());
    get<gl_shader>(current_shader_.c_str()).uniform(glUniformMatrix4fv, gl_shader::view_name, 1, GL_FALSE, glm::value_ptr(get<gl_object>(current_camera_.c_str()).get_view_mat()));
    return *this;
}

gl_world& gl_world::set_current_shader_projection_mat(float fov, float near, float far)
{
    assert(!current_shader_.empty());
    glm::mat4 projection = glm::perspective(fov, static_cast<float>(screen_size_.first) / static_cast<float>(screen_size_.second), near, far);
    get<gl_shader>(current_shader_.c_str()).uniform(glUniformMatrix4fv, gl_shader::projection_name, 1, GL_FALSE, glm::value_ptr(projection));
    return *this;
}

gl_world& gl_world::use_shader(const char* shader_name)
{
    assert(exists<gl_shader>(shader_name));
    current_shader_ = shader_name;
    get<gl_shader>(current_shader_.c_str()).use();
    return *this;
}

} // namespace lomegl
