#pragma once
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "lomegl/gl_base.h"

namespace lomegl {

// Throw when error about shader
struct shader_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

// This class manage shader program.
class gl_shader
{
public:
    gl_shader();
    ~gl_shader();
    gl_shader(const gl_shader&) = delete;
    gl_shader& operator=(const gl_shader&) = delete;
    gl_shader(gl_shader&&) = default;
    gl_shader& operator=(gl_shader&&) = default;

    [[nodiscard]] bool is_vaild() const noexcept;
    [[nodiscard]] unsigned int shader() const noexcept;
    [[nodiscard]] int get_uniform_loc(std::string_view uniform_name) const noexcept;
    gl_shader& add_vertex(std::string_view vertex_source);
    gl_shader& add_geometry(std::string_view geometry_source);
    gl_shader& add_fragment(std::string_view fragment_source);
    gl_shader& link_shader();
    gl_shader& use();

    template <typename Func, typename... Args>
    gl_shader& uniform(Func func, std::string_view uniform_name, Args&&... args)
    {
        assert(is_vaild());
        // Must call use() first
        assert([this] {
            int current_program = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
            return !(current_program == 0 || current_program != shader_program_.get());
        }());

        auto iter = uniform_loc_map.find(uniform_name.data());
        if (iter != uniform_loc_map.end())
        {
            func(iter->second, std::forward<Args>(args)...);
        } else {
            auto loc = get_uniform_loc(uniform_name);
            if (loc == -1)
                throw shader_error(std::string("Can't find uniform name ") + uniform_name.data());
            uniform_loc_map.emplace(uniform_name.data(), loc);
            func(loc, std::forward<Args>(args)...);
        }
        return *this;
    }

    static constexpr const char* model_name = "model";
    static constexpr const char* view_name = "view";
    static constexpr const char* projection_name = "projection";

private:
    void add_source_(gl_val& shader_index, std::string_view shader_name, std::string_view source);

    gl_val shader_program_;
    gl_val vertex_shader_;
    gl_val fragment_shader_;
    gl_val geometry_shader_;
    bool is_linked_ = false;
    std::unordered_map<std::string, int> uniform_loc_map;
};

} // namespace lomegl