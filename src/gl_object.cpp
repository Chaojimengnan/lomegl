#include "lomegl/gl_object.h"
#include "lomegl/gl_exception.h"
#include "lomegl/gl_shader.h"
#include "lomegl/gl_texture.h"
#include "lomegl/gl_vertex.h"
#include "lomegl/gl_world.h"

#include <string>

#include <glm/gtc/type_ptr.hpp>

namespace lomegl {

gl_object::gl_object() : gl_object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))
{
}

gl_object::gl_object(const glm::vec3& pos) : gl_object(pos, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))
{
}

gl_object::gl_object(const glm::vec3& pos, const glm::vec3& angle) : gl_object(pos, angle, glm::vec3(1, 1, 1))
{
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
gl_object::gl_object(const glm::vec3& pos, const glm::vec3& angle, const glm::vec3& scale)
    : pos_(pos), obj_front_(world_front), obj_right_(world_right), obj_up_(world_up), rotate_(),
      scale_(scale), model_no_scale_(), model_(), view_()
{
    set_angle(angle);
    update_matrix();
}

gl_object::~gl_object() = default;

glm::mat4& gl_object::get_model_mat()
{
    if (is_transform_)
        update_matrix();
    return model_;
}

glm::mat4& gl_object::get_model_mat_no_scale()
{
    if (is_transform_)
        update_matrix();
    return model_no_scale_;
}

glm::mat4& gl_object::get_view_mat()
{
    if (is_transform_)
        update_matrix();
    return view_;
}

glm::vec3 gl_object::from_world(const glm::vec3& world_pos)
{
    if (is_transform_)
        update_matrix();
    return view_ * glm::vec4(world_pos, 0);
}

glm::vec3 gl_object::to_world(const glm::vec3& local_pos)
{
    if (is_transform_)
        update_matrix();
    return model_no_scale_ * glm::vec4(local_pos, 0);
}

void gl_object::update_matrix()
{
    if (is_transform_)
    {

        // model_no_scale_ = glm::mat4(1.0F);
        // model_no_scale_ = glm::translate(model_no_scale_, pos_);
        // model_no_scale_ = model_no_scale_ * glm::toMat4(rotate_);
        // model_ = glm::scale(model_no_scale_, scale_);
        view_ = glm::lookAt(pos_, pos_ + obj_front_, obj_up_);
        model_no_scale_ = glm::inverse(view_);
        model_ = glm::scale(model_no_scale_, scale_);
        is_transform_ = false;
    }
}

gl_object& gl_object::set_pos(const glm::vec3& new_pos) noexcept
{
    is_transform_ = true;
    pos_ = new_pos;
    return *this;
}

gl_object& gl_object::add_pos(const glm::vec3& pos) noexcept
{
    is_transform_ = true;
    pos_ += pos;
    return *this;
}

[[nodiscard]] glm::vec3& gl_object::get_pos() noexcept
{
    return pos_;
}

gl_object& gl_object::set_pos(float pos_x, float pos_y, float pos_z) noexcept
{
    is_transform_ = true;
    pos_.x = pos_x;
    pos_.y = pos_y;
    pos_.z = pos_z;
    return *this;
}

gl_object& gl_object::set_pos_x(float pos_x) noexcept
{
    is_transform_ = true;
    pos_.x = pos_x;
    return *this;
}

gl_object& gl_object::set_pos_y(float pos_y) noexcept
{
    is_transform_ = true;
    pos_.y = pos_y;
    return *this;
}

gl_object& gl_object::set_pos_z(float pos_z) noexcept
{
    is_transform_ = true;
    pos_.z = pos_z;
    return *this;
}

gl_object& gl_object::add_pos(float pos_x, float pos_y, float pos_z) noexcept
{
    is_transform_ = true;
    pos_.x += pos_x;
    pos_.y += pos_y;
    pos_.z += pos_z;
    return *this;
}

gl_object& gl_object::add_pos_x(float pos_x) noexcept
{
    add_pos(pos_x, 0, 0);
    return *this;
}

gl_object& gl_object::add_pos_y(float pos_y) noexcept
{
    add_pos(0, pos_y, 0);
    return *this;
}

gl_object& gl_object::add_pos_z(float pos_z) noexcept
{
    set_pos(0, 0, pos_z);
    return *this;
}

gl_object& gl_object::add_pos_local(const glm::vec3& pos) noexcept
{
    add_pos_local(pos.x, pos.y, pos.z);
    return *this;
}

gl_object& gl_object::add_pos_local(float pos_x, float pos_y, float pos_z) noexcept
{
    glm::vec3 temp = pos_x * obj_right_ + pos_y * obj_up_ + pos_z * obj_front_;
    add_pos(temp);
    return *this;
}

gl_object& gl_object::add_pos_x_local(float pos_x) noexcept
{
    add_pos_local(pos_x, 0, 0);
    return *this;
}

gl_object& gl_object::add_pos_y_local(float pos_y) noexcept
{
    add_pos_local(0, pos_y, 0);
    return *this;
}
gl_object& gl_object::add_pos_z_local(float pos_z) noexcept
{
    add_pos_local(0, 0, pos_z);
    return *this;
}

gl_object& gl_object::set_angle(const glm::vec3& new_angle) noexcept
{
    is_transform_ = true;
    rotate_ = glm::quat(new_angle);
    obj_front_ = rotate_ * world_front;
    obj_right_ = rotate_ * world_right;
    obj_up_ = rotate_ * world_up;
    return *this;
}

gl_object& gl_object::add_angle(const glm::vec3& angle) noexcept
{
    is_transform_ = true;
    rotate_ = glm::quat(angle);
    obj_front_ = rotate_ * obj_front_;
    obj_right_ = rotate_ * obj_right_;
    obj_up_ = rotate_ * obj_up_;
    // rotate_ = glm::quat(angle) * rotate_;
    // obj_front_ = rotate_ * world_front;
    // obj_right_ = rotate_ * world_right;
    // obj_up_ = rotate_ * world_up;
    return *this;
}

[[nodiscard]] glm::vec3& gl_object::get_front() noexcept
{
    return obj_front_;
}

[[nodiscard]] glm::vec3& gl_object::get_right() noexcept
{
    return obj_right_;
}

[[nodiscard]] glm::vec3& gl_object::get_up() noexcept
{
    return obj_up_;
}

gl_object& gl_object::set_angle(float roll, float yaw, float pitch) noexcept
{
    set_angle(glm::vec3(roll, yaw, pitch));
    return *this;
}

gl_object& gl_object::add_angle(float roll, float yaw, float pitch) noexcept
{
    add_angle(glm::vec3(roll, yaw, pitch));
    return *this;
}

gl_object& gl_object::add_angle_pitch(float pitch) noexcept
{
    add_angle(pitch, 0, 0);
    return *this;
}

gl_object& gl_object::add_angle_yaw(float yaw) noexcept
{
    add_angle(0, yaw, 0);
    return *this;
}

gl_object& gl_object::add_angle_roll(float roll) noexcept
{
    add_angle(0, 0, roll);
    return *this;
}

gl_object& gl_object::add_angle_local(const glm::vec3& angle) noexcept
{
    add_angle_local(angle.r, angle.y, angle.p);
    return *this;
}

gl_object& gl_object::add_angle_local(float roll, float yaw, float pitch) noexcept
{
    is_transform_ = true;
    rotate_ = glm::angleAxis(glm::degrees(roll), obj_front_) * glm::angleAxis(glm::degrees(yaw), obj_up_) * glm::angleAxis(glm::degrees(pitch), obj_right_);
    obj_front_ = rotate_ * obj_front_;
    obj_right_ = rotate_ * obj_right_;
    obj_up_ = rotate_ * obj_up_;
    // rotate_ = glm::angleAxis(glm::degrees(roll), obj_front_) * glm::angleAxis(glm::degrees(yaw), obj_up_) * glm::angleAxis(glm::degrees(pitch), obj_right_) * rotate_;
    // obj_front_ = rotate_ * world_front;
    // obj_right_ = rotate_ * world_right;
    // obj_up_ = rotate_ * world_up;
    return *this;
}

gl_object& gl_object::add_angle_pitch_local(float pitch) noexcept
{
    add_angle_local(0, 0, pitch);
    return *this;
}
gl_object& gl_object::add_angle_yaw_local(float yaw) noexcept
{
    add_angle_local(0, yaw, 0);
    return *this;
}
gl_object& gl_object::add_angle_roll_local(float roll) noexcept
{
    add_angle_local(roll, 0, 0);
    return *this;
}

gl_object& gl_object::set_scale(const glm::vec3& new_scale) noexcept
{
    is_transform_ = true;
    scale_ = new_scale;
    return *this;
}

gl_object& gl_object::add_scale(const glm::vec3& scale) noexcept
{
    is_transform_ = true;
    scale_ += scale;
    return *this;
}

[[nodiscard]] glm::vec3& gl_object::get_scale() noexcept
{
    return scale_;
}

gl_object& gl_object::set_scale(float scale_x, float scale_y, float scale_z) noexcept
{
    is_transform_ = true;
    scale_.x = scale_x;
    scale_.y = scale_y;
    scale_.z = scale_z;
    return *this;
}

gl_object& gl_object::set_scale_x(float scale_x) noexcept
{
    is_transform_ = true;
    scale_.x = scale_x;
    return *this;
}

gl_object& gl_object::set_scale_y(float scale_y) noexcept
{
    is_transform_ = true;
    scale_.y = scale_y;
    return *this;
}

gl_object& gl_object::set_scale_z(float scale_z) noexcept
{
    is_transform_ = true;
    scale_.z = scale_z;
    return *this;
}

gl_object& gl_object::add_scale(float scale_x, float scale_y, float scale_z) noexcept
{
    is_transform_ = true;
    scale_.x += scale_x;
    scale_.y += scale_y;
    scale_.z += scale_z;
    return *this;
}

gl_object& gl_object::add_scale_x(float scale_x) noexcept
{
    is_transform_ = true;
    scale_.x += scale_x;
    return *this;
}

gl_object& gl_object::add_scale_y(float scale_y) noexcept
{
    is_transform_ = true;
    scale_.y += scale_y;
    return *this;
}

gl_object& gl_object::add_scale_z(float scale_z) noexcept
{
    is_transform_ = true;
    scale_.z += scale_z;
    return *this;
}

gl_entity::gl_entity(std::string_view vertex, std::initializer_list<std::string_view> texture_list)
    : vertex_(vertex)
{
    // Should not have empty str and must in gl_world
    assert([&texture_list] {
        for (auto&& item : texture_list)
            if (item.empty() || !gl_world::instance().exists<gl_texture>(item))
                return false;
        return true;
    }());
    assert(gl_world::instance().exists<gl_vertex>(vertex) || vertex.empty());

    for (auto&& item : texture_list)
        texture_.emplace_back(item.data());
}

[[nodiscard]] const std::string& gl_entity::get_vertex() const noexcept
{
    return vertex_;
}

[[nodiscard]] const std::vector<std::string>& gl_entity::get_texture_list() const noexcept
{
    return texture_;
}

gl_entity& gl_entity::set_vertex(std::string_view vertex)
{
    assert(gl_world::instance().exists<gl_vertex>(vertex) || vertex.empty());
    vertex_ = vertex;
    return *this;
}

gl_entity& gl_entity::add_texture(std::string_view new_texture)
{
    assert(!new_texture.empty());
    texture_.emplace_back(new_texture.data());
    return *this;
}

gl_entity& gl_entity::remove_texture(int texture_index)
{
    assert(texture_index >= 0 && texture_index < texture_.size());
    if (texture_.erase(texture_.cbegin() + texture_index) == texture_.cend())
        throw std::runtime_error(std::string("Wrong texture index :") + std::to_string(texture_index));
    return *this;
}

gl_entity& gl_entity::clear_texture() noexcept
{
    texture_.clear();
    return *this;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
gl_entity& gl_entity::draw(unsigned int draw_type, unsigned int elem_index_type)
{
    return draw([](gl_entity* entity) {
        gl_world::instance().get_current_shader().uniform(glUniformMatrix4fv, "model", 1, GL_FALSE, glm::value_ptr(entity->get_model_mat()));
    },
        draw_type, elem_index_type);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
gl_entity& gl_entity::draw(const std::function<void(gl_entity*)>& func, unsigned int draw_type, unsigned int elem_index_type)
{
    assert(!vertex_.empty());
    auto&& world_ins = gl_world::instance();

    auto& current_vertex_ptr = world_ins.get<gl_vertex>(vertex_);

    current_vertex_ptr.bind_this();
    for (auto&& iter : texture_)
        world_ins.get<gl_texture>(iter).bind();

    func(this);

    if (current_vertex_ptr.is_ebo_binded())
        lomeglcall(glDrawElements, draw_type, current_vertex_ptr.ebo_counts(), elem_index_type, nullptr); // NOLINT(modernize-use-nullptr)
    else
        lomeglcall(glDrawArrays, draw_type, 0, current_vertex_ptr.vbo_counts());
    return *this;
}

} // namespace lomegl