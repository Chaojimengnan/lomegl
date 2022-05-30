#pragma once

#include "lomegl/gl_fwd.h"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <lotools/utility.h>

namespace lomegl {
class gl_world
{
public:
    ~gl_world();
    gl_world(const gl_world&) = delete;
    gl_world(gl_world&&) = delete;
    gl_world& operator=(const gl_world&) = delete;
    gl_world& operator=(gl_world&&) = delete;

    static gl_world& instance();

    gl_world& set_current_camera(std::string_view obj_name);
    gl_world& set_screen_size(int width, int height) noexcept;
    [[nodiscard]] gl_shader& get_current_shader();
    [[nodiscard]] gl_object& get_current_camera();
    [[nodiscard]] std::string get_current_shader_name() noexcept;
    [[nodiscard]] std::string get_current_camera_name() noexcept;
    [[nodiscard]] std::pair<int, int> get_screen_size() noexcept;
    [[nodiscard]] int get_screen_width() const noexcept;
    [[nodiscard]] int get_screen_height() const noexcept;
    gl_world& set_current_shader_view_mat();
    gl_world& set_current_shader_projection_mat(float fov = 45.0F, float near = 0.1F, float far = 100.0F);
    gl_world& use_shader(std::string_view shader_name);

    template <typename T, typename... Args>
    auto& create(std::string_view obj_name, Args&&... args)
    {
        auto* map = get_map_from_derived_type_<T>();

        assert(map->find(obj_name.data()) == map->cend());
        auto&& item = map->emplace(obj_name.data(), std::make_unique<T>(std::forward<Args>(args)...));
        if (!item.second)
            throw std::runtime_error(std::string("emplace texture ") + obj_name.data() + " fails!");
        return static_cast<T&>(*item.first->second.get());
    }

    template <typename T>
    [[nodiscard]] auto& get(std::string_view obj_name)
    {
        auto* map = get_map_from_derived_type_<T>();
        auto&& result = map->find(obj_name.data());

        if (result == map->cend())
            throw std::runtime_error(std::string("Can't find key ") + obj_name.data());

        return *(result->second.get());
    }

    template <typename T>
    auto remove(std::string_view obj_name)
    {
        auto* map = get_map_from_derived_type_<T>();
        return map->erase(obj_name.data());
    }

    template <typename T>
    [[nodiscard]] bool exists(std::string_view obj_name)
    {
        auto* map = get_map_from_derived_type_<T>();
        auto&& result = map->find(obj_name.data());
        return result != map->cend();
    }

private:
    gl_world();

    template <typename T>
    constexpr auto* get_map_from_derived_type_() noexcept
    {
        using BaseType = lot::any_type_true_t<false, std::is_base_of, T, gl_texture, gl_shader, gl_vertex, gl_object>;
        static_assert(!std::is_same_v<BaseType, void>, "T must be or derived from gl_texture, gl_shader, gl_vertex, or gl_object");

        std::unordered_map<std::string, std::unique_ptr<BaseType>>* map = nullptr;
        if constexpr (std::is_base_of_v<gl_texture, T>)
        {
            map = &texture_;
        } else if constexpr (std::is_base_of_v<gl_shader, T>)
        {
            map = &shader_;
        } else if constexpr (std::is_base_of_v<gl_vertex, T>)
        {
            map = &vertex_;
        } else if constexpr (std::is_base_of_v<gl_object, T>)
        {
            map = &object_;
        }
        return map;
    }

    std::unordered_map<std::string, std::unique_ptr<gl_texture>> texture_;
    std::unordered_map<std::string, std::unique_ptr<gl_shader>> shader_;
    std::unordered_map<std::string, std::unique_ptr<gl_vertex>> vertex_;
    std::unordered_map<std::string, std::unique_ptr<gl_object>> object_;
    std::string current_camera_;
    std::string current_shader_;
    std::pair<int, int> screen_size_ = { 0, 0 };
};

} // namespace lomegl
