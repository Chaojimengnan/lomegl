#include "lomegl/gl_vertex.h"
#include "lomegl/gl_exception.h"

namespace lomegl {

gl_vertex::gl_vertex() : VAO_(gl_val_factory(gl_val_type::vao)),
                         VBO_(gl_val_factory(gl_val_type::vbo)),
                         EBO_(gl_val_factory(gl_val_type::ebo))
{
}

gl_vertex::~gl_vertex() = default;

[[nodiscard]] unsigned int gl_vertex::vao() const noexcept
{
    return VAO_.get();
}

[[nodiscard]] unsigned int gl_vertex::vbo() const noexcept
{
    return VBO_.get();
}

[[nodiscard]] unsigned int gl_vertex::ebo() const noexcept
{
    return EBO_.get();
}

[[nodiscard]] bool gl_vertex::is_vbo_binded() const noexcept
{
    return is_vbo_binded_;
}

[[nodiscard]] bool gl_vertex::is_ebo_binded() const noexcept
{
    return is_ebo_binded_;
}

[[nodiscard]] int gl_vertex::ebo_counts() const noexcept
{
    return ebo_counts_;
}

[[nodiscard]] int gl_vertex::vbo_counts() const noexcept
{
    return vbo_counts_;
}

// Before any operation, call this function
gl_vertex& gl_vertex::bind_this()
{
    glBindVertexArray(VAO_.get());
    return *this;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
gl_vertex& gl_vertex::bind_elemnt_buffer_data(const void* ebo_data, GLsizeiptr size, GLenum usage, int data_counts)
{
    assert(check_vao_bind_());
    assert(!is_ebo_binded_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_.get());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, ebo_data, usage);
    MYGL_CHECK_GL_ERROR();
    is_ebo_binded_ = true;
    ebo_counts_ = data_counts;
    return *this;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
gl_vertex& gl_vertex::bind_array_buffer_data(const void* vbo_data, GLsizeiptr size, GLenum usage, int data_counts)
{
    assert(check_vao_bind_());
    assert(!is_vbo_binded_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_.get());
    glBufferData(GL_ARRAY_BUFFER, size, vbo_data, usage);
    MYGL_CHECK_GL_ERROR();
    is_vbo_binded_ = true;
    vbo_counts_ = data_counts;
    return *this;
}

gl_vertex& gl_vertex::vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* start_offset)
{
    assert(check_vao_bind_() && check_vbo_bind_());
    glVertexAttribPointer(index, size, type, normalized, stride, start_offset);
    MYGL_CHECK_GL_ERROR();
    return *this;
}

gl_vertex& gl_vertex::enable_vertex_attrib_array(GLuint index)
{
    assert(check_vao_bind_() && check_vbo_bind_());
    glEnableVertexAttribArray(index);
    assert(glGetError() == 0);
    return *this;
}

bool gl_vertex::check_vao_bind_()
{
    int current_bind_vao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_bind_vao);
    return !(current_bind_vao == 0 || current_bind_vao != VAO_.get());
}

bool gl_vertex::check_vbo_bind_()
{
    int current_bind_vbo = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_bind_vbo);
    return !(current_bind_vbo == 0 || current_bind_vbo != VBO_.get());
}

} // namespace lomegl