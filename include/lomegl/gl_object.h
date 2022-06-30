#pragma once

#include <glad/glad.h>

#include "lomegl/gl_base.h"
#include "lomegl/gl_fwd.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace lomegl {

class gl_object : public string_id
{
public:
    gl_object();
    gl_object(const glm::vec3& pos);
    gl_object(const glm::vec3& pos, const glm::vec3& angle);
    gl_object(const glm::vec3& pos, const glm::vec3& angle, const glm::vec3& scale);
    virtual ~gl_object();
    gl_object(const gl_object&) = default;
    gl_object& operator=(const gl_object&) = default;
    gl_object(gl_object&&) = default;
    gl_object& operator=(gl_object&&) = default;

    // x, y, z
    static constexpr glm::vec3 world_up = glm::vec3(0, 1, 0);
    static constexpr glm::vec3 world_front = glm::vec3(0, 0, 1);
    static constexpr glm::vec3 world_right = glm::vec3(1, 0, 0);

    glm::mat4& get_model_mat();
    glm::mat4& get_model_mat_no_scale();
    glm::mat4& get_view_mat();
    glm::vec3 from_world(const glm::vec3& world_pos);
    glm::vec3 to_world(const glm::vec3& local_pos);
    void update_matrix();

    [[nodiscard]] glm::vec3& get_pos() noexcept;
    gl_object& set_pos(const glm::vec3& new_pos) noexcept;
    gl_object& add_pos(const glm::vec3& pos) noexcept;
    gl_object& set_pos(float pos_x, float pos_y, float pos_z) noexcept;
    gl_object& set_pos_x(float pos_x) noexcept;
    gl_object& set_pos_y(float pos_y) noexcept;
    gl_object& set_pos_z(float pos_z) noexcept;
    gl_object& add_pos(float pos_x, float pos_y, float pos_z) noexcept;
    gl_object& add_pos_x(float pos_x) noexcept;
    gl_object& add_pos_y(float pos_y) noexcept;
    gl_object& add_pos_z(float pos_z) noexcept;
    gl_object& add_pos_local(const glm::vec3& pos) noexcept;
    gl_object& add_pos_local(float pos_x, float pos_y, float pos_z) noexcept;
    gl_object& add_pos_x_local(float pos_x) noexcept;
    gl_object& add_pos_y_local(float pos_y) noexcept;
    gl_object& add_pos_z_local(float pos_z) noexcept;

    // in radian
    [[nodiscard]] glm::vec3& get_front() noexcept;
    [[nodiscard]] glm::vec3& get_right() noexcept;
    [[nodiscard]] glm::vec3& get_up() noexcept;
    gl_object& set_angle(const glm::vec3& new_angle) noexcept;
    gl_object& add_angle(const glm::vec3& angle) noexcept;
    gl_object& set_angle(float roll, float yaw, float pitch) noexcept;
    // gl_object& set_angle_pitch(float pitch) noexcept;
    // gl_object& set_angle_yaw(float yaw) noexcept;
    // gl_object& set_angle_roll(float roll) noexcept;
    gl_object& add_angle(float roll, float yaw, float pitch) noexcept;
    gl_object& add_angle_pitch(float pitch) noexcept;
    gl_object& add_angle_yaw(float yaw) noexcept;
    gl_object& add_angle_roll(float roll) noexcept;
    gl_object& add_angle_local(const glm::vec3& angle) noexcept;
    gl_object& add_angle_local(float roll, float yaw, float pitch) noexcept;
    gl_object& add_angle_pitch_local(float pitch) noexcept;
    gl_object& add_angle_yaw_local(float yaw) noexcept;
    gl_object& add_angle_roll_local(float roll) noexcept;

    [[nodiscard]] glm::vec3& get_scale() noexcept;
    gl_object& set_scale(const glm::vec3& new_scale) noexcept;
    gl_object& add_scale(const glm::vec3& scale) noexcept;
    gl_object& set_scale(float scale_x, float scale_y, float scale_z) noexcept;
    gl_object& set_scale_x(float scale_x) noexcept;
    gl_object& set_scale_y(float scale_y) noexcept;
    gl_object& set_scale_z(float scale_z) noexcept;
    gl_object& add_scale(float scale_x, float scale_y, float scale_z) noexcept;
    gl_object& add_scale_x(float scale_x) noexcept;
    gl_object& add_scale_y(float scale_y) noexcept;
    gl_object& add_scale_z(float scale_z) noexcept;

private:
    bool is_transform_ = true;
    glm::vec3 pos_;            // world cood
    glm::vec3 obj_front_;      // object front
    glm::vec3 obj_right_;      // object right
    glm::vec3 obj_up_;         // object up
    glm::quat rotate_;         // rotate quaternion
    glm::vec3 scale_;          // scale
    glm::mat4 model_no_scale_; // model matrix(local -> world), no scale
    glm::mat4 model_;          // model matrix(local -> world), with scale
    glm::mat4 view_;           // view matrix(world -> local), no scale
};

// TODO: normal object which has texture, shader, vertex.
class gl_entity : public gl_object
{
public:
    gl_entity() = default;
    gl_entity(const char* vertex, std::initializer_list<const char*> texture_list);

    [[nodiscard]] const std::weak_ptr<gl_vertex>& get_vertex() const noexcept;
    [[nodiscard]] const std::vector<std::weak_ptr<gl_texture>>& get_texture_list() const noexcept;
    gl_entity& set_vertex(const char* vertex);
    gl_entity& add_texture(const char* new_texture);
    gl_entity& remove_texture(int texture_index);
    gl_entity& replace_texture(const char* new_texture, int texture_index);
    gl_entity& clear_texture() noexcept;

    // Draw this entity, if the vertex is not use EBO, second param is ignored
    // Your shader need a uniform value 'model', the model mat of this entity will be passed
    gl_entity& draw(unsigned int draw_type, unsigned int elem_index_type = 0);

    // You need pass a custom function, this will call it after bind everythins needed.
    gl_entity& draw(const std::function<void(gl_entity*)>& func, unsigned int draw_type, unsigned int elem_index_type = 0);

private:
    std::weak_ptr<gl_vertex> vertex_;
    std::vector<std::weak_ptr<gl_texture>> texture_;
};

} // namespace lomegl