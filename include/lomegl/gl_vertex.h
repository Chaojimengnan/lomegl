#pragma once
#include "lomegl/gl_base.h"

namespace lomegl {

// The class which manage how vertex array data input to vertex shader.
class gl_vertex : public string_id
{
public:
    gl_vertex();
    ~gl_vertex();
    gl_vertex(const gl_vertex&) = delete;
    gl_vertex(gl_vertex&&) = default;
    gl_vertex& operator=(const gl_vertex&) = delete;
    gl_vertex& operator=(gl_vertex&&) = default;

    [[nodiscard]] unsigned int vao() const noexcept;
    [[nodiscard]] unsigned int vbo() const noexcept;
    [[nodiscard]] unsigned int ebo() const noexcept;
    // [[nodiscard]] bool is_vbo_binded() const noexcept;
    // [[nodiscard]] bool is_ebo_binded() const noexcept;
    [[nodiscard]] int ebo_counts() const noexcept;
    [[nodiscard]] int vbo_counts() const noexcept;

    // Before any operation, call this function
    gl_vertex& bind_this();
    gl_vertex& bind_elemnt_buffer_data(const void* ebo_data, GLsizeiptr size, GLenum usage, int elemnt_counts);
    gl_vertex& bind_array_buffer_data(const void* vbo_data, GLsizeiptr size, GLenum usage, int vertex_counts);
    gl_vertex& vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* start_offset);
    gl_vertex& enable_vertex_attrib_array(GLuint index);

private:
    bool check_vao_bind_();
    bool check_vbo_bind_();

    unique_vao VAO_;
    unique_vbo VBO_;
    unique_ebo EBO_;
    // bool is_vbo_binded_ = false;
    // bool is_ebo_binded_ = false;
    int ebo_counts_ = 0;
    int vbo_counts_ = 0;
};

} // namespace lomegl