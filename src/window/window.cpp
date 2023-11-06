#include "window.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const char* glsl_version = "#version 330 core";

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Window::Window(int width, int height){
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        throw;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    static ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}
Window::~Window(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::isOpen()const{
    return !glfwWindowShouldClose(window);
}
void Window::close(){
    glfwSetWindowShouldClose(window, true);
}

void Window::poll(){
    glfwPollEvents();
}
void Window::swapBuffer(){
    glfwSwapBuffers(window);
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void Window::mousePosCallback(GLFWwindow* window, double xpos, double ypos){
    // Window::mouse_pos[0] = xpos;
    // Window::mouse_pos[1] = ypos;
}

void Window::keyCallback(GLFWkeyfun fun) const{
    // glfwSetKeyCallback(window, fun);
}
void Window::mouseCallback(GLFWmousebuttonfun fun) const{
    // glfwSetMouseButtonCallback(window,fun);
}
void Window::mouseMoveCallback(GLFWcursorposfun fun) const{
    // glfwSetCursorPosCallback(window,fun);
}
void Window::mousePos(double& x, double& y) const{
    glfwGetCursorPos(window, &x, &y);
}

Window::WindowSize Window::getSize() const{
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
}