#include <cassert>
#include <glad/glad.h>

#include "lomegl/gl_exception.h"
#include "lomegl/gl_texture.h"

namespace lomegl {

gl_texture::gl_texture(unsigned int texture_type) : texture_(lomegl::gl_val_factory(gl_val_type::texture)),
                                                    texture_type_(texture_type)
{
    // Make sure target_type is vaild
    assert([texture_type] {
        return get_texture_pname_from_type_(texture_type) != 0;
    }());
}

[[nodiscard]] const gl_val& gl_texture::get_texture() const noexcept
{
    return texture_;
}

[[nodiscard]] gl_val& gl_texture::get_texture() noexcept
{
    return texture_;
}

gl_texture& gl_texture::active_texture_unit(unsigned int texture_unit)
{
    assert(GL_TEXTURE0 <= texture_unit && texture_unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    lomeglcall(glActiveTexture, texture_unit);
    return *this;
}

gl_texture& gl_texture::bind()
{
    lomeglcall(glBindTexture, texture_type_, texture_.get());
    return *this;
}

gl_texture& gl_texture::tex_parameteri(unsigned int pname, int param)
{
    assert(check_texture_bind_());
    lomeglcall(glTexParameteri, texture_type_, pname, param);
    return *this;
}

gl_texture& gl_texture::add_image_data_to(int level, int internal_format,
    int width, int height, int dummy, unsigned int data_format,
    unsigned int data_type, const void* image)
{
    assert(check_texture_bind_());
    lomeglcall(glTexImage2D, texture_type_, level, internal_format, width,
        height, dummy, data_format, data_type, image);
    return *this;
}

gl_texture& gl_texture::generate_mipmap()
{
    assert(check_texture_bind_());
    lomeglcall(glGenerateMipmap, texture_type_);
    return *this;
}

bool gl_texture::check_texture_bind_()
{
    int current_bind_texture = 0;
    auto current_bind_type_pname = get_texture_pname_from_type_(texture_type_);
    if (current_bind_type_pname == 0)
        return false;
    lomeglcall(glGetIntegerv, current_bind_type_pname, &current_bind_texture);

    return !(current_bind_texture == 0 || current_bind_texture != texture_.get());
}

[[nodiscard]] constexpr unsigned int gl_texture::get_texture_pname_from_type_(unsigned int target_type)
{
    switch (target_type)
    {
    case GL_TEXTURE_1D:
        return GL_TEXTURE_BINDING_1D;
    case GL_TEXTURE_1D_ARRAY:
        return GL_TEXTURE_BINDING_1D_ARRAY;
    case GL_TEXTURE_2D:
        return GL_TEXTURE_BINDING_2D;
    case GL_TEXTURE_2D_ARRAY:
        return GL_TEXTURE_BINDING_2D_ARRAY;
    case GL_TEXTURE_2D_MULTISAMPLE:
        return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
    case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
        return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
    case GL_TEXTURE_3D:
        return GL_TEXTURE_BINDING_3D;
    case GL_TEXTURE_BUFFER:
        return GL_TEXTURE_BINDING_BUFFER;
    case GL_TEXTURE_CUBE_MAP:
        return GL_TEXTURE_BINDING_CUBE_MAP;
    case GL_TEXTURE_RECTANGLE:
        return GL_TEXTURE_BINDING_RECTANGLE;
    default:
        return 0;
    }
}

} // namespace lomegl
