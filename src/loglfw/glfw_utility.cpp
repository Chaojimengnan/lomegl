#include "lomegl/loglfw/glfw_utility.h"
#include "lomegl/gl_object.h"
#include "lomegl/gl_world.h"

#include <stdexcept>

namespace lomegl {
float glfw_utility::delta_time = 0;      // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
float glfw_utility::last_frame_time = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

void glfw_utility::on_window_size_change(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfw_utility::process_input(GLFWwindow* window)
{
    float camera_speed = 5.0F * delta_time;
    auto&& my_camera = lomegl::gl_world::instance().get_current_camera();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        my_camera.add_pos_z_local(camera_speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        my_camera.add_pos_z_local(-camera_speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        my_camera.add_pos_x_local(camera_speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        my_camera.add_pos_x_local(-camera_speed);
}

GLFWwindow* glfw_utility::init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
        throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(window);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        throw std::runtime_error("Failed to initialize GLAD");

    return window;
}

void glfw_utility::calulate_frame_time()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    delta_time = currentFrame - last_frame_time;
    last_frame_time = currentFrame;
}

void glfw_utility::mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    static float lastX = 400;
    static float lastY = 300;
    static bool firstMouse = true;

    if (firstMouse) // 这个bool变量初始时是设定为true的
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    auto xoffset = static_cast<float>(lastX - xpos);
    auto yoffset = static_cast<float>(ypos - lastY); // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    float sensitivity = 0.005F;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    lomegl::gl_world::instance().get_current_camera().add_angle_local(0, glm::radians(xoffset), glm::radians(yoffset));
}
} // namespace lomegl
