#include "app.h"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "channels/channel.h"
#include "channels/virtual_camera/virtual_camera.h"
#include "channels/background/background.h"
#include "channels/step_ahead/step_ahead.h"
#include "channels/character_animation/character_animation.h"
#include "channels/special_effect/special_effect.h"
#include "channels/plants/plants.h"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const int size[2] = {SCR_WIDTH, SCR_HEIGHT};

App::App()
{
    create_window();
    m_cam = new Camera();
    m_render = new Render(m_cam, size);
    // load_channels();
}

App::~App()
{
    for (unsigned int i = 0; i < m_channels.size(); ++i)
    {
        delete m_channels[i]->ch;
        delete m_channels[i];
    }
    delete m_render;
    delete m_cam;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void App::create_window()
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Error could not load glfw\n");
        return throw;
    }

    const char *glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create window with graphics context
    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (m_window == NULL)
    {
        fprintf(stderr, "Error creating glfw window\n");
        return throw;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        throw;
    }
    glfwSetScrollCallback(m_window, scroll_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void App::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        main_loop();
    }
}

App::ChannelType App::channel_name_to_type(std::string name)
{
    for (unsigned int i = 0; i < channel_type_to_string.size(); ++i)
    {
        if (channel_type_to_string[i] == name)
        {
            return (ChannelType)i;
        }
    }
    return virtual_cam;
}

Channel* App::create_channel(ChannelType type, std::string channel_name)
{
    switch (type)
    {
    case virtual_cam:
        return new VirtualCamera(channel_name);
    case background:
        return new Background(channel_name);
    case step_ahead:
        return new StepAhead(channel_name);
    case character_animation:
        return new CharacterAnimation(channel_name);
    case special_effect:
        return new SpecialEffect(channel_name);
    case plants:
        return new Plants(channel_name);
    default:
        fprintf(stderr, "Failed to create channel: unkown type '%s'", channel_name.c_str());
        throw;
    }
}

void App::add_channel(ChannelType type, Channel* channel){
    ChannelData* data = new ChannelData();
    data->ch = channel;
    data->type = type;
    m_channels.push_back(data);
}

void App::remove_channel(int index){
    fprintf(stderr, "Remove: %s\n", m_channels[index]->ch->get_name());
    delete m_channels[index]->ch;
    m_channels.erase(m_channels.begin() + index);
}
void App::clear_channels(){
    m_channels.clear();
}