#pragma once

#include "lomegl/gl_base.h"

namespace lomegl {

class gl_texture
{
public:
    gl_texture(unsigned int texture_type);
    ~gl_texture() = default;
    gl_texture(const gl_texture&) = delete;
    gl_texture(gl_texture&&) = default;
    gl_texture& operator=(const gl_texture&) = delete;
    gl_texture& operator=(gl_texture&&) = default;

    [[nodiscard]] const gl_val& get_texture() const noexcept;
    [[nodiscard]] gl_val& get_texture() noexcept;
    gl_texture& active_texture_unit(unsigned int texture_unit);
    gl_texture& bind();
    gl_texture& tex_parameteri(unsigned int pname, int param);
    gl_texture& add_image_data_to(int level, int internal_format,
        int width, int height, int dummy, unsigned int data_format,
        unsigned int data_type, const void* image);
    gl_texture& generate_mipmap();

private:
    bool check_texture_bind_();
    [[nodiscard]] static constexpr unsigned int get_texture_pname_from_type_(unsigned int target_type);

    gl_val texture_;
    unsigned int texture_type_ = 0;
};

} // namespace lomegl