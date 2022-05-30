#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <lotools/raii_control.h>

namespace lomegl {

using glfw_control = lot::raii_control<decltype(glfwInit)*, decltype(glfwTerminate)*, glfwInit, glfwTerminate>;

struct glfw_utility
{
    static GLFWwindow* init();
    static void on_window_size_change(GLFWwindow* /*window*/, int width, int height);
    static void process_input(GLFWwindow* window);
    static void calulate_frame_time();
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static float delta_time;      // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static float last_frame_time; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};

} // namespace lomegl